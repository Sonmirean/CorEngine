
#pragma once

#include "auxiliary/short_type.hpp"


namespace Dim2
{
	struct Texture_2D
	{

	};
}

namespace Dim3
{
	// 3D model vertex.
	struct Vertex_3D
	{
		arr<float, 3> coord{};
		arr<float, 3> normal{};
		arr<float, 2> tex_coord{};
	};

	// 3-dimensional triangulated model.
	struct Model_3D
	{
		vec<Vertex_3D> vertices;
		vec<arr<int, 3>> faces;

		vec<Dim2::Texture_2D> textures;
	};
	

	
	// 3-dimensional texture.
	struct Texture_3D
	{

	};

}

