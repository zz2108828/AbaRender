// AbaRender.cpp: 定义应用程序的入口点。
#include "tgaimage.hpp"
#include <iostream>
using namespace aba;

const Color WHITE = Color(255, 255, 255, 255);
const Color RED = Color(255, 0, 0, 255);

int main()
{
	TGAImage image(100,100,RGB);
	image.set(41, 52, RED);
	image.flipVertically();
	image.writeTGAFile("out.tga");
	return 0;
}