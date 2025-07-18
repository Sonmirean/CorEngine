
#include "vector.hpp"

float& Vec4::x()
{
	return val[0];
}

float& Vec4::y()
{
	return val[1];
}

float& Vec4::z()
{
	return val[2];
}

float& Vec4::w()
{
	return val[3];
}


float& Vec4::operator[](int dim)
{
	return val[dim];
}






float& Vec3::x()
{
	return val[0];
}

float& Vec3::y()
{
	return val[1];
}

float& Vec3::z()
{
	return val[2];
}


float& Vec3::operator[](int dim)
{
	return val[dim];
}