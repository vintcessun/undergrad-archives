#pragma once

#include <Eigen/Core>
#include <iostream>
using namespace std;
#define PI 3.1415927

extern Eigen::Matrix4f mModelView;
extern Eigen::Matrix4f mProjection;
extern Eigen::Matrix4f mViewPort;

// 测试函数：演示 Eigen 的基本用法
void Test()
{
	// 参考 Eigen 快速参考手册
	// http://eigen.tuxfamily.org/dox/group__QuickRefPage.html

	// 矩阵示例
	Eigen::Matrix4f m;
	m.setZero();
	m(0, 0) = 1;
	m(3, 3) = 1;
	m(1, 1) = 2.3;   m(1, 2) = 0.99;
	m(2, 1) = 1.1;   m(2, 2) = 0;
	cout << "Matrix:" << endl;
	cout << m << endl;

	// 向量示例
	Eigen::Vector4f v;
	v[0] = 1.0;
	v[1] = 2.0;
	v[2] = 3.0;
	v[3] = 1.0;
	v.normalize(); // 单位化

	// 计算并输出结果
	cout << "result:" << endl;
	cout << m * v << endl;
}

// 顶点变换：将顶点从模型空间变换到屏幕空间
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

// 绕 X 轴旋转 angle（度）
Eigen::Matrix4f myRotateX(float angle)
{
	float theta = angle / 180.0 * PI;
	Eigen::Matrix4f m;
	m.setZero();
	// 初始化变换矩阵

	// m.setIdentity(); // 如需使用单位矩阵，可取消注释
	float costheta = cos(theta);
	float sintheta = sin(theta);
	m(0, 0) = 1;
	m(3, 3) = 1;
	m(1, 1) = costheta;   m(1, 2) = -sintheta;
	m(2, 1) = sintheta;   m(2, 2) = costheta;

	return m;
}

// 绕 Y 轴旋转 angle（度）
Eigen::Matrix4f myRotateY(float angle)
{
	float theta = angle / 180.0 * PI;
	Eigen::Matrix4f m;
	m.setZero();
	// 初始化变换矩阵

	// m.setIdentity(); // 如需使用单位矩阵，可取消注释
	float costheta = cos(theta);
	float sintheta = sin(theta);
	m(1, 1) = 1;
	m(3, 3) = 1;
	m(0, 0) = costheta;
	m(0, 2) = sintheta;
	m(2, 0) = -sintheta;
	m(2, 2) = costheta;

	//cout << m << endl;
	return m;
}

// 绕 Z 轴旋转 angle（度）
Eigen::Matrix4f myRotateZ(float angle)
{
	float theta = angle / 180.0 * PI;
	Eigen::Matrix4f m;
	m.setZero();
	// 初始化变换矩阵

	// m.setIdentity(); // 如需使用单位矩阵，可取消注释
	float costheta = cos(theta);
	float sintheta = sin(theta);
	m(2, 2) = 1;
	m(3, 3) = 1;
	m(0, 0) = costheta;
	m(0, 1) = -sintheta;
	m(1, 0) = sintheta;
	m(1, 1) = costheta;

	//cout << m << endl;
	return m;
}


// 平移变换：沿 x, y, z 平移
Eigen::Matrix4f myTranslate(float x, float y, float z)
{
	Eigen::Matrix4f m;
	m.setZero();
	// 初始化变换矩阵

	// m.setIdentity(); // 如需使用单位矩阵，可取消注释
	m(0, 0) = 1;
	m(1, 1) = 1;
	m(2, 2) = 1;
	m(3, 3) = 1;

	m(0, 3) = x;
	m(1, 3) = y;
	m(2, 3) = z;

	//cout << m << endl;
	return m;
}

// 绕任意轴 (x,y,z) 旋转 angle（度）。注意：轴向量会被归一化
Eigen::Matrix4f myRotate(float angle, float x, float y, float z)
{
	float theta = angle / 180.0 * PI;

	Eigen::Matrix4f m;

	// 初始化和构造旋转矩阵

	// m.setIdentity(); // 如需使用单位矩阵，可取消注释

	// 计算并归一化旋转轴
	Eigen::Vector3f normal;
	normal[0] = x;
	normal[1] = y;
	normal[2] = z;
	normal.normalize();

	// 构造旋转矩阵
	m.setZero();
	m(0, 0) = cos(theta) + (1 - cos(theta)) * normal[0] * normal[0];
	m(0, 1) = (1 - cos(theta)) * normal[0] * normal[1] - normal[2] * sin(theta);
	m(0, 2) = (1 - cos(theta)) * normal[0] * normal[2] + normal[1] * sin(theta);
	m(1, 0) = (1 - cos(theta)) * normal[0] * normal[1] + normal[2] * sin(theta);
	m(1, 1) = cos(theta) + (1 - cos(theta)) * normal[1] * normal[1];
	m(1, 2) = (1 - cos(theta)) * normal[1] * normal[2] - normal[0] * sin(theta);
	m(2, 0) = (1 - cos(theta)) * normal[0] * normal[2] - normal[1] * sin(theta);
	m(2, 1) = (1 - cos(theta)) * normal[1] * normal[2] + normal[0] * sin(theta);
	m(2, 2) = cos(theta) + (1 - cos(theta)) * normal[2] * normal[2];
	m(3, 3) = 1;

	return m;
}

// 缩放变换：沿 x, y, z 缩放
Eigen::Matrix4f myScalef(float x, float y, float z)
{

	Eigen::Matrix4f m;

	// 初始化变换矩阵

	// m.setIdentity(); // 如需使用单位矩阵，可取消注释
	m.setIdentity();
	m(0, 0) = x;
	m(1, 1) = y;
	m(2, 2) = z;

	return m;
}

