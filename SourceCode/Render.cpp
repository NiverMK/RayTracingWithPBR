#include <utility>
#include "Render.h"
#include "Constants.h"
#include "Figure.h"
#include "Scene.h"

Render::Render(size_t _width, size_t _height)
	: renderWidth(_width)
	, renderHeight(_height)
{
	/* 1 reflection with 1 ray */
	raysPerIterationFast.push_back(1);
	/* 2 reflections. at 1 - 500 rays, at 2 - 1 ray */
	raysPerIterationLong.push_back(500);
	raysPerIterationLong.push_back(1);

	fastRenderPrescaler = FAST_RENDER_PRESCALER;
	gamma = GAMMA;

	pixelsRGB = new Vector3D[renderWidth * renderHeight];
}

Render::~Render()
{
	/* stop and terminate all threads */
	threadsPool.terminateAllThreads();

	delete[] pixelsRGB;
}

void Render::renderFrame(Scene& scene)
{
	/* add function to queue for execution by threads. for 1280x720 there will be 720 functions */
	for (size_t i = 0; i < renderHeight; i++)
	{
		threadsPool.addFunctionToQueue([&, i] { traceRays(scene, i, i + 1); });
	}

	raysPerIteration = raysPerIterationLong;

	threadsPool.resumeAllThreads();
}

void Render::renderFastFrame(Scene& scene)
{
	/* add function to queue for execution by threads. for 320x180 there will be 180 functions */
	for (size_t i = 0; i < renderHeight; i = i + fastRenderPrescaler)
	{
		threadsPool.addFunctionToQueue([&, i] { traceFastRays(scene, i, i + 1); });
	}

	raysPerIteration = raysPerIterationFast;

	threadsPool.resumeAllThreads();
}

void Render::traceRays(Scene& scene, size_t heightStart, size_t heightEnd)
{
	for (size_t height = heightStart; height < heightEnd; height = height + 1)
	{
		for (size_t width = 0; width < renderWidth; width = width + 1)
		{
			/* get ray and render pixel */
			Vector3D ray = scene.camera.getRay(width, height);
			Vector3D pixel = renderPixel(scene, scene.camera.getPosition(), ray, 0, 0.0);
			 
			/* tonal compression and gamma corrections of pixel */
			ReinhardToneMapping(pixel);
			pixelGammaCorrection(pixel);

			pixelsRGB[height * renderWidth + width] = pixel;
		}
	}
}

void Render::traceFastRays(Scene& scene, size_t heightStart, size_t heightEnd)
{
	for (size_t height = heightStart; height < heightEnd; height = height + fastRenderPrescaler)
	{
		for (size_t width = 0; width < renderWidth; width = width + fastRenderPrescaler)
		{
			/* get ray and render pixel */
			Vector3D ray = scene.camera.getRay(width, height);
			Vector3D pixel = renderPixel(scene, scene.camera.getPosition(), ray, 0, 0.0);

			/* tonal compression and gamma correction of pixel */
			ReinhardToneMapping(pixel);
			pixelGammaCorrection(pixel);

			/* scale one pixel to fastRenderPrescaler X fastRenderPrescaler */
			for (size_t h = 0; h < fastRenderPrescaler; h++)
			{
				for (size_t w = 0; w < fastRenderPrescaler; w++)
				{
					pixelsRGB[(height + h) * renderWidth + width + w] = pixel;
				}
			}
		}
	}
}

