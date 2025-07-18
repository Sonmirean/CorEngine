
#pragma once

// TODO - make a Vec class with ability to assign size upon creation.

// 4-dimensional signed float vector suitable for doing math on it.
struct Vec4
{
	float val[4];

	float& x();
	float& y();
	float& z();
	float& w();

	float& operator[](int dim);
};

struct Vec3
{
	float val[3];

	float& x();
	float& y();
	float& z();

	float& operator[](int dim);
};