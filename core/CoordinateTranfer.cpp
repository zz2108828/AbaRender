#include "coordinatetranfer.hpp"
#include <algorithm>

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

Vec3i Model2ScreenTranfer::model2screen(const Vec3f& model_coord)
{
	Vec3i screen_coord;
	int x = static_cast<int>((model_coord.x - orgin_.x) * zoom_ + offset_x_);
	int y = static_cast<int>((model_coord.y - orgin_.y) * zoom_ + offset_y_);
	int z = static_cast<int>((model_coord.z - orgin_.z) * z_zoom_);
	//cout << model_coord.z << " ->" << z << "\n";
	screen_coord.x = x;
	screen_coord.y = y;
	screen_coord.z = z;
	return screen_coord;
}
