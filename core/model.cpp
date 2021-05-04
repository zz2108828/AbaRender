#include "model.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>

/**
ref: http://paulbourke.net/dataformats/obj/
**/

Model::Model()
{

}

Model::Model(const char* filename)
{
	std::ifstream in;
	in.open(filename);
	if (in.fail()) {
		throw "Can't open obj";
	}
	std::string line;
	while (!in.eof()) {
		getline(in, line);
		std::istringstream iss(line);
		char trash;
		if (!line.compare(0,2,"v ")) {
			iss >> trash;
			Vec3f v;
			for (int i = 0; i < 3; i++) {
				iss >> v.rows[i];
			}
			verts_.push_back(v);
		}
		else if (!line.compare(0, 2, "f ")) {
			std::vector<int> f;
			int v, vt, vn;
			iss >> trash;
			while (iss >> v >> trash >> vt >> trash >> vn) {
				v -= 1;// vertex index start from 1.
				f.push_back(v);
			}
			faces_.push_back(f);
		}
	}
	printf("load [%s] finish\n", filename);
	in.close();
}

std::vector<int> Model::getFaces(int id)
{
	return faces_[id];
}

Vec3f Model::getVert(int id)
{
	return verts_[id];
}

size_t Model::getFacesNum()
{
	return faces_.size();
}

size_t Model::getVertsNum()
{
	return verts_.size();
}

Boundary Model::getBoundary()
{
	float x_min = 1e9f, x_max = -1e9f;
	float y_min = 1e9f, y_max = -1e9f;
	float z_min = 1e9f, z_max = -1e9f;
	for (int i = 0; i < getVertsNum(); i++) {
		Vec3f vert = getVert(i);
		x_min = std::min(vert.x, x_min);
		y_min = std::min(vert.y, y_min);
		z_min = std::min(vert.z, z_min);
		x_max = std::max(vert.x, x_max);
		y_max = std::max(vert.y, y_max);
		z_max = std::max(vert.z, z_max);
	}
	Vec3f center((x_max + x_min) / 2.0f, (y_max + y_min) / 2.0f, (z_max + z_min) / 2.0f);
	Vec3f span(x_max - x_min, y_max - y_min, z_max - z_min);
	Vec3f orgin(x_min, y_min, z_min);
	return Boundary(center, span, orgin);
}
