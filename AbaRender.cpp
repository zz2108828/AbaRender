// AbaRender.cpp: 定义应用程序的入口点。
#include <iostream>
#include <math.h>
#include <algorithm>
#include <tgaimage.h>
#include <model.h>
#include <vector>
#include <time.h>
#include <geometry.h>
#include <zbuffer.h>
#include <Shader.h>
#include <CoordinateTranform.h>

using namespace aba;

//void triangleTest(TGAImage& image) {
//	int x_dis = 330, y_dis = 330;
//	int x_init = 20, y_init = 20;
//	std::vector<std::vector<Vertex> > triangles{
//		{Vec3i(x_init,300),Vec3i(300,300),Vec3i(300,x_init)},
//		{Vec3i(x_init + x_dis,300),Vec3i(300 + x_dis,150),Vec3i(x_init + x_dis,y_init)},
//		{Vec3i(150 + x_dis * 2,300),Vec3i(300 + x_dis * 2,150),Vec3i(x_init + x_dis * 2, y_init)},
//		{Vec3i(x_init,y_init + y_dis),Vec3i(300,300 + y_dis),Vec3i(300,y_init + y_dis)},
//		{Vec3i(300 + x_dis,300 + y_dis),Vec3i(300 + x_dis,y_init + y_dis),Vec3i(x_init + x_dis,150 + y_dis)},
//		{Vec3i(x_init + x_dis * 2,150 + y_dis),Vec3i(120 + x_dis * 2,300 + y_dis),Vec3i(300 + x_dis * 2,y_init + y_dis)},
//		{Vec3i(x_init,60 + 2 * y_dis),Vec3i(150,300 + 2 * y_dis),Vec3i(300,80 + 2 * y_dis)},
//		{Vec3i(300,80 + 2 * y_dis),Vec3i(250 + x_dis,y_init + 2 * y_dis),Vec3i(300 + x_dis,300 + 2 * y_dis)},
//		{Vec3i(300 + x_dis,300 + 2 * y_dis),Vec3i(270 + x_dis * 2,280 + 2 * y_dis),Vec3i(100 + x_dis * 2,y_init + 2 * y_dis)},
//	};
//	for (int i = 0; i < triangles.size(); i++) {
//		std::cout << "Test Traingle[" << i << "]" << std::endl;
//		drawTriangle(triangles[i][0], triangles[i][1], triangles[i][2], image, WHITE);
//	}
//	 	int x_dis = 330, y_dis = 330;
//	int x_init = 20, y_init = 20;
//	const int test_num = 9;
//	Vertex triangles[test_num][3] = {
//		{Vertex(x_init,300,0.5),Vertex(300,300,0.6),Vertex(300,x_init,0.9)},
//		{Vertex(x_init + x_dis,300,0.4),Vertex(300 + x_dis,150,1),Vertex(x_init + x_dis,y_init,0)},
//		{Vertex(150 + x_dis * 2,300,1),Vertex(300 + x_dis * 2,150,0),Vertex(x_init + x_dis * 2, y_init,1)},
//
//		{Vertex(x_init,y_init + y_dis,0),Vertex(300,300 + y_dis,1),Vertex(300,y_init + y_dis,0)},
//		{Vertex(300 + x_dis,300 + y_dis,0),Vertex(300 + x_dis,y_init + y_dis,0),Vertex(x_init + x_dis,150 + y_dis,1)},
//		{Vertex(x_init + x_dis * 2,150 + y_dis,0),Vertex(120 + x_dis * 2,300 + y_dis,1),Vertex(300 + x_dis * 2,y_init + y_dis,1)},
//		{ Vertex(x_init,60 + 2 * y_dis,0),Vertex(150,300 + 2 * y_dis,1),Vertex(300,80 + 2 * y_dis,1) },
//		{ Vertex(300,80 + 2 * y_dis,1),Vertex(250 + x_dis,y_init + 2 * y_dis,1),Vertex(300 + x_dis,300 + 2 * y_dis,0) },
//		{ Vertex(300 + x_dis,300 + 2 * y_dis,1),Vertex(270 + x_dis * 2,280 + 2 * y_dis,1),Vertex(100 + x_dis * 2,y_init + 2 * y_dis,0) },
//	};
//	for (int i = 0; i < test_num; i++) {
//		std::cout << "Test Traingle[" << i << "]" << std::endl;
//		drawTriangle(triangles[i], shader, image, &zbuffer);
//	}
//}

void drawTheModel(aba::TGAImage &image,const char* filename) {
	Model model(filename);
	uint32_t width = image.getWidth(), height = image.getHeight();
	printf("faces:{%llu},verts:{%llu}\n", model.getFacesNum(), model.getVertsNum());
	uint32_t image_size = width*height;
	ZBuffer zbuffer(width,height);
	Vec3f light_dir(1, 1, 1);
	Shader shader(model);
	shader.setLightDir(light_dir);
	for (int i = 0; i < model.getFacesNum(); i++) {
		Vertex screen_coord[3];
		for (int j = 0; j < 3; j++) {
			screen_coord[j] = shader.vetexShade(i, j);
		}
		drawTriangle(screen_coord, shader, image, &zbuffer);
	}
}

int main()
{
	TGAImage image(1024, 1024, RGB);
	drawTheModel(image, "obj/clay_vase.obj");
	//drawTheModel(image, "obj/african_head.obj");
	//image.readTGAFile("out.tga");
	//Color c2 = image.get(578, 776);
	//Color c = image.get(578, 247);
	image.writeTGAFile("out.tga");
	return 0;
}