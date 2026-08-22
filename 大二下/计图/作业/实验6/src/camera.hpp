#pragma once
#include "ray.hpp"
#include <Eigen/Core>
using namespace Eigen;

class Camera
{
public:
	Camera()
	{
		_origin = Vector3f(0, 0, 0);
		_lower_left_corner = Vector3f(-2.0, -1.5, -1.0);
		_horizontal = Vector3f(4.0, 0.0, 0.0);
		_vetical = Vector3f(0.0, 3.0, 0.0);
	}

	Ray GenerateRay(float u, float v)
	{
		return Ray(_origin,
			_lower_left_corner + u * _horizontal + v * _vetical - _origin);
	}

	Vector3f _origin;
	Vector3f _lower_left_corner;
	Vector3f _horizontal;
	Vector3f _vetical;
};