Vector3D Render::renderPixel(Scene& scene, const Vector3D& position, Vector3D ray, uint32_t iteration, double roughness)
{
	Vector3D pixel(0.0);

	FigureData intersectionData = findIntersection(scene.figures, position, ray);
	Figure* figurePointer = intersectionData.figure;
	Vector3D normalVec = intersectionData.normal;
	double rayLength = intersectionData.scaler;

	if (figurePointer)
	{
		/* decrease scaler to avoid black dots */
		rayLength -= 0.0001;
		Vector3D intersectionPoint = position + ray * rayLength;
		MaterialData figureMaterialData = figurePointer->getMaterial().getParameters(intersectionData.u, intersectionData.v);

		ray.invert();

		/* increase roughness of reflected objects to avoid white noise */
		double tanh = (exp(16 * roughness) - 1) / (exp(16 * roughness) + 1);
		figureMaterialData.roughness = figureMaterialData.roughness + std::pow((1.0 - figureMaterialData.roughness), 4) * tanh * 0.2;

		pixel = figurePointer->getMaterial().shader.calculateBlinnPhongLight(intersectionPoint, ray, normalVec, scene.lights, figureMaterialData);

		/* calculate reflected light */
		if (iteration < raysPerIteration.size())
		{
			/* reflected ray from object */
			Vector3D rayReflected = 2.0 * intersectionData.normal * dot(ray, normalVec) - ray;
			/* micro-reflected ray from object */
			Vector3D rayReflectedHammersley;
			/* sum of colours from reflected rays */
			Vector3D pixelReflected(0.0);
			/* number of rays, that wasn't casted inside the object */
			double acceptedRaysNumber = DBL_EPSILON;

			/* if roughness -> 0, then number of rays -> 1; 
			if roughness -> 1, then number of rays -> max */
			int32_t raysNumber = static_cast<int32_t>(std::max(1.0, raysPerIteration[iteration] * pow(figureMaterialData.roughness, 1.0 / 8)));

			/* increase reflection iteration */
			iteration++;

			for (int32_t index = 0; index < raysNumber; index++)
			{
				/* generate lobe of reflected rays */
				rayReflectedHammersley = GenerateHammersleyRay(rayReflected, index, raysNumber, figureMaterialData.roughness);

				/* if ray was not casted in object */
				if (dot(normalVec, rayReflectedHammersley) > 0.0)
				{
					acceptedRaysNumber += 1.0;
					Vector3D reflectionLight = renderPixel(scene, intersectionPoint, rayReflectedHammersley, iteration, figureMaterialData.roughness);
					pixelReflected = pixelReflected + figurePointer->getMaterial().shader.calculateReflection(ray, normalVec, rayReflectedHammersley, reflectionLight, figureMaterialData);
				}
			}

			pixel = pixel + pixelReflected / acceptedRaysNumber;
		}
	}

	return pixel;
}

FigureData Render::findIntersection(std::vector<Figure*>& objects, const Vector3D& position, const Vector3D& ray)
{
	FigureData output;

	for (Figure* figure : objects)
	{
		FigureData data = figure->isIntersectsRay(position, ray);
		
		if (data.scaler < output.scaler)
		{
			output = data;
		}
	}

	return output;
}

Vector3D Render::GenerateHammersleyRay(const Vector3D& mainVec, int32_t i, int32_t num, double roughness)
{
	/* coordinates of a vector in a spherical system */
	double azimuthAngle = static_cast<double>(i) / num;
	double polarAngle = VanDerCorput(i, 2);

	return ImportanceSampleGGX(mainVec, azimuthAngle, polarAngle, roughness);
}

double Render::VanDerCorput(int32_t n, int32_t base)
{
	double invBase = 1.0 / base;
	double denom = 1.0;
	double result = 0.0;

	for (int i = 0; i < 32; i++)
	{
		if (n > 0)
		{
			denom = n - 2.0 * static_cast<int32_t>(n / 2.0);
			result += denom * invBase;
			invBase = invBase / 2.0;
			n = static_cast<int32_t>(n / 2.0);
		}
	}

	return result;
}

Vector3D Render::ImportanceSampleGGX(const Vector3D& mainVec, double azimuthAngle, double polarAngle, double roughness)
{
	/* rotate generated vector to vector OZ
	if roughness = 1, angle between generated vector and OZ = 45 degrees. 
	if roughness -> 0, then angle -> 0 */
	double a = roughness * roughness;
	double phi = 2.0 * PI * azimuthAngle;
	double cosTheta = sqrt((1.0 - polarAngle) / (1.0 + (a * a - 1.0) * polarAngle));
	double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

	/* from spherical to cartesian coordinates */
	Vector3D H;
	H.x = cos(phi) * sinTheta;
	H.y = sin(phi) * sinTheta;
	H.z = cosTheta;
	
	/* transform generated vector from local system near vector OZ to global system near vector mainVec */
	/* calculate vector between mainVec and OZ */
	Vector3D middleVec = Vector3D(0.001, 0.0, 1.0) + mainVec;

	/* find reflection of generated vector H from middleVec */
	Vector3D result = 2 * middleVec * (dot(middleVec, H) / dot(middleVec, middleVec)) - H;
	
	return result;
}

Vector3D* Render::isRendered()
{
	if (threadsPool.isThreadFinished())
		return pixelsRGB;
	else
		return nullptr;
}

void Render::stopRendering()
{
	threadsPool.pauseAllThreads();
}

void Render::pixelGammaCorrection(Vector3D& pixel)
{
	double gammaPow = 1.0 / gamma;

	pixel.x = std::pow(pixel.x, gammaPow);
	pixel.y = std::pow(pixel.y, gammaPow);
	pixel.z = std::pow(pixel.z, gammaPow);
}

void Render::ReinhardToneMapping(Vector3D& colour)
{
	colour.x = colour.x / (colour.x + 1.0);
	colour.y = colour.y / (colour.y + 1.0);
	colour.z = colour.z / (colour.z + 1.0);
}
