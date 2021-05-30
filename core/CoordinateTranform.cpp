#include "CoordinateTranform.h"
#include <algorithm>

namespace aba {

ModelTranfer::ModelTranfer(const Model& model, uint32_t width, uint32_t height) 
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


Vec3f ModelTranfer::model2screen(const Vec3f& model_coord) const
{
	Vec3f screen_coord;
	float x = ((model_coord.x - orgin_.x) * zoom_ + offset_x_);
	float y = ((model_coord.y - orgin_.y) * zoom_ + offset_y_);
	float z = ((model_coord.z - orgin_.z) * zoom_);
	screen_coord.x = x;
	screen_coord.y = y;
	screen_coord.z = z;
	return screen_coord;
}

//World Origin Pos (0,0,0)
Vec3f ModelTranfer::model2world(const Vec3f& model_coord) const
{
	return Vec3f();
}

Matrix viewportTransform(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	return Matrix();
}

Matrix perspectiveTransform(float fov, float aspect, float near, float far)
{
	Matrix m(4, 4);
	m = m.identity();
	fov = fov / 180.0 * PI;
	float tan2Fov = fabs(near) * tan(fov / 2.0f);
	float r = aspect * tan2Fov;

	m[0][0] = near / r;
	m[1][1] = near / tan2Fov;
	m[2][2] = (near + far) / (near - far);
	m[2][3] = 2 * near * far / (far - near);
	m[3][2] = 1;
	m[3][3] = 0;

	return m;
}



//Matrix perspectiveTransform(float coeff)
//{
//	Matrix m(4, 4);
//	m = m.identity();
//	m[3][2] = coeff;
//	return m;
//}

Matrix cameraTransform(Vec3f eye, Vec3f lookat, Vec3f up)
{
	Matrix m(4,4);
	m = m.identity();
	Vec3f forward = (eye - lookat).normalize();// ->z
	Vec3f right = cross(up, forward).normalize();// ->x
	Vec3f vup = cross(forward, right).normalize();// ->y
	for (int i = 0; i < 3; i++) {
		m[0][i] = right[i];
		m[1][i] = vup[i];
		m[2][i] = forward[i];
	}
	m[0][3] = -dot(right, eye);
	m[1][3] = -dot(vup, eye);
	m[2][3] = -dot(forward, eye);
	return m;
}

Matrix modelTransform()
{
	return Matrix();
}

}

