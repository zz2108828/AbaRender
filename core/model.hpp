#pragma once
#include <string>
#include <vector>
#include "geometry.hpp"
using namespace std;

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
	std::vector<std::vector<int> > faces_;
	Boundary* boundary;
public:
	Model();
	Model(const char* filename);
	std::vector<int> getFaces(int id) const;
	Vec3f getVert(int id) const;
	size_t getFacesNum() const;
	size_t getVertsNum() const;
	Boundary getBoundary() const;
};