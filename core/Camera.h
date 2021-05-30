#pragma once
#include "geometry.h"

namespace aba{
	class Camera {
	public:
		Camera(const Vec3f& eye, const Vec3f& lookat,const Vec3f& up, float aspect);
		Vec3f eye_, lookat_,up_;
		Vec3f pos;
		float aspect_;
	private:
		Matrix camera_mat;
		Matrix perspective_mat;
		float x_angle = 0, y_angle = 0;
		bool is_update_camera_matrix = true;
		bool is_update_perspective_matrix = true;
	public:
		Matrix getCameraMatrix();
		Matrix getPerspectiveMatrix();
		void moveForward(float dis);
		void moveUp(float dis);
		void moveRight(float dis);
		void rotateX(float angle);
		void rotateY(float angle);
	private:
		Vec3f eular2Dir(Vec3f euler);
	};
}
