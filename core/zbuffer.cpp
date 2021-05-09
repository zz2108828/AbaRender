#include "zbuffer.hpp"
#include <limits>
#include <iostream>

uint32_t ZBuffer::getIndex(uint32_t x, uint32_t y)
{
	uint32_t idx = (width_ - 1 - y)* width_ + x;
	return idx;
}

ZBuffer::ZBuffer(uint32_t width, uint32_t height) :width_(width), height_(height), z_buffer_(NULL)
{
	uint32_t size = width * height;
	z_buffer_ = new float[size];
	for (uint32_t i = 0; i < size; i++) {
		z_buffer_[i] = std::numeric_limits<float>::min();
	}
}

int ZBuffer::get(uint32_t x, uint32_t y)
{
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
	if (get(x, y) < z) {
		return false;
	}
	return true;
}

uint32_t ZBuffer::size()
{
	return width_ * height_;
}
