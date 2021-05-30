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
		Vec3f normal;
		Vertex(Vec3f pos, float light_intensity) :
			x(pos.x), y(pos.y), z(pos.z),light_intensity(light_intensity) {
		}
		Vertex(int x, int y, float light_intensity) :
			x(x), y(y), z(0), light_intensity(light_intensity) {
		}
		Vertex():x(0),y(0),z(0),light_intensity(0){}
	};

	class Shader {
	protected:
		Model* model_;
		Vec3f light_dir_;// normalize
		const ModelTranfer* tranfer_;
	public:		
		Matrix project_matrix_;
		Matrix camera_matrix_;
		Shader(Model& model,const ModelTranfer& tranfer) : model_(&model), tranfer_(&tranfer) {

		}

		void setModel(Model& model) {
			*model_ = model;
		}

		void setLightDir(Vec3f light_dir) {
			light_dir_ = light_dir.normalize();
		}

		virtual Vertex vetexShade(int iface, int ivetex) {
			Vertex result;
			Vec3f vert = model_->getVert(iface, ivetex);
			//Matrix pos = project_matrix_ * vert;
			Matrix pos = camera_matrix_ * vert;
			vert.x = pos[0][0];
			vert.y = pos[1][0];
			vert.z = pos[2][0];
			vert = tranfer_->model2screen(vert); // coordination
			result.x = vert.x;
			result.y = vert.y;
			result.z = vert.z;
			result.uv = model_->getUv(iface, ivetex); // uv
			result.normal = model_->getVertNormal(iface, ivetex);// vetex normal
			
			float intensity = dot(result.normal, light_dir_); // calculte the cos¦È
			result.light_intensity = std::max(0.3f, intensity);// intensity

			return result;
		}

		virtual bool fragmentShade(Vertex vetex, Color& color){
			Vec3f uv = vetex.uv;

			// warning:we use isometry to tranfer,so we don't need to tranfer normal vector
			Vec3f normal;
			if (model_->hasNormalMap()) {
				normal = model_->getNormalMap(uv).normalize();
			}
			else {
				normal = vetex.normal;
			}
			normal = normal.normalize();
			Vec3f light = light_dir_;
			Vec3f reflect = (normal * (dot(normal, light) *2.0f) - light).normalize();
			float specular = std::pow(std::max(reflect.z, 0.0f), model_->getSpecular(uv));
			float intensity = dot(normal, light);
			float diffuse = std::max(0.1f, intensity);
			color = model_->getDiffuseColor(uv);
			for (int i = 0; i < 3; i++) {
				float t = std::min(25 + color[i] * (diffuse + 0.6f * specular), 255.0f);
				color[i] = t;
 			}
			return false;
		}
	};

	class LightShader :public Shader {
	public:
		LightShader(Model& model, const ModelTranfer& tranfer) : Shader(model, tranfer) {
		}

		Vertex vetexShade(int iface, int ivetex) override {
			Vertex result;
			Vec3f vert = model_->getVert(iface, ivetex);
			Matrix pos = camera_matrix_ * vert;
			vert.x = pos[0][0];
			vert.y = pos[1][0];
			vert.z = pos[2][0];
			vert = tranfer_->model2screen(vert); // coordination
			result.x = vert.x;
			result.y = vert.y;
			result.z = vert.z;
			//result.normal = model_->getVertNormal(iface, 0);// vetex normal
			//float intensity = dot(result.normal, light_dir_); // calculte the cos¦È
			//result.light_intensity = std::max(0.3f, intensity);// intensity
			return result;
		}

		bool fragmentShade(Vertex vetex, Color& color) override {
			float intensity = vetex.light_intensity;
			color = Color(255, 255, 255) * vetex.light_intensity;
			return false;
		}
	};

	class PhoneShader :public Shader {
	public:
		PhoneShader(Model& model, const ModelTranfer& tranfer) : Shader(model, tranfer) {
		}

		Vertex vetexShade(int iface, int ivetex) override {
			Vertex result;
			Vec3f vert = model_->getVert(iface, ivetex);
			Matrix pos = camera_matrix_ * vert;
			vert.x = pos[0][0];
			vert.y = pos[1][0];
			vert.z = pos[2][0];
			vert = tranfer_->model2screen(vert); // coordination
			result.x = vert.x;
			result.y = vert.y;
			result.z = vert.z;
			result.normal = model_->getVertNormal(iface, ivetex);// vetex normal
			float intensity = dot(result.normal, light_dir_); // calculte the cos¦È
			result.light_intensity = std::max(0.3f, intensity);// intensity
			return result;
		}

		bool fragmentShade(Vertex vetex, Color& color) override {
			float intensity = vetex.light_intensity;
			color = Color(255, 255, 255) * vetex.light_intensity;
			return false;
		}
	};

	std::vector<Vertex> getLinePoints(Vertex v0, Vertex v1, bool is_Transpose = true);
	//void drawPoints(const std::vector<Vec3f>& points,IShader& shader,TGAImage& image, ZBuffer* z_buffer);
	//void drawWireframe(Vec3f v0, Vec3f v1, IShader& shader, TGAImage& image, bool is_Transpose = false, ZBuffer* z_buffer = NULL);
	void drawTriangle(Vertex* vetexs, Shader* shader, TGAImage& image, ZBuffer* z_buffer = NULL);
	void drawWireframe(Vertex* vetexs, Color& color, TGAImage& image, ZBuffer* z_buffer = NULL);
}

