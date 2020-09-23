#pragma once

#include <vector>
#include <tuple>
#include "Vector3D.h"
#include "ThreadPool.h"
#include "ReturnData.h"

class Scene;

class Render
{
public:
	/* pixels rgb = xyz [0...1] */
	Vector3D* pixelsRGB;

	double gamma;
	/* number of reflected rays per iteration. number of iterations = container size */
	std::vector<uint32_t> raysPerIterationFast;		/* for long render */
	std::vector<uint32_t> raysPerIterationLong;		/* for fast render */

	Render(const size_t _width = 1280, const size_t _height = 720);
	~Render();

	/* render high-resolution frame */
	void renderFrame(Scene& scene);
	/* render low-resolution frame */
	void renderFastFrame(Scene& scene);
	/* was frame rendering finished. return nullptr if it's not */
	Vector3D* isRendered();
	/* stop rendering and pause all threads */
	void stopRendering();

protected:
	size_t fastRenderPrescaler;
	/* Current container of reflected rays number per iteration. Container size - number of iterations.
	If roughness -> 0, then number of rays -> 1. If roughness -> 1, then number of rays -> max */
	std::vector<uint32_t> raysPerIteration;

	ThreadPool threadsPool;

	/* width and height of high-resolution frame */
	size_t renderWidth;
	size_t renderHeight;

	/* trace rays for high-reso;ution frame */
	void traceRays(Scene& scene, size_t heightStart, size_t heightEnd);
	/* trace rays for low-resolution frame */
	void traceFastRays(Scene& scene, size_t heightStart, size_t heightEnd);
	/* find intersection point of ray and object. returns pointer to object, normal vector and scaler for ray to calculate point's coordinates */
	FigureData findIntersection(std::vector<Figure*>& objects, const Vector3D& position, const Vector3D& ray);
	/* render pixel*/
	Vector3D renderPixel(Scene& scene, const Vector3D& position, Vector3D ray, uint32_t iteration, double roughness = 0.0);
	/* tonal compression */
	void ReinhardToneMapping(Vector3D& colour);
	/* gamma correction */
	void pixelGammaCorrection(Vector3D& pixel);
	/* generate ray */
	Vector3D GenerateHammersleyRay(const Vector3D& mainVec, int32_t i, int32_t num, double roughness);
	/* hemispherical ray generation */
	double VanDerCorput(int32_t n, int32_t base);
	/* compression of the generated hemisphere rays lobe to the desired angle in accordance with the roughness */
	Vector3D ImportanceSampleGGX(const Vector3D& mainVec, double azimuthAngle, double polarAngle, double roughness);
};
