#include "CoordinateTranform.h"
#include <algorithm>
#include <model.h>

namespace aba {

Model2ScreenTranfer::Model2ScreenTranfer(const Model& model, uint32_t width, uint32_t height) 
	: modelptr_(&model)
{
	Boundary boundary = modelptr_->getBoundary();
	Vec3f span = boundary.span_;
	Vec3f center = boundary.center_;
	orgin_ = boundary.origin_;
	float x_zoom = ((width * 0.8f) / span.x);
	float y_zoom = ((height * 0.8f) / span.y);
	z_zoom_ = 1000000.0f / span.z;
	zoom_ = std::min(x_zoom, y_zoom);
	// [-5,5] -> [0,10]
	offset_x_ = width / 2.0f - ((center.x - orgin_.x) * zoom_);
	offset_y_ = height / 2.0f - ((center.y - orgin_.y) * zoom_);
}

Vec3f Model2ScreenTranfer::model2screen(const Vec3f& model_coord)
{
	Vec3f screen_coord;
	float x = ((model_coord.x - orgin_.x) * zoom_ + offset_x_);
	float y = ((model_coord.y - orgin_.y) * zoom_ + offset_y_);
	float z = ((model_coord.z - orgin_.z) * zoom_);
	//cout << model_coord.z << " ->" << z << "\n";
	screen_coord.x = x;
	screen_coord.y = y;
	screen_coord.z = z;
	return screen_coord;
}

Matrix viewportTransform(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	return Matrix();
}

Matrix perspectiveTransform(float coff)
{
	return Matrix();
}

Matrix cameraTransform(Vec3f camera, Vec3f center, Vec3f up)
{
	Matrix ModelView;

	return ModelView;
}

Matrix modelTransform()
{
	return Matrix();
}

}

