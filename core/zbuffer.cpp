#include "zbuffer.h"
#include <limits>
#include <iostream>
namespace aba {

uint32_t ZBuffer::getIndex(uint32_t x, uint32_t y)
{
	uint32_t idx = (width_ - 1 - y)* height_ + x;
	return idx;
}

ZBuffer::ZBuffer(uint32_t width, uint32_t height) :width_(width), height_(height), z_buffer_(NULL)
{
	uint32_t size = width * height;
	z_buffer_ = new float[size];
	for (uint32_t i = 0; i < size; i++) {
		z_buffer_[i] = std::numeric_limits<float>::lowest();
	}
}

float ZBuffer::get(uint32_t x, uint32_t y)
{
	using namespace std;
	return z_buffer_[getIndex(x,y)];
}

void ZBuffer::set(uint32_t x, uint32_t y,int z)
{
	if (!isCulling(x,y,z)) {
		z_buffer_[getIndex(x, y)] = z;
	}
}

bool ZBuffer::isCulling(uint32_t x, uint32_t y, int z)
{
	float t = get(x, y);
	if (t < z) {
		return false;
	}
	return true;
}

uint32_t ZBuffer::size()
{
	return width_ * height_;
}

void ZBuffer::clear()
{
	for (int i = 0; i < size(); i++) {
		z_buffer_[i] = std::numeric_limits<float>::lowest();
	}
}

}// aba