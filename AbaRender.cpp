// AbaRender.cpp: 定义应用程序的入口点。
#include <iostream>
#include <math.h>
#include <algorithm>
#include "tgaimage.hpp"
#include <model.hpp>
#include <vector>
#include <time.h>
#include <geometry.cpp>
#include <zbuffer.hpp>
#include <coordinatetranfer.hpp>

using namespace aba;
const Color WHITE = Color(255, 255, 255, 255);
const Color RED = Color(255, 0, 0, 255);
const Color GREEN = Color(0, 255, 0, 255);

//Bresenham's line algorithm
std::vector<Vec3i> getLinePoints(Vec3i v0, Vec3i v1,bool is_Transpose = true, ZBuffer* z_buffer = NULL) {
	std::vector<Vec3i> points;// ascending order by the X - coordinate
	if (v0.x == v1.x && v0.y == v1.y) {
		points.push_back(v1);
		return points;
	}
	int x0 = v0.x, y0 = v0.y, x1 = v1.x, y1 = v1.y, z0 = v0.z, z1 = v1.z;
	bool transpose = false;
	// transpose image make disX > disY
	if (is_Transpose && abs(x1 - x0) < abs(y1 - y0)) {
		swap(x0, y0);
		swap(x1, y1);
		transpose = true;
	}
	// make it left to right
	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}
	int dx = x1 - x0, dy = abs(y1 - y0), dz = abs(z1 - z0);
	int delta_erro_y = 2 * dy;
	int delta_erro_z = 2 * dz;
	int error_y = 0;
	int error_z = 0;
	float y_float = static_cast<float>(y0); // isTranspose
	float z_float = static_cast<float>(z0);
	int y = y0, z = z0;
	for (int x = x0; x <= x1; x++) {
		error_y += delta_erro_y;
		if (z_buffer != NULL) { // z_buffer
			error_z += delta_erro_z;
			if (error_z > dz) {
				float delata = (dx == 0 ? 1 : (dz * 1.0f) / (dx * 1.0f));
				z_float += (z0 < z1 ? delata : -delata);
				error_z -= 2 * dx;
			}
		}
		z = static_cast<int>(z_float);
		if (transpose) {
			points.push_back(Vec3i(y, x, z));
		}
		else { // isTranspose
			if (!is_Transpose && dx < dy) y = static_cast<int>(y_float);// isTranspose
			points.push_back(Vec3i(x, y, z));
		}
		if (error_y > dx) {
			if (dx >= dy) {
				y += (y0 < y1 ? 1 : -1);
			}
			else if(!is_Transpose) { // isTranspose
				float delata = (dx == 0 ? 1 : (dy*1.0f) / (dx*1.0f));
				y_float += (y0 < y1 ? delata : -delata);
			}
			error_y -= 2 * dx;
		}
	}
	return points;
}

void drawLine(const std::vector<Vec3i>& points, TGAImage& image, const Color color, ZBuffer* z_buffer = NULL) {
	for (auto point : points) {
		int x = point.x, y = point.y, z = point.z;
		if (z_buffer != NULL) {
			if (!z_buffer->isCulling(x, y, z)) {
				z_buffer->set(x, y, z);
				image.set(x, y, color);
			}
		}
		else
		{
			image.set(x, y, color);
		}
	}
}

void drawLine(Vec3i v0, Vec3i v1, TGAImage& image, const Color color,bool is_Transpose = false, ZBuffer* z_buffer = NULL) {
	drawLine(getLinePoints(v0, v1, is_Transpose), image, color, z_buffer);
}


//use Edge-walking,Edge equations not implement.
void drawTriangle(Vec3i v0, Vec3i v1, Vec3i v2, TGAImage& image, const Color& color,ZBuffer* z_buffer = NULL) {
	if (v0.y == v1.y && v1.y == v2.y) return;
	// v0,v1,v2 are sorted in ascending order by the X-coordinate (left to right)
	if (v0.x > v1.x) swap(v0, v1);
	if (v0.x > v2.x) swap(v0, v2);
	if (v1.x > v2.x) swap(v1, v2);
	// v0->v2 points
	std::vector<Vec3i> points = getLinePoints(v0, v2, false, z_buffer);
	int start = 0;
	// left part v0->v1
	if (v0.x != v1.x) {
		std::vector<Vec3i> left_points = getLinePoints(v0, v1, false, z_buffer);
		int i = 0;
		while (i < left_points.size()) {
			drawLine(points[start++], left_points[i++], image, color, true, z_buffer);
		}
		start -= 1;
	}
	// right part v1->v2
	if (v1.x != v2.x) {
		std::vector<Vec3i> right_points = getLinePoints(v1, v2, false, z_buffer);
		int i = 0;
		while (i < right_points.size()) {
			drawLine(points[start++], right_points[i++], image, color, true, z_buffer);
		}
	}
}

void triangleTest(TGAImage& image) {
	int x_dis = 330, y_dis = 330;
	int x_init = 20, y_init = 20;
	std::vector<std::vector<Vec3i> > triangles{
		//{Vec3i(x_init,300),Vec3i(300,300),Vec3i(300,x_init)},
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
		cout << "Test Traingle[" << i << "]" << endl;
		drawTriangle(triangles[i][0], triangles[i][1], triangles[i][2], image, WHITE);
	}
}

void drawWireFrame(TGAImage &image,const char* filename) {
	Model model(filename);
	uint32_t width = image.getWidth(), height = image.getHeight();
	Model2ScreenTranfer tranfer(model, height, width);
	printf("faces:{%llu},verts:{%llu}\n", model.getFacesNum(), model.getVertsNum());
	uint32_t image_size = width*height;
	ZBuffer zbuffer(width,height);
	Vec3f light_dir(0, 0, -1);
	// draw the wireframe faces.
	for (int i = 0; i < model.getFacesNum(); i++) {
		std::vector<int> faces = model.getFaces(i);
		Vec3i screen_coord[3];
		Vec3f model_coords[3];
		for (int j = 0; j < 3; j++) {
			model_coords[j] = model.getVert(faces[j]);
			screen_coord[j] = tranfer.model2screen(model_coords[j]);
		}
		Vec3f normal = cross(model_coords[2] - model_coords[0], model_coords[1] - model_coords[0]);
		normal.normalize();
		float intensity = dot(normal,light_dir);
		if (intensity > 0) {
			uint8_t rgb = intensity * 255;
			Color gray = Color(rgb, rgb, rgb);
			drawTriangle(screen_coord[0], screen_coord[1], screen_coord[2], image, gray);
		}
	}
}

int main()
{
	TGAImage image(1000, 1000, RGB);
	drawWireFrame(image, "obj/clay_vase.obj");
	image.writeTGAFile("out.tga");
	return 0;
}