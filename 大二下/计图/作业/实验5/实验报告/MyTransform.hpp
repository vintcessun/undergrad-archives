#pragma once

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <iostream>
using namespace std;
#define PI 3.1415927

extern Eigen::Matrix4f mModelView;
extern Eigen::Matrix4f mProjection;
extern Eigen::Matrix4f mViewPort;


void Test()
{
	//矩阵用法
	Eigen::Matrix4f m;
	m.setZero();
	m(0, 0) = 1;
	m(3, 3) = 1;
	m(1, 1) = 2.3;   m(1, 2) = 0.99;
	m(2, 1) = 1.1;   m(2, 2) = 0;
	cout << "Matrix:" << endl;
	cout << m << endl;

	//向量用法
	Eigen::Vector4f v;
	v[0] = 1.0;
	v[1] = 2.0;
	v[2] = 3.0;
	v[3] = 1.0;
	v.normalize();//归一化

	//输出矩阵与向量相乘的结果
	cout << "result:" << endl;
	cout << m * v << endl;
}

vector<Eigen::Vector3f> VertexTransform(vector<Eigen::Vector3f> vertices)
{
	vector<Eigen::Vector3f> rlt(vertices.size());
	Eigen::Vector4f v;

	Eigen::Matrix4f M = mViewPort * mProjection * mModelView;

	for (int i = 0; i < vertices.size(); ++i)
	{
		v[3] = 1.0;
		v.head<3>() = vertices[i];

		v = M * v;
		rlt[i][0] = v[0] / v[3];
		rlt[i][1] = v[1] / v[3];
		rlt[i][2] = v[2] / v[3];
	}
	return rlt;
}

Eigen::Matrix4f myLookAt(float ex, float ey, float ez, float atx, float aty, float atz, float upx, float upy, float upz)
{
	Eigen::Matrix4f m;
	m.setZero();

	// 计算相机坐标系的三个正交基向量
	// 前向向量 f = normalize(at - eye)
	Eigen::Vector3f eye(ex, ey, ez);
	Eigen::Vector3f at(atx, aty, atz);
	Eigen::Vector3f up(upx, upy, upz);

	Eigen::Vector3f f = (at - eye).normalized();   // 前向（z轴负方向）
	Eigen::Vector3f s = f.cross(up).normalized();  // 右向（x轴方向）
	Eigen::Vector3f u = s.cross(f);                // 上向（y轴方向）

	// 旋转矩阵（将世界坐标系转到相机坐标系）
	// 平移矩阵（将eye移到原点）
	// LookAt = R * T
	m(0, 0) =  s[0];  m(0, 1) =  s[1];  m(0, 2) =  s[2];  m(0, 3) = -s.dot(eye);
	m(1, 0) =  u[0];  m(1, 1) =  u[1];  m(1, 2) =  u[2];  m(1, 3) = -u.dot(eye);
	m(2, 0) = -f[0];  m(2, 1) = -f[1];  m(2, 2) = -f[2];  m(2, 3) =  f.dot(eye);
	m(3, 0) =  0.0f;  m(3, 1) =  0.0f;  m(3, 2) =  0.0f;  m(3, 3) =  1.0f;

	mModelView *= m;
	return m;
}

Eigen::Matrix4f myRotateX(float angle)
{
	float theta = angle / 180.0 * PI;
	Eigen::Matrix4f m;
	m.setZero();

	m(0, 0) = 1.0f;
	m(1, 1) =  cos(theta);  m(1, 2) = -sin(theta);
	m(2, 1) =  sin(theta);  m(2, 2) =  cos(theta);
	m(3, 3) = 1.0f;

	mModelView *= m;
	return m;
}

Eigen::Matrix4f myRotateY(float angle)
{
	float theta = angle / 180.0 * PI;
	Eigen::Matrix4f m;
	m.setZero();

	m(0, 0) =  cos(theta);  m(0, 2) =  sin(theta);
	m(1, 1) =  1.0f;
	m(2, 0) = -sin(theta);  m(2, 2) =  cos(theta);
	m(3, 3) =  1.0f;

	mModelView *= m;
	return m;
}

Eigen::Matrix4f myRotateZ(float angle)
{
	float theta = angle / 180.0 * PI;
	Eigen::Matrix4f m;
	m.setZero();

	m(0, 0) =  cos(theta);  m(0, 1) = -sin(theta);
	m(1, 0) =  sin(theta);  m(1, 1) =  cos(theta);
	m(2, 2) =  1.0f;
	m(3, 3) =  1.0f;

	mModelView *= m;
	return m;
}


Eigen::Matrix4f myTranslate(float x, float y, float z)
{
	Eigen::Matrix4f m;
	m.setZero();

	m(0, 0) = 1.0f;  m(0, 3) = x;
	m(1, 1) = 1.0f;  m(1, 3) = y;
	m(2, 2) = 1.0f;  m(2, 3) = z;
	m(3, 3) = 1.0f;

	mModelView *= m;
	return m;
}

Eigen::Matrix4f myRotate(float angle, float x, float y, float z)
{
	float theta = angle / 180.0 * PI;

	Eigen::Matrix4f m;

	// 归一化旋转轴
	Eigen::Vector3f axis(x, y, z);
	axis.normalize();
	float nx = axis[0], ny = axis[1], nz = axis[2];
	float c = cos(theta), s = sin(theta), t = 1.0f - c;

	// Rodrigues旋转公式
	m(0, 0) = t*nx*nx + c;      m(0, 1) = t*nx*ny - s*nz;  m(0, 2) = t*nx*nz + s*ny;  m(0, 3) = 0;
	m(1, 0) = t*nx*ny + s*nz;  m(1, 1) = t*ny*ny + c;      m(1, 2) = t*ny*nz - s*nx;  m(1, 3) = 0;
	m(2, 0) = t*nx*nz - s*ny;  m(2, 1) = t*ny*nz + s*nx;  m(2, 2) = t*nz*nz + c;      m(2, 3) = 0;
	m(3, 0) = 0;                m(3, 1) = 0;                m(3, 2) = 0;                m(3, 3) = 1;

	mModelView *= m;

	return m;
}

Eigen::Matrix4f myFrustum(float l, float r, float b, float t, float n, float f)
{
	Eigen::Matrix4f m;
	m.setZero();

	// OpenGL glFrustum 透视投影矩阵
	// 将视锥体内的顶点映射到NDC [-1,1]^3
	m(0, 0) = 2.0f * n / (r - l);
	m(0, 2) = (r + l) / (r - l);
	m(1, 1) = 2.0f * n / (t - b);
	m(1, 2) = (t + b) / (t - b);
	m(2, 2) = -(f + n) / (f - n);
	m(2, 3) = -2.0f * f * n / (f - n);
	m(3, 2) = -1.0f;

	mProjection *= m;
	return m;
}

Eigen::Matrix4f myOrtho(float l, float r, float b, float t, float n, float f)
{
	Eigen::Matrix4f m;
	m.setZero();

	// OpenGL glOrtho 正交投影矩阵
	// 先平移使中心到原点，再缩放到 [-1,1]^3
	m(0, 0) =  2.0f / (r - l);  m(0, 3) = -(r + l) / (r - l);
	m(1, 1) =  2.0f / (t - b);  m(1, 3) = -(t + b) / (t - b);
	m(2, 2) = -2.0f / (f - n);  m(2, 3) = -(f + n) / (f - n);
	m(3, 3) =  1.0f;

	mProjection *= m;

	return m;
}

Eigen::Matrix4f myViewPort(float x, float y, float w, float h)
{
	Eigen::Matrix4f m;
	m.setZero();

	// 将NDC [-1,1]^2 映射到屏幕像素坐标 [x, x+w] x [y, y+h]
	m(0, 0) = w / 2.0f;
	m(0, 3) = x + w / 2.0f;
	m(1, 1) = h / 2.0f;
	m(1, 3) = y + h / 2.0f;
	m(2, 2) = 1.0f;
	m(3, 3) = 1.0f;

	mViewPort = m;

	return m;
}

Eigen::Matrix4f myPerspective(float fovy, float aspect, float zNear, float zFar)
{
	Eigen::Matrix4f m;
	m.setZero();
	const float pi = 3.1415926535897932384626433832795;

	// 由fovy和aspect计算视锥体的l,r,b,t，再调用Frustum公式
	float t = zNear * tan(fovy / 2.0f * pi / 180.0f);
	float b = -t;
	float r = t * aspect;
	float l = -r;

	m(0, 0) = 2.0f * zNear / (r - l);
	m(0, 2) = (r + l) / (r - l);
	m(1, 1) = 2.0f * zNear / (t - b);
	m(1, 2) = (t + b) / (t - b);
	m(2, 2) = -(zFar + zNear) / (zFar - zNear);
	m(2, 3) = -2.0f * zFar * zNear / (zFar - zNear);
	m(3, 2) = -1.0f;

	mProjection *= m;
	return m;
}
