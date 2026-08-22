#include <Eigen/Core>
#include "ppm.hpp"
#include "camera.hpp"
#include "raytracer.hpp"
#include <filesystem>
#include <iostream>
#include <string>
using namespace std;

Scene BuildScene(bool modifiedGround, bool includeFallingBall, float fallingBallY)
{
	Scene world;
	Sphere sp1(Vector3f(0, 0.5, -2), 1.0);
	Sphere sp2(Vector3f(0, -10000.5, -2), 10000.0);
	Sphere sp3(Vector3f(-2.0, 0, -2), 0.5);
	Sphere sp4(Vector3f(1.0, -0.2, -1.0), 0.3);
	Sphere sp5(Vector3f(3.0, 0, -2.2), 0.4);
	Sphere fallingBall(Vector3f(-0.8, fallingBallY, -1.2), 0.22);

	Material mtl, mtl2, mtl3, mtl4, mtl5, mtl6;
	mtl.SetKa(Vector3f(0.5, 0.5, 0.5));
	mtl.SetKd(Vector3f(0.8, 0.6, 0.0));
	mtl.SetKs(Vector3f(0.7, 0.8, 0.8));
	mtl.SetTransparent(false);
	mtl.SetReflective(true);
	mtl.SetShiness(50);

	mtl2.SetKa(Vector3f(0.5, 0.5, 0.5));
	mtl2.SetKd(modifiedGround ? Vector3f(0.7, 0.2, 0.2) : Vector3f(0.0, 0.6, 0.2));
	mtl2.SetKs(Vector3f(0.1, 1.0, 0.8));
	mtl2.SetTransparent(false);
	mtl2.SetReflective(false);
	mtl2.SetShiness(10);

	mtl3.SetKa(Vector3f(0.5, 0.5, 0.5));
	mtl3.SetKd(Vector3f(0.0, 0.3, 0.6));
	mtl3.SetKs(Vector3f(0.0, 0.0, 0.0));
	mtl3.SetTransparent(false);
	mtl3.SetReflective(false);
	mtl3.SetShiness(10);

	mtl4.SetKa(Vector3f(0.1, 0.1, 0.1));
	mtl4.SetKd(Vector3f(0.0, 0.0, 0.0));
	mtl4.SetKs(Vector3f(0.0, 0.0, 0.0));
	mtl4.SetTransparent(true);
	mtl4.SetReflective(false);
	mtl4.SetRefraction(1.33);
	mtl4.SetShiness(200);

	mtl5.SetKa(Vector3f(0.5, 0.5, 0.5));
	mtl5.SetKd(Vector3f(0.75, 0.56, 0.0));
	mtl5.SetKs(Vector3f(0.75, 0.56, 0.0));
	mtl5.SetTransparent(false);
	mtl5.SetReflective(false);
	mtl5.SetRefraction(1.33);
	mtl5.SetShiness(100);

	mtl6.SetKa(Vector3f(0.4, 0.4, 0.4));
	mtl6.SetKd(Vector3f(0.9, 0.1, 0.1));
	mtl6.SetKs(Vector3f(0.9, 0.9, 0.9));
	mtl6.SetTransparent(false);
	mtl6.SetReflective(false);
	mtl6.SetShiness(80);

	world.Insert(make_pair(sp1, mtl));
	world.Insert(make_pair(sp2, mtl2));
	world.Insert(make_pair(sp3, mtl3));
	world.Insert(make_pair(sp4, mtl4));
	world.Insert(make_pair(sp5, mtl5));
	if (includeFallingBall)
		world.Insert(make_pair(fallingBall, mtl6));

	return world;
}

void RenderScene(Scene& world, int width, int height, const char* filename)
{
	PPM outrlt(width, height);
	Camera cam;
	for (int j = height - 1; j >= 0; --j)
	{
		for (int i = 0; i < width; ++i)
		{
			float u = float(i) / float(width);
			float v = float(j) / float(height);
			Ray ray = cam.GenerateRay(u, v);
			Vector3f color = RayColor(ray, world);

			outrlt.SetPixel(j, i,
				int(255.99 * color[0]), int(255.99 * color[1]), int(255.99 * color[2]));
		}
	}
	outrlt.Write2File(filename);
}

void RenderAnimation()
{
	const int width = 320;
	const int height = 240;
	const int frameCount = 24;
	const float dt = 0.08f;
	const float gravity = 9.8f;
	const float radius = 0.22f;
	const float groundY = -0.5f + radius;
	const float bounce = 0.78f;

	filesystem::create_directories("animation_frames");

	float y = 2.4f;
	float velocity = 0.0f;
	for (int frame = 0; frame < frameCount; ++frame)
	{
		char filename[128];
		snprintf(filename, sizeof(filename), "animation_frames/frame_%03d.ppm", frame);

		Scene world = BuildScene(true, true, y);
		RenderScene(world, width, height, filename);

		velocity -= gravity * dt;
		y += velocity * dt;
		if (y < groundY)
		{
			y = groundY;
			velocity = -velocity * bounce;
		}
		cout << "Rendered animation frame " << frame << endl;
	}
}

int main(int argc, char** argv)
{
	if (argc > 1 && string(argv[1]) == "--animation")
	{
		RenderAnimation();
		return 0;
	}

	Scene world = BuildScene(true, false, 0.0f);
	RenderScene(world, 1280, 960, "output.ppm");
	return 0;
}
