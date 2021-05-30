#include "Camera.h"
#include <core/CoordinateTranform.h>

namespace aba {
	Camera::Camera(const Vec3f& eye, const Vec3f& lookat, const Vec3f& up, float aspect) :eye_(eye), lookat_(lookat), up_(up), aspect_(aspect)
	{
		x_angle = 89;
		perspective_mat = perspectiveTransform(100, aspect_, 0.1, 1000);
	}

	Matrix Camera::getCameraMatrix()
	{
		if (is_update_camera_matrix) {
			is_update_camera_matrix = false;
			eye_ = eular2Dir(Vec3f(x_angle, y_angle,0.0f));
			camera_mat = cameraTransform(eye_, lookat_, up_);
		}
		return camera_mat;
	}

	Matrix Camera::getPerspectiveMatrix()
	{
		if (is_update_perspective_matrix) {
			is_update_perspective_matrix = false;
			perspective_mat = perspectiveTransform(100, aspect_, 0.1, 1000);
		}
		return perspective_mat;
	}

	void Camera::moveForward(float dis)
	{

	}

	void Camera::moveUp(float dis)
	{

	}

	void Camera::moveRight(float dis)
	{

	}

	void Camera::rotateX(float angle)
	{
		x_angle += angle;
		// (0,180)
		if (x_angle >= 180)
			x_angle = 179;
		if (x_angle <= 0)
			x_angle = 1;
		is_update_camera_matrix = true;
	}

	void Camera::rotateY(float angle)
	{
		y_angle += angle;
		if (y_angle > 360)
			y_angle = 0;
		if (y_angle < 0)
			y_angle = 360;
		is_update_camera_matrix = true;
	}

	Vec3f Camera::eular2Dir(Vec3f euler)
	{
		Vec3f dir;
		float radian = PI / 180;
		dir.x = sin(radian * euler.x) * sin(radian * euler.y);
		dir.y = cos(radian * euler.x);
		dir.z = sin(radian * euler.x) * cos(radian * euler.y);
		return dir;
	}
}