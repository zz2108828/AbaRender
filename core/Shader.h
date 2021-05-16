#pragma once
#include <tgaimage.h>
#include <vector>
#include <geometry.h>
#include <zbuffer.h>
#include <model.h>
#include <CoordinateTranform.h>

namespace aba {
	struct Vertex {
		float x, y, z;
		float light_intensity;
		Vec3f uv;
		Vertex(Vec3f pos, float light_intensity) :
			x(pos.x), y(pos.y), z(pos.z),light_intensity(light_intensity) {
		}
		Vertex(int x, int y, float light_intensity) :
			x(x), y(y), z(0), light_intensity(light_intensity) {
		}
		Vertex():x(0),y(0),z(0),light_intensity(0){}
	};

	class Shader {
	private:
		Model2ScreenTranfer tranfer;
		const Model* const model_;
		Vec3f light_dir_;// normalize
	public:
		Shader(const Model& model) : model_(&model), tranfer(model, 1000, 1000) {
		}

		virtual void setLightDir(Vec3f light_dir) {
			light_dir_ = light_dir.normalize();
		}

		virtual Vertex vetexShade(int iface, int ivetex) {
			
			Vertex result;
			Vec3f vert = model_->getVert(iface, ivetex);
			vert = tranfer.model2screen(vert); // coordination
			result.x = vert.x;
			result.y = vert.y;
			result.z = vert.z;
			result.uv = model_->getUv(iface, ivetex); // uv
			//Vec3f normal = model_->getVertNormal(iface, ivetex);// vetex normal
			//float intensity = dot(normal, light_dir_); // calculte the cos¦È
			//result.light_intensity = std::max(0.0f, intensity);// intensity
			return result;
		}

		virtual bool fragmentShade(Vertex vetex, Color& color) {
			Vec3f uv = vetex.uv;
			//// warning:we use isometry to tranfer,so we don't need to tranfer normal vector
			Vec3f normal = model_->getNormalMap(uv).normalize();
			Vec3f light = light_dir_.normalize();
			float t = dot(normal, light) *2.0f;
			Vec3f tt = normal * t;
			Vec3f reflect = (tt - light).normalize();
			float TTT =  model_->getSpecular(uv);
			float specular = std::pow(std::max(reflect.z, 0.0f), model_->getSpecular(uv));
			float intensity = dot(normal, light);
			float diffuse = std::max(0.0f, intensity);
			//float intensity = vetex.light_intensity;
			color = model_->getDiffuseColor(uv);
			for (int i = 0; i < 3; i++) {
				float t = std::min(5 + color[i] * (diffuse + 0.6f * specular), 255.0f);
				color[i] = t;
 			}
			return false;
		}
	};

	std::vector<Vertex> getLinePoints(Vertex v0, Vertex v1, bool is_Transpose = true);
	//void drawPoints(const std::vector<Vec3f>& points,IShader& shader,TGAImage& image, ZBuffer* z_buffer);
	//void drawLine(Vec3f v0, Vec3f v1, IShader& shader, TGAImage& image, bool is_Transpose = false, ZBuffer* z_buffer = NULL);
	void drawTriangle(Vertex* vetexs, Shader& shader, TGAImage& image, ZBuffer* z_buffer = NULL);
}

