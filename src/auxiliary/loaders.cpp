
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "auxiliary/loaders.hpp"

static void processLine(std::vector<std::string> storage, size_t offset, char separation, std::string line)
{
	char slot = 0; // current slot of the storage
	for (size_t i = offset; i < line.length(); i++)
	{
		if (line[i] != separation)
		{
			storage[slot][i - offset] = (line[i]);
		}
		else
		{
			slot++; // if came to the separation sign, jumps to the next slot
		}
	}
}

Dim3::Model_3D loadModelOBJ(const char* file_path)
{
	Dim3::Model_3D model;

	std::ifstream stream;

	stream.open(static_cast<std::string>(file_path).append(".obj"));
	//std::cout << static_cast<std::string>(file_path).append(".obj") << std::endl;

	std::string line;
	while (std::getline(stream, line))
	{
		Dim3::Vertex_3D vertex;
		switch (line[0])
		{
		case ('v'):
			switch (line[1])
			{
			case (' '):
			{
				//v 1.000000 1.000000 -1.000000
				//^^ <- offset = 2
				vec<str> coord;
				processLine(coord, 2, ' ', line);
				vertex.coord[0] = std::stof(coord[0]);
				vertex.coord[1] = std::stof(coord[1]);
				vertex.coord[2] = std::stof(coord[2]);
			}
			case ('n'):
			{
				//vn -0.0000 1.0000 -0.0000
				//^^^ <- offset = 3
				vec<str> normal;
				processLine(normal, 3, ' ', line);
				vertex.normal[0] = std::stof(normal[0]);
				vertex.normal[1] = std::stof(normal[1]);
				vertex.normal[2] = std::stof(normal[2]);
			}
			case ('t'):
			{
				//vt 0.625000 0.500000
				//^^^ <- offset = 3
				vec<str> tex_coord;
				processLine(tex_coord, 3, ' ', line);
				vertex.tex_coord[0] = std::stof(tex_coord[0]);
				vertex.tex_coord[1] = std::stof(tex_coord[1]);
			}
			}
		case ('f'):
			//f 1/1/1 5/2/1 7/3/1
			//^^ <- offset = 2
			// non-triangle polygons are not supported.
			// TODO - add ear-clip algorithm
			vec<str> face;
			processLine(face, 3, ' ', line);
			for (size_t i = 0; i < 2; i++)
			{
				arr<int, 3> iface;
				std::string part = face[i];
				vec<str> val;
				processLine(val, 0, '/', part);

				iface[0] = std::stoi(val[0]);
				iface[1] = std::stoi(val[1]);
				iface[2] = std::stoi(val[2]);
				model.faces.push_back(iface);
			}
		
		}
	}
	return model;
}
