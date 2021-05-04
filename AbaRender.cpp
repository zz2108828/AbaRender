// AbaRender.cpp: 定义应用程序的入口点。
#include <iostream>
#include <math.h>
#include <algorithm>
#include "tgaimage.hpp"
#include <model.hpp>
#include <vector>

using namespace aba;
const Color WHITE = Color(255, 255, 255, 255);
const Color RED = Color(255, 0, 0, 255);

void line(int x0,int y0,int x1,int y1,TGAImage &image,Color color) {
	bool transpose = false;
	//transpose image make disX > disY
	if (abs(x1 - x0) < abs(y1 - y0)) {
		swap(x0, y0);
		swap(x1, y1);
		transpose = true;
	}
	//make it left to right
	if (x0 > x1) {
		swap(x0, x1); 
		swap(y0, y1);
	}
	int dx = x1 - x0, dy = abs(y1 - y0);
	int delta_erro = 2 * dy;
	int error = 0;
	for (int x = x0, y = y0; x < x1; x++) {		
		error += delta_erro;
		if (transpose) {
			image.set(y, x, color);
		}
		else {
			image.set(x, y, color);
		}
		if (error > dx) {
			y += y0 < y1 ? 1 : -1;
		 	error -= 2*dx;
		}
	}
}

int main()
{
	
	//Model model("obj/african_head.obj");
	Model model("obj/stone_lantern_low.obj");
	Boundary boundary = model.getBoundary();
	Vec3f span = boundary.span_;
	Vec3f center = boundary.center_;
	Vec3f orgin = boundary.origin_;
	TGAImage image(1000, 1000,RGB);
	int width = image.getWidth(), height = image.getHeight();
	float x_zoom = ((width * 0.8f) / span.x);
	float y_zoom = ((height * 0.8f) / span.y);
	float zoom = min(x_zoom, y_zoom);
	float offset_x = width / 2.0f - ((center.x - orgin.x) * zoom);
	float offset_y = height / 2.0f - ((center.y - orgin.y) * zoom);
	printf("faces:{%llu},verts:{%llu}", model.getFacesNum(), model.getVertsNum());
	for (int i = 0; i < model.getFacesNum(); i++) {
		std::vector<int> faces = model.getFaces(i);
		for (int j = 0; j < 3; j++) {
			Vec3f v0 = model.getVert(faces[j]);
			Vec3f v1 = model.getVert(faces[(j+1)%3]);
			//[-5,5] -> [0,10]
			int x0 = (v0.x - orgin.x) * zoom + offset_x;
			int y0 = (v0.y - orgin.y) * zoom + offset_y;
			int x1 = (v1.x - orgin.x) * zoom + offset_x;
			int y1 = (v1.y - orgin.y) * zoom + offset_y;
			line(x0, y0, x1, y1, image, WHITE);
		}
	}
	image.flipVertically();
	image.writeTGAFile("out.tga");
	return 0;
}