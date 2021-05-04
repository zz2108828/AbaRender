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

void drawLine(Vec3i v1, Vec3i v2,TGAImage &image, const Color color) {
	int x0 = v1.x, y0 = v1.y, x1 = v2.x, y1 = v2.y;
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

void drawTriangle(Vec3i v1, Vec3i v2, Vec3i v3, TGAImage& image, const Color& color) {
	drawLine(v1, v2, image, color);
	drawLine(v2, v3, image, color);
	drawLine(v3, v1, image, color);
}

void drawWireFrame(TGAImage &image,string filename) {
	Model model("obj/stone_lantern_low.obj");
	Boundary boundary = model.getBoundary();
	Vec3f span = boundary.span_;
	Vec3f center = boundary.center_;
	Vec3f orgin = boundary.origin_;
	int width = image.getWidth(), height = image.getHeight();
	float x_zoom = ((width * 0.8f) / span.x);
	float y_zoom = ((height * 0.8f) / span.y);
	float zoom = min(x_zoom, y_zoom);
	float offset_x = width / 2.0f - ((center.x - orgin.x) * zoom);
	float offset_y = height / 2.0f - ((center.y - orgin.y) * zoom);
	printf("faces:{%llu},verts:{%llu}", model.getFacesNum(), model.getVertsNum());
	// draw the wireframe faces.
	for (int i = 0; i < model.getFacesNum(); i++) {
		std::vector<int> faces = model.getFaces(i);
		for (int j = 0; j < 3; j++) {
			Vec3f v0 = model.getVert(faces[j]);
			Vec3f v1 = model.getVert(faces[(j + 1) % 3]);
			// [-5,5] -> [0,10]
			int x0 = static_cast<int>((v0.x - orgin.x) * zoom + offset_x);
			int y0 = static_cast<int>((v0.y - orgin.y) * zoom + offset_y);
			int x1 = static_cast<int>((v1.x - orgin.x) * zoom + offset_x);
			int y1 = static_cast<int>((v1.y - orgin.y) * zoom + offset_y);
			drawLine(Vec3i(x0, y0), Vec3i(x1, y1), image, WHITE);
		}
	}
}

int main()
{
	TGAImage image(1000, 1000, RGB);
	std::vector<Vec3i> v = { Vec3i(100,100),Vec3i(400,300),Vec3i(50,400) };
	int x_dis = 330, y_dis = 330;
	int x_init = 20, y_init = 20;
	std::vector<std::vector<Vec3i> > triangles{
		{Vec3i(x_init,300),Vec3i(300,300),Vec3i(300,x_init)},
		{Vec3i(x_init + x_dis,300),Vec3i(300 + x_dis,150),Vec3i(x_init + x_dis,y_init)},
		{Vec3i(150 + x_dis * 2,300),Vec3i(300 + x_dis * 2,150),Vec3i(x_init + x_dis * 2, y_init)},

		{Vec3i(x_init,y_init + y_dis),Vec3i(300,300 + y_dis),Vec3i(300,y_init + y_dis)},
		{Vec3i(300 + x_dis,300 + y_dis),Vec3i(300 + x_dis,y_init + y_dis),Vec3i(x_init + x_dis,150 + y_dis)},
		{Vec3i(x_init + x_dis * 2,150 + y_dis),Vec3i(120 + x_dis * 2,300 + y_dis),Vec3i(300 + x_dis * 2,y_init + y_dis)},

		{Vec3i(x_init,60 + 2 * y_dis),Vec3i(150,300 + 2 * y_dis),Vec3i(300,80 + 2 * y_dis)},
		{Vec3i(300,80 + 2 * y_dis),Vec3i(250 + x_dis,y_init + 2 * y_dis),Vec3i(300 + x_dis,300 + 2 * y_dis)},
		{Vec3i(300 + x_dis,300 + 2 * y_dis),Vec3i(270 + x_dis * 2,280 + 2 * y_dis),Vec3i(100 + x_dis * 2,y_init + 2 * y_dis)},
	};
	for (int i = 0; i < triangles.size(); i++) {
		drawTriangle(triangles[i][0], triangles[i][1], triangles[i][2], image, WHITE);
	}
	//drawWireFrame(image, "obj/stone_lantern_low.obj");
	image.writeTGAFile("out.tga");
	return 0;
}