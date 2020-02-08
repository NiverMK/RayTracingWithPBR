#include "Render.h"


Render::Render(const uint32_t &_width, const uint32_t &_height) {

	//frame size
	width = _width;
	height = _height;

	gamma = 2.2;

	//1 reflection with 1 ray
	raysPerIteration_fast.push_back(0);
	//2 reflections. at 1 - 500 rays, at 2 - 1 ray
	raysPerIteration_long.push_back(500);
	raysPerIteration_long.push_back(0);

	pixels_rgb = new Vector3D[width * height];
}



Render::~Render() {

	threadsPool.terminateAllThreads();

	delete[] pixels_rgb;
}



void Render::renderFrame(Scene &scene) {

	//adding functions to threads queue
	for (uint32_t i = 0; i < height; i = i + 1) {
		threadsPool.addFunctionToQueue([&, i] { traceRays(scene, i, i + 1); });
	}

	raysPerIteration = raysPerIteration_long;

	threadsPool.resumeAllThreads();
}



void Render::renderFastFrame(Scene &scene) {

	//adding functions to threads queue
	for (uint32_t i = 0; i < height; i = i + 4) {
		threadsPool.addFunctionToQueue([&, i] { traceFastRays(scene, i, i + 1); });
	}

	raysPerIteration = raysPerIteration_fast;

	threadsPool.resumeAllThreads();
}



void Render::traceRays(Scene &scene, const uint32_t height_start, const uint32_t height_end) {
	
	for (uint32_t _height = height_start; _height < height_end; _height = _height + 1) {
		for (uint32_t _width = 0; _width < width; _width = _width + 1) {

			uint32_t iteration = 0;		//number of current iteration

			//get ray from camera
			Vector3D ray = scene.camera.getRay(_width, _height);

			Vector3D pixel = renderPixel(scene, scene.camera.getPosition(), ray, iteration);
			 
			pixels_rgb[_height * width + _width] = Reinhard_Tone_Mapping(pixel);

			pixelGammaCorrection(pixels_rgb[_height * width + _width]);
		}
	}
}



void Render::traceFastRays(Scene &scene, const uint32_t height_start, const uint32_t height_end) {

	for (uint32_t _height = height_start; _height < height_end; _height = _height + 4) {
		for (uint32_t _width = 0; _width < width; _width = _width + 4) {

			uint32_t iteration = 0;		//number of current iteration

			//get ray from camera
			Vector3D ray = scene.camera.getRay(_width, _height);

			Vector3D pixel = renderPixel(scene, scene.camera.getPosition(), ray, iteration);

			pixels_rgb[_height * width + _width] = Reinhard_Tone_Mapping(pixel);

			pixelGammaCorrection(pixels_rgb[_height * width + _width]);

			//upscale one pixel to 4x4
			for (int h = 0; h < 4; h++) {
				for (int w = 0; w < 4; w++) {
					pixels_rgb[(_height + h) * width + _width + w] = pixels_rgb[_height * width + _width];
				}
			}
		}
	}
}



Vector3D Render::renderPixel(Scene &scene, const Vector3D &position, Vector3D ray, uint32_t iteration) {
	
	Vector3D pixel(0);

	//search for the intersection point of the ray and the object
	auto [pointer, normal, length] = findIntersection(scene.objects, position, ray);

	if (pointer) {		//if the pointer exists, then the intersection point exists
		
		Vector3D pointPos = position + ray * length;		//point of intersection of the ray with the object

		ray.invert();


		if (iteration == 0) {
			//calculating Blinn-Phong lighting
			pixel = pixel + pointer->getMaterial()->shader.calculateBlinnPhongLight(pointPos, ray, normal, scene.lights);
		} else {
			//calculating diffuse lighting
			pixel = pixel + pointer->getMaterial()->shader.calculateDiffuseLight(pointPos, normal, scene.lights);
		}
	
		//reflection calculation
		if (iteration < raysPerIteration.size()) {	

			Vector3D median;			//micronormal
			Vector3D ray_reflected;		//reflected ray
			Vector3D pixel_reflect(0);	//reflection light
			
			//amount of reflected rays depending on roughness
			int _numOfRays = (int)(raysPerIteration[iteration] * pow(pointer->getMaterial()->getRoughness(), 1.0 / 8))  + 1;

			iteration++;		//no reflections at the last iteration

			for (int i = 0; i < _numOfRays; i++) {

				//generate micronormal
				median = GenerateHammersleyRay(i, _numOfRays, normal, pointer->getMaterial()->getRoughness());

				//find reflected ray
				ray_reflected = 2.0 * median * dot(ray, median) - ray;


				if (dot(normal, ray_reflected) > 0) {

					Vector3D reflectionLight = renderPixel(scene, pointPos, ray_reflected, iteration);

					pixel_reflect = pixel_reflect + pointer->getMaterial()->shader.calculateReflection(ray, normal, median, ray_reflected, reflectionLight);
				}
			}

			//result
			pixel = pixel + pixel_reflect / _numOfRays;
		}
	}

	return pixel;
}


