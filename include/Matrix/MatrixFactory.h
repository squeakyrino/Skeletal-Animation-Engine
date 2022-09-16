#pragma once
#include <Matrix\Mat2.h>
#include <Matrix\Mat3.h>
#include <Matrix\Mat4.h>
#include <Vectors/Vec3.h>
#include <Quaternion\Quaternion.h>

namespace Engine {
	class MatrixFactory 
	{
		MatrixFactory();
	public:
		static Mat2 identityMat2();
		static Mat3 identityMat3();
		static Mat4 identityMat4();

		static Mat3 dualMatrix(Vec3 m);

		static Mat4 scalingMatrix(float scale);
		static Mat4 scalingMatrix(float scaleX, float scaleY, float scaleZ);
		static Mat4 scalingMatrix(Vec3 v);

		static Mat4 translationMatrix(float dx, float dy, float dz);
		static Mat4 translationMatrix(Vec3 v);

		static Mat4 rotationMatrixByX(float degrees);
		static Mat4 rotationMatrixByY(float degrees);
		static Mat4 rotationMatrixByZ(float degrees);

		static Mat4 rotationByAxis(Vec3 axis, float degrees);

		static Mat4 ortho(float left, float right, float top, float bottom, float near, float far);
		static Mat4 perspective(float fov, float aspectRatio, float near, float far);

		static Mat4 viewMatrix(Vec3 eye, Vec3 center, Vec3 up);

		static Mat4 rotationByAxis(Quaternion q);
	};
}