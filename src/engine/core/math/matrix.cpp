
#include "matrix.hpp"

CorE::math::Mat4x4::Mat4x4()
{
	for (uint8_t i = 0; i < 4; i++)
	{
		for (uint8_t j = 0; i < 4; i++)
		{
			//val[i][j] = 0;
		}
	}
}

float* CorE::math::Mat4x4::operator[](int column)
{
	//return val[column];
	return 0;//
}


CorE::math::Mat4x4& CorE::math::Mat4x4::projection(float fov, float aspect_ratio, float z_near, float z_far)
{
	Mat4x4 result;

	// TODO - do a research about how projection actually works

	return result;
}

CorE::math::Mat4x4& CorE::math::Mat4x4::transformation(Vec3 scale, Vec3 rotate, Vec3 displace)
{
	Mat4x4 result;

	//result[1][1] = scale.x();
	//result[2][2] = scale.y();
	//result[3][3] = scale.z();
	

	return result;
}

CorE::math::Mat4x4& CorE::math::Mat4x4::identity()
{
	Mat4x4 result;

	for (int i = 0; i < 4; i++)
	{
		//result[i][i] = 1;
	}

	return result;
}

CorE::math::Mat4x4& CorE::math::Mat4x4::multiplyNaive(Mat4x4& rhs) const
{
	Mat4x4 result;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//result[i][j] += val[i][j] * rhs[i][j];
		}
	}
	return result;
}

CorE::math::Mat4x4& CorE::math::Mat4x4::divideNaive(Mat4x4& rhs) const
{
	Mat4x4 result;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//result[i][j] += val[i][j] * rhs[i][j];
		}
	}
	return result;
}

CorE::math::Mat4x4& CorE::math::Mat4x4::T()
{
	Mat4x4 result;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//result[i][j] = val[j][i];
		}
	}
	return result;
}


CorE::math::Mat4x4 &CorE::math::Mat4x4::operator*(Mat4x4& rhs)
{
	Mat4x4 result;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				//result[i][j] += val[i][k] * rhs[k][j];
			}
		}
	}
	return result;
}


CorE::math::Mat4x4& CorE::math::Mat4x4::operator+(Mat4x4& rhs)
{
	Mat4x4 result;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//result[i][j] = val[i][j] + rhs[i][j];
		}
	}
	return result;
}

CorE::math::Mat4x4& CorE::math::Mat4x4::operator-(Mat4x4& rhs)
{
	Mat4x4 result;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//result[i][j] = val[i][j] - rhs[i][j];
		}
	}
	return result;
}

CorE::math::Mat4x4& CorE::math::Mat4x4::operator*(float& rhs)
{
	Mat4x4 result;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//result[i][j] += val[i][j] * rhs;
		}
	}
	return result;
}


CorE::math::Mat4x4& CorE::math::Mat4x4::operator+(float& rhs)
{
	Mat4x4 result;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//result[i][j] += val[i][j] + rhs;
		}
	}
	return result;
}

CorE::math::Mat4x4& CorE::math::Mat4x4::operator-(float& rhs)
{
	Mat4x4 result;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//result[i][j] += val[i][j] - rhs;
		}
	}
	return result;
}


CorE::math::Vec4& CorE::math::Mat4x4::operator*(Vec4& rhs)
{
	Vec4 result;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//result[i] += rhs[j] * val[i][j];//questionable, may contain dumb mistake(s)
											//same can be said about the whole code
		}
	}
	return result;
}

/*
template<typename T>
Mat4x4& loopThrough(Mat4x4* mat, T& rhs, char op, bool naive_or_scalar, bool vec)
{
	Mat4x4 result;

	for (char i = 0; i < 4; i++)
	{
		for (char j = 0; j < 4; j++)
		{
			switch(op)
				case '*':
					if (naive_or_scalar)
					{
						result.val[i][j] += *mat.val[i][j] * rhs.val[i][j];
					}
					else
					{
						for (char k = 0; k < 4; k++)
						{
							if (vec)
								result.val[i][j] += *mat.val[i][k] * rhs[k];
							else
								result.val[i][j] += *mat.val[i][k] * rhs.val[k][j];
						}
					}
				case '+':
					if (naive_or_scalar)
					{
						result.val[i][j] += *mat.val[i][j] + rhs;
					}
					else if (vec)
					{
						result.val[i][j] += *mat.val[i][j] + rhs[i];
					}
					else
					{
						result.val[i][j] += *mat.val[i][j] + rhs.val[i][j];
					}
				case '-':
					if (naive_or_scalar)
					{
						result.val[i][j] += *mat.val[i][j] - rhs;
					}
					else if (vec)
					{
						result.val[i][j] += *mat.val[i][j] - rhs[i];
					}
					else
					{
						result.val[i][j] += *mat.val[i][j] - rhs.val[i][j];
					}
				default:
					result = nullptr;
		}
	}
	return result;
}
*/
