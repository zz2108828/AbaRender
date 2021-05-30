#pragma once
#include <model.h>
#include <math.h>

namespace aba {
	class ModelTranfer {
	private:
		const Model* const modelptr_;
		Vec3f orgin_;
		float zoom_;
		float offset_x_;
		float offset_y_;
		float z_zoom_;
	public:
		ModelTranfer(const Model& model, uint32_t width, uint32_t height) ;
		Vec3f model2screen(const Vec3f& model_coord) const;
		Vec3f model2world(const Vec3f& model_coord) const;
	};

	// All matrixs are column-major order

	Matrix viewportTransform(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
	//Matrix perspectiveTransform(float coeff);
	Matrix perspectiveTransform(float fov, float aspect, float near, float far);
	Matrix cameraTransform(Vec3f camera, Vec3f center, Vec3f up);
	Matrix modelTransform();
}


