#include "model.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>

/**
ref: http://paulbourke.net/dataformats/obj/
**/
namespace aba{

void Model::load_texture(std::string filename, const char* suffix, TGAImage& img)
{
	std::string texfile(filename);
	size_t dot = texfile.find_last_of(".");
	if (dot != std::string::npos) {
		texfile = texfile.substr(0, dot) + std::string(suffix);
		std::cout << "Texture[" << texfile << "] Loading " << (img.readTGAFile(texfile.c_str()) ? "OK" : "ERROR") << std::endl;
	}
}

Model::Model() : boundary(NULL){}

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
		if (!line.compare(0,2,"v ")) { // vetex (x,y,z)
			iss >> trash;
			Vec3f v;
			for (int i = 0; i < 3; i++) {
				iss >> v.rows[i];
			}
			verts_.push_back(v);
		}
		else if (!line.compare(0, 3, "vt ")) { // texture (u,v,w)
			Vec3f uv;
			iss >> trash >> trash;
			for (int i = 0; i < 3; i++) {
				iss >> uv[i];
			}
			uv_.push_back(uv);
		}
		else if (!line.compare(0, 3, "vn ")) { // normal (x,y,z)
			Vec3f normal;
			iss >> trash >> trash;
			for (int i = 0; i < 3; i++) {
				iss >> normal[i];
			}
			normal_.push_back(normal);
		}
		else if (!line.compare(0, 2, "f ")) { // face (v/vt/vn v/vt/vn v/vt/vtn)
			std::vector<int> v;
			std::vector<int> vt;
			std::vector<int> vn;
			int tmp[3];
			iss >> trash;
			while (iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2]) {
				for (int i = 0; i < 3; i++) {
					tmp[i]--;// index should start from 0.
				}
				v.push_back(tmp[0]);
				vt.push_back(tmp[1]);
				vn.push_back(tmp[2]);
			}
			std::vector<std::vector<int> > face{ v,vt,vn };
			faces_.push_back(face);
		}
	}

	load_texture(filename, "_diffuse.tga", diffusemap_);
	load_texture(filename, "_normal_object.tga", normalmap_);
	load_texture(filename, "_specual.tga", specularmap_);

	float x_min = 1e9f, x_max = -1e9f;
	float y_min = 1e9f, y_max = -1e9f;
	float z_min = 1e9f, z_max = -1e9f;
	for (int i = 0; i < getVertsNum(); i++) {
		Vec3f vert = verts_[i];
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
	boundary = new Boundary(center, span, orgin);
	printf("load [%s] finish\n", filename);
	in.close();
}

std::vector<std::vector<int> > Model::getFaces(int id) const
{
	return faces_[id];
}

Vec3f Model::getVert(int iface, int ivertex) const
{
	return verts_[getFaces(iface)[0][ivertex]];
}

Vec3f Model::getVertNormal(int iface, int ivertex) const
{
	return normal_[getFaces(iface)[2][ivertex]];
}

Vec3f Model::getUv(int iface, int ivertex) const
{
	return uv_[getFaces(iface)[1][ivertex]];
}

Color Model::getDiffuseColor(Vec3f uvf) const
{
	Vec3i uv(uvf[0]* diffusemap_.getWidth(), uvf[1]* diffusemap_.getHeight());
	return diffusemap_.get(uv[0],uv[1]);
}

Vec3f Model::getNormalMap(Vec3f uvf) const
{
	Vec3i uv(uvf[0] * normalmap_.getWidth(), uvf[1] * normalmap_.getHeight());
	Color rgb = normalmap_.get(uv[0], uv[1]);
	Vec3f res;
	// rgb[0,255] to x,y,z
	for (int i = 0; i < 3; i++) {
		res[2-i] = (rgb[i]*1.0f/255.0f) * 2.0 - 1.0;
		//res[i] = (rgb[i] * 1.0f / 255.0f) ;
	}
	return res;
}

float Model::getSpecular(Vec3f uvf) const
{
	Vec3i uv(uvf[0] * specularmap_.getWidth(), uvf[1] * specularmap_.getHeight());
	return static_cast<float>(specularmap_.get(uv[0], uv[1])[0]);
}

size_t Model::getFacesNum() const
{
	return faces_.size();
}

size_t Model::getVertsNum() const
{
	return verts_.size();
}

Boundary Model::getBoundary() const
{
	return *boundary;
}

}// aba
