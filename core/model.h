#pragma once
#include <string>
#include <vector>
#include "geometry.h"
#include <tgaimage.h>

namespace aba {

struct Boundary{
	Vec3f center_;
	Vec3f span_;
	Vec3f origin_;
	Boundary(const Vec3f& center, const Vec3f& span, const Vec3f& origin):
		center_(center), span_(span), origin_(origin) {

	}
};

class Model {
private:
	std::vector<Vec3f> verts_;//¶¥µã
	std::vector<std::vector<std::vector<int> > > faces_;
	std::vector<Vec3f> normal_;
	std::vector<Vec3f> uv_;
	TGAImage* diffusemap_;
	TGAImage* normalmap_;
	TGAImage* specularmap_;
	Boundary* boundary;
	void load_texture(std::string filename, const char* suffix, TGAImage*& img);
public:
	Model();
	Model(const char* filename);
	std::vector<std::vector<int> > getFaces(int id) const;
	Vec3f getVert(int iface,int ivertex) const;
	Vec3f getVertNormal(int iface, int ivertex) const;
	Vec3f getUv(int iface, int ivertex) const;
	Color getDiffuseColor(Vec3f uvf) const;
	Vec3f getNormalMap(Vec3f uvf) const;
	float Model::getSpecular(Vec3f uvf) const;
	size_t getFacesNum() const;
	size_t getVertsNum() const;
	Boundary getBoundary() const;
	bool hasNormalMap() const;
};

}
