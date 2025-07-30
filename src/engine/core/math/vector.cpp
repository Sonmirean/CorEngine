
#include "vector.hpp"

float& CorE::math::Vec4::x()
{
	return val[0];
}

float& CorE::math::Vec4::y()
{
	return val[1];
}

float& CorE::math::Vec4::z()
{
	return val[2];
}

float& CorE::math::Vec4::w()
{
	return val[3];
}


float& CorE::math::Vec4::operator[](int dim)
{
	return val[dim];
}






float& CorE::math::Vec3::x()
{
	return val[0];
}

float& CorE::math::Vec3::y()
{
	return val[1];
}

float& CorE::math::Vec3::z()
{
	return val[2];
}


float& CorE::math::Vec3::operator[](int dim)
{
	return val[dim];
}