#pragma once

#include <glm/glm.hpp>
#include "Types.h"


struct Vertex
{
	float coord[3];
	float normal[3];
	float tex[2];
};


struct Model
{
	vec<Vertex> vertices;

	vec <int[3][3]> faces;

};

// Textured model with a transformation matrix.
struct	Object3D : Model
{

};

// Textured model with a set of bones.
// Practically ready to use. (no lol)
struct Creature : Object3D
{

};