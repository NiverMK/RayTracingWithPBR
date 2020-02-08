#pragma once

#include <vector>
#include <tuple>

#include "Scene.h"
#include "Vector3D.h"
#include "Camera3D.h"
#include "PointLight.h"
#include "ThreadPool.h"

class Render {
public:

	//number of reflected rays per iteration. Container size = number of iterations
	std::vector<uint32_t> raysPerIteration_fast;	//for fast render
	std::vector<uint32_t> raysPerIteration_long;	//for slow render

	double gamma;			//gamma

	Vector3D *pixels_rgb;	//rgb[0...1]

	Render(const uint32_t &_width = 1280, const uint32_t &_height = 720);
	~Render();

	//render frame
	void renderFrame(Scene &scene);

	//render fast frame with low resolution
	void renderFastFrame(Scene &scene);

	//returns a pointer to pixels_rgb, or null
	Vector3D *isRendered();

	void stopRendering();

protected:

	//number of reflected rays per iteration. Container size = number of iterations
	std::vector<uint32_t> raysPerIteration;

	ThreadPool threadsPool;

	//render size
	uint32_t width;
	uint32_t height;


	//ray tracer
	void traceRays(Scene &scene, const uint32_t height_start, const uint32_t height_end);

	void traceFastRays(Scene &scene, const uint32_t height_start, const uint32_t height_end);

	//search for the intersection point of the ray and the object
	std::tuple<Object3D *, Vector3D, double> findIntersection(std::vector<Object3D *> &objects, const Vector3D &position, Vector3D &ray);

	Vector3D renderPixel(Scene &scene, const Vector3D &position, Vector3D ray, uint32_t iteration);

	//tonal compression
	Vector3D Reinhard_Tone_Mapping(Vector3D &colour);

	//gamma-correction
	void pixelGammaCorrection(Vector3D &pixel);

	Vector3D GenerateHammersleyRay(const uint32_t &i, const uint32_t &num, const Vector3D &N, const double &roughness);

	double VanDerCorput(int32_t n, uint32_t base);

	Vector3D ImportanceSampleGGX(const double &azimuthAngle, const double &polarAngle, const Vector3D &N, const double &roughness);
};
