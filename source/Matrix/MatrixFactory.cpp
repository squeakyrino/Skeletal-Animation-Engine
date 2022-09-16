#include <Matrix/MatrixFactory.h>
#include <EngineHelperFunctions.h>

namespace Engine {
	Mat2 MatrixFactory::identityMat2()
	{
		return Mat2(1, 0, 0, 1);
	}
	Mat3 MatrixFactory::identityMat3()
	{
		std::vector<std::vector<float>> temp(3, std::vector<float>(3, 0));

		for (int i = 0; i < 3; i++)
		{
			temp[i][i] = 1;
		}

		return Mat3(temp);
	}

	Mat4 MatrixFactory::identityMat4()
	{
		std::vector<std::vector<float>> temp(4, std::vector<float>(4, 0));

		for (int i = 0; i < 4; i++)
		{
		
			temp[i][i] = 1;

		}

		return Mat4(temp);
	}

	Mat3 MatrixFactory::dualMatrix(Vec3 m)
	{
		Vec3 mN = m.normalize();

		std::vector<std::vector<float>> temp(3, std::vector<float>(3, 0));

		temp[0][1] = -mN.z;
		temp[0][2] = mN.y;

		temp[1][0] = mN.z;
		temp[1][2] = -mN.x;

		temp[2][0] = -mN.y;
		temp[2][1] = mN.x;

		return Mat3(temp);
	}

	Mat4 MatrixFactory::scalingMatrix(float scale)
	{
		
		Mat4 result = identityMat4();

		result[0][0] = scale;
		result[1][1] = scale;
		result[2][2] = scale;

		return result;
	}

	Mat4 MatrixFactory::scalingMatrix(float scaleX, float scaleY, float scaleZ)
	{
		Mat4 result = identityMat4();

		result[0][0] = scaleX;
		result[1][1] = scaleY;
		result[2][2] = scaleZ;

		return result;
	}

	Mat4 MatrixFactory::scalingMatrix(Vec3 v)
	{
		return scalingMatrix(v.x, v.y, v.z);
	}

	Mat4 MatrixFactory::translationMatrix(float dx, float dy, float dz)
	{
		Mat4 result = identityMat4();

		result[0][3] = dx;
		result[1][3] = dy;
		result[2][3] = dz;

		return result;
	}

	Mat4 MatrixFactory::translationMatrix(Vec3 v)
	{
		return translationMatrix(v.x, v.y, v.z);
	}

	Mat4 MatrixFactory::rotationMatrixByX(float degrees)
	{
		Mat4 result = identityMat4();

		float rad = toRad(degrees);

		result[1][1] = cosf(rad);
		result[1][2] = -sinf(rad);

		result[2][1] = sinf(rad);
		result[2][2] = cosf(rad);

		return result;
	}

	Mat4 MatrixFactory::rotationMatrixByY(float degrees)
	{
		Mat4 result = identityMat4();

		float rad = toRad(degrees);

		result[0][0] = cosf(rad);
		result[0][2] = sinf(rad);

		result[2][0] = -sinf(rad);
		result[2][2] = cosf(rad);

		return result;
	}

	Mat4 MatrixFactory::rotationMatrixByZ(float degrees)
	{
		Mat4 result = identityMat4();

		float rad = toRad(degrees);

		result[0][0] = cosf(rad);
		result[0][1] = -sinf(rad);

		result[1][0] = sinf(rad);
		result[1][1] = cosf(rad);

		return result;
	}

	Mat4 MatrixFactory::rotationByAxis(Vec3 axis, float degrees)
	{
		Mat4 result = identityMat4();

		Vec3 axisN = axis.normalize();
		float rad = toRad(degrees);

		float cos = cosf(rad);
		float sin = sinf(rad);
		
		float x = axisN.x;
		float y = axisN.y;
		float z = axisN.z;

		float x2 = x * x;
		float y2 = y * y;
		float z2 = z * z;

		float xy = x * y;
		float xz = x * z;
		float yz = y * z;

		float c = 1 - cos;

		result[0][0] = cos + x2 * c;
		result[0][1] = xy * c - z * sin;
		result[0][2] = xz * c + y * sin;

		result[1][0] = xy * c + z * sin;
		result[1][1] = cos + y2 * c;
		result[1][2] = yz * c - x * sin;

		result[2][0] = xz * c - y * sin;
		result[2][1] = yz * c + x * sin;
		result[2][2] = cos + z2 * c;

		return result;
	}

	Mat4 MatrixFactory::ortho(float left, float right, float top, float bottom, float near, float far)
	{
		Mat4 result = identityMat4();

		result[0][0] = 2.0f / (right - left);
		result[1][1] = 2.0f / (top - bottom);
		result[2][2] = -2.0f / (far - near);

		result[0][3] = - (right + left) / (right - left);
		result[1][3] = - (top + bottom) / (top - bottom);
		result[2][3] = - (far + near) / (far - near);

		return result;
	}

	Mat4 MatrixFactory::perspective(float fov, float aspectRatio, float near, float far)
	{
		Mat4 result;

		float tanHalfFov = tanf(toRad(fov) / 2);
		float f = 1.0f / tanHalfFov;

		result[0][0] = f / aspectRatio;
		result[1][1] = f;
		result[2][2] = (far + near) / (near - far);
		result[2][3] = (2.0f * far * near) / (near - far);
		result[3][2] = -1.0f;

		return result;

	}

	Mat4 MatrixFactory::viewMatrix(Vec3 eye, Vec3 target, Vec3 up)
	{
		Vec3 zAxis = (eye - target).normalize();
		Vec3 xAxis = up.cross(zAxis).normalize();
		Vec3 yAxis = zAxis.cross(xAxis);

		Mat4 result = identityMat4();

		result[0][0] = xAxis.x;
		result[0][1] = xAxis.y;
		result[0][2] = xAxis.z;

		result[1][0] = yAxis.x;
		result[1][1] = yAxis.y;
		result[1][2] = yAxis.z;

		result[2][0] = zAxis.x;
		result[2][1] = zAxis.y;
		result[2][2] = zAxis.z;

		result[0][3] = -(xAxis.dot(eye));
		result[1][3] = -(yAxis.dot(eye));
		result[2][3] = -(zAxis.dot(eye));

		return result;
	}

	Mat4 MatrixFactory::rotationByAxis(Quaternion q)
	{
		Mat4 result = identityMat4();

		float ai = q.w * q.v.x;
		float aj = q.w * q.v.y;
		float ak = q.w * q.v.z;

		float ii = q.v.x * q.v.x;
		float ij = q.v.x * q.v.y;
		float ik = q.v.x * q.v.z;

		float jj = q.v.y * q.v.y;
		float jk = q.v.y * q.v.z;

		float kk = q.v.z * q.v.z;

		result[0][0] = 1 - 2 * jj - 2 * kk;
		result[0][1] = 2 * ij - 2 * ak;
		result[0][2] = 2 * ik + 2 * aj;

		result[1][0] = 2 * ij + 2 * ak;
		result[1][1] = 1 - 2 * ii - 2 * kk;
		result[1][2] = 2 * jk - 2 * ai;

		result[2][0] = 2 * ik - 2 * aj;
		result[2][1] = 2 * jk + 2 * ai;
		result[2][2] = 1 - 2 * ii - 2 * jj;

		return result;
	}

}
