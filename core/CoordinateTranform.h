#pragma once
#include <model.h>

namespace aba {
	class Model2ScreenTranfer {
	private:
		const Model* const modelptr_;
		Vec3f orgin_;
		float zoom_;
		float offset_x_;
		float offset_y_;
		float z_zoom_;
	public:
		Model2ScreenTranfer(const Model& model, uint32_t width, uint32_t height);
		Vec3f model2screen(const Vec3f& model_coord);
	};

	Matrix viewportTransform(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
	Matrix perspectiveTransform(float coff = 0.3f);
	Matrix cameraTransform(Vec3f camera, Vec3f center, Vec3f up);
	Matrix modelTransform();
}


