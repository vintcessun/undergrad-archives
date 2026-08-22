#pragma once
#include "ray.hpp"
#include "scene.hpp"
#include <iostream>
#include <cfloat>
#include <cmath>
#define MAXDEPTH 20

bool Refract(const Vector3f& d, const Vector3f& n,
	float ni_over_nt, Vector3f& refracted)
{
	Vector3f unit_d = d.normalized();
	float dt = unit_d.dot(n);
	float disc = 1.0f - ni_over_nt * ni_over_nt * (1.0f - dt * dt);
	if (disc > 0)
	{
		refracted = ni_over_nt * (unit_d - n * dt) - n * sqrt(disc);
		return true;
	}
	return false;
}

Vector3f RayColor(const Ray& ray, Scene& scene, int depth = 0, bool test = false)
{
	HitInfo closest_hp;
	closest_hp.t = FLT_MAX;
	closest_hp.objIdx = -1;
	for (int i = 0; i < scene.ObjectCount(); ++i)
	{
		HitInfo ht;
		bool bhit = scene.GetObjectPtr(i)->Hit(ray, ht);
		if (bhit && ht.t > 0 && ht.t < closest_hp.t)
		{
			closest_hp = ht;
			closest_hp.objIdx = i;
		}
	}

	Vector3f lightpos(0.0, 4, 2);
	Vector3f lightAmbient(0.6, 0.6, 0.6);
	Vector3f lightSpecular(1.0, 1.0, 1.0);

	if (closest_hp.objIdx != -1)
	{
		int idx = closest_hp.objIdx;
		Material mtl = scene._scene[idx].second;
		Vector3f ambient(
			lightAmbient[0] * mtl._Ka[0],
			lightAmbient[1] * mtl._Ka[1],
			lightAmbient[2] * mtl._Ka[2]);

		Vector3f color = ambient;
		bool isShadow = false;
		Ray shadow_ray(closest_hp.position, lightpos - closest_hp.position);
		float light_distance = shadow_ray.Direction().norm();
		shadow_ray._origin = closest_hp.position + 1e-2f * shadow_ray.Direction().normalized();
		for (int i = 0; i < scene.ObjectCount(); ++i)
		{
			HitInfo shadow_hit;
			if (scene.GetObjectPtr(i)->Hit(shadow_ray, shadow_hit) &&
				shadow_hit.t > 1e-3f && shadow_hit.t < light_distance)
			{
				isShadow = true;
				break;
			}
		}

		Vector3f eyedir = (Vector3f(0, 0, 0) - closest_hp.position).normalized();
		if (!isShadow)
		{
			Vector3f lightdir = (lightpos - closest_hp.position).normalized();
			float coscoef = lightdir.dot(closest_hp.normal);
			if (coscoef < 0) coscoef = 0;
			Vector3f diffuse(
				0.8f * mtl._Kd[0] * coscoef,
				0.8f * mtl._Kd[1] * coscoef,
				0.8f * mtl._Kd[2] * coscoef);

			Vector3f half = (eyedir + shadow_ray.Direction().normalized()).normalized();
			float specularcoef = half.dot(closest_hp.normal);
			if (specularcoef < 0) specularcoef = 0;
			else specularcoef = pow(specularcoef, mtl._shiness);
			Vector3f specular(
				lightSpecular[0] * mtl._Ks[0] * specularcoef,
				lightSpecular[1] * mtl._Ks[1] * specularcoef,
				lightSpecular[2] * mtl._Ks[2] * specularcoef);

			color += diffuse + specular;
		}

		Vector3f reflectColor(0.0, 0.0, 0.0);
		Vector3f reflectDir = 2.0f * eyedir.dot(closest_hp.normal) * closest_hp.normal - eyedir;
		Ray reflectRay(closest_hp.position, reflectDir);
		reflectRay._origin = closest_hp.position + 1e-2f * reflectDir.normalized();
		if (mtl._reflective && depth < MAXDEPTH)
			reflectColor = RayColor(reflectRay, scene, depth + 1, test);

		Vector3f refractionColor(0.0, 0.0, 0.0);
		if (mtl._transparent)
		{
			Vector3f refractDir;
			float ni_over_nt;
			bool refract;
			if (ray.Direction().dot(closest_hp.normal) < 0)
			{
				ni_over_nt = 1.0f / mtl._refraction;
				refract = Refract(ray.Direction(), closest_hp.normal, ni_over_nt, refractDir);
			}
			else
			{
				ni_over_nt = mtl._refraction;
				refract = Refract(ray.Direction(), -closest_hp.normal, ni_over_nt, refractDir);
			}

			if (refract)
			{
				Ray refractRay(closest_hp.position, refractDir);
				refractRay._origin = closest_hp.position + 1e-2f * refractDir.normalized();
				if (depth < MAXDEPTH)
					refractionColor = RayColor(refractRay, scene, depth + 1, test);
			}
			else
			{
				refractionColor = Vector3f(0, 0, 0);
			}
		}

		color = color + 0.2f * reflectColor + 0.9f * refractionColor;
		if (color[0] > 1.0) color[0] = 1.0;
		if (color[1] > 1.0) color[1] = 1.0;
		if (color[2] > 1.0) color[2] = 1.0;
		return color;
	}

	return Vector3f(0.6, 0.8, 0.8);
}