std::tuple<Object3D*, Vector3D, double> Render::findIntersection(std::vector<Object3D *> &objects, const Vector3D &position, Vector3D &ray) {
	
	double length = DBL_MAX;		//ray multiplier
	
	Vector3D normal;				//normal vector
	Object3D *pointer = NULL;		//object pointer

	for (auto object : objects) {		
		
		auto [_pointer, _normal, _length] = object->isIntersectsRay(position, ray);
		
		if (_length > 0 && _length < length) {

			//if camera is not behind the front side of the object
			if (dot(ray.getInverted(), _normal) > 0) {
				
				pointer = _pointer;
				length = _length - 0.0001;	
				normal = _normal;

				continue;		//continue cycle
			}
			//if object's material is double-sided
			if (_pointer->getMaterial()->isDoubleSided) {
				_normal.invert();

				pointer = _pointer;
				length = _length - 0.0001;
				normal = _normal;
			}
		}
	}

	return { pointer, normal, length };
}


Vector3D Render::GenerateHammersleyRay(const uint32_t &i, const uint32_t &num, const Vector3D &N, const double &roughness){
	//vector coordinates in a spherical system
	double azimuthAngle = (double)i / num;
	double polarAngle = VanDerCorput(i, 2);

	Vector3D vec = ImportanceSampleGGX(azimuthAngle, polarAngle, N, roughness);

	return vec;
}


double Render::VanDerCorput(int32_t n, uint32_t base){

	double invBase = 1.0 / base;
	double denom = 1.0;
	double result = 0.0;

	for (int i = 0; i < 32; ++i){

		if (n > 0){

			denom = n - 2.0 * (int)(n / 2.0);
			result += denom * invBase;
			invBase = invBase / 2.0;
			n = int32_t(double(n) / 2.0);
		}
	}

	return result;
}


Vector3D Render::ImportanceSampleGGX(const double &azimuthAngle, const double &polarAngle, const Vector3D &N, const double &roughness){

	double a = roughness * roughness;

	/*roughness = 1		polarAngle = 45
	roughness = 0		polarAngle - 0 */
	double phi = 2.0 * PI * azimuthAngle;
	double cosTheta = sqrt((1.0 - polarAngle) / (1.0 + (a * a - 1.0) * polarAngle));
	double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

	//conversion from spherical to cartesian coordinates
	Vector3D H;
	H.x = cos(phi) * sinTheta;
	H.y = sin(phi) * sinTheta;
	H.z = cosTheta;
	
	
	//transform vector from local coordinates to global
	Vector3D middle_N = Vector3D(0, 0, 0.999) + N;

	Vector3D result = 2 * middle_N * (dot(middle_N, H) / dot(middle_N, middle_N)) - H;
	
	return result;
}



Vector3D *Render::isRendered() {

	if (threadsPool.isThreadFinished())
		return pixels_rgb;
	else
		return NULL;
}



void Render::stopRendering() {
	threadsPool.pauseAllThreads();
}



void Render::pixelGammaCorrection(Vector3D &pixel) {

	pixel.x = pow(pixel.x, 1.0 / gamma);
	pixel.y = pow(pixel.y, 1.0 / gamma);
	pixel.z = pow(pixel.z, 1.0 / gamma);
}



Vector3D Render::Reinhard_Tone_Mapping(Vector3D &colour) {
	Vector3D _colour = colour;

	_colour.x = _colour.x / (_colour.x + 1.0);
	_colour.y = _colour.y / (_colour.y + 1.0);
	_colour.z = _colour.z / (_colour.z + 1.0);

	return _colour;
}
