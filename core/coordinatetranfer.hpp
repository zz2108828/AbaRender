#pragma once
#include <model.hpp>

struct Tranfer{
	float x_zoom;
	float y_zoom;
	float z_zoom;
	float zoom;
	float offset_x;
	float offset_y;
};

class Model2ScreenTranfer {
private:
	const Model* const modelptr_;
	Vec3f orgin_;
	float z_zoom_;
	float zoom_;
	float offset_x_;
	float offset_y_;
public:
	Model2ScreenTranfer(const Model& model, uint32_t width, uint32_t height);
	Vec3i model2screen(const Vec3f& model_coord);
};