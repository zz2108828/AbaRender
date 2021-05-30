#pragma once
#include <cstdint>
namespace aba {

class ZBuffer{
private:
	uint32_t width_;
	uint32_t height_;
	float* z_buffer_;
	uint32_t getIndex(uint32_t x, uint32_t y);
public:
	ZBuffer(uint32_t width, uint32_t height);
	float get(uint32_t x, uint32_t y);
	void set(uint32_t x, uint32_t y, int v);
	bool isCulling(uint32_t x, uint32_t y, int v);
	uint32_t size();
	void clear();
};

}