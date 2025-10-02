
#pragma once

#include "CorE/math/vector.hpp"

namespace CorE
{
	namespace math
	{
		// TODO - make a Mat class with ability to assign size upon creation.

		struct Mat4x4
		{

			float val[4][4];

			Mat4x4();

			// Constructs a projection matrix from given values.
			static Mat4x4& projection(float fov, float aspect_ratio, float z_near, float z_far);
			// Constructs a transformation matrix from given values.
			static Mat4x4& transformation(Vec3 scale, Vec3 rotate, Vec3 displace);
			// Constructs an identity matrix
			static Mat4x4& identity();

			Mat4x4& multiplyNaive(Mat4x4& rhs) const;
			Mat4x4& divideNaive(Mat4x4& rhs) const;

			Mat4x4& T(); // transpose
			//TODO - Mat4x4& invert();

			float* operator[](int column);

			// Matrix operators
			Mat4x4& operator*(Mat4x4& rhs);
			Mat4x4& operator+(Mat4x4& rhs);
			Mat4x4& operator-(Mat4x4& rhs);

			// Scalar operators
			Mat4x4& operator*(float& rhs);
			Mat4x4& operator+(float& rhs);
			Mat4x4& operator-(float& rhs);

			// Vector operators
			Vec4& operator*(Vec4& rhs);

		};
	}
}