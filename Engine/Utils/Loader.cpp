
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Loader.h"

static void processLine(char** storage, size_t offset, char separation, std::string line)
{
	// current dimension of storage
	char dim = 0;
	for (size_t i = offset; i < line.length(); i++)
	{
		if (line[i] != separation)
		{
			storage[dim][i - offset] = (line[i]);
		}
		else
		{
			// if came to the separation sign, comes to the next dimension
			dim++;
		}
	}
}

Model loadModelOBJ(const char* file_path)
{
	Model model;

	std::ifstream stream;

	stream.open(static_cast<std::string>(file_path).append(".obj"));
	std::cout << static_cast<std::string>(file_path).append(".obj") << std::endl;

	std::string line;
	while (std::getline(stream, line))
	{
		Vertex vertex;
		switch (line[0])
		{
		case ('v'):
			switch (line[1])
			{
			case (' '):
				//v 1.000000 1.000000 -1.000000
				char* coord[3];
				processLine(coord, 2, ' ', line);
				vertex.coord[0] = std::stof(coord[0]);
				vertex.coord[1] = std::stof(coord[1]);
				vertex.coord[2] = std::stof(coord[2]);
			case ('n'):
				//vn -0.0000 1.0000 -0.0000
				char* normal[3];
				processLine(normal, 3, ' ', line);
				vertex.normal[0] = std::stof(normal[0]);
				vertex.normal[1] = std::stof(normal[1]);
				vertex.normal[2] = std::stof(normal[2]);
			case ('t'):
				//vt 0.625000 0.500000
				char* tex[2];
				processLine(tex, 3, ' ', line);
				vertex.tex[0] = std::stof(tex[0]);
				vertex.tex[1] = std::stof(tex[1]);
			}
		case ('f'):
			//f 1/1/1 5/2/1 7/3/1
			// non-triangle polygons are not supported.
			Face result;
			char* face[3];
			processLine(face, 3, ' ', line);
			for (size_t i = 0; i < 2; i++)
			{
				std::string part = face[i];
				char* val[3];
				processLine(val, 0, '/', part);

				result.indices[i][0] = std::stoi(val[0]);
				result.indices[i][1] = std::stoi(val[1]);
				result.indices[i][2] = std::stoi(val[2]);
			}
			
			model.faces.push_back(result);
		}
	}
	return model;
}