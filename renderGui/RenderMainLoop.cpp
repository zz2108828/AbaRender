#include "RenderMainLoop.h"
#include <core/Camera.h>
#include <core/CoordinateTranform.h>
#include <core/Shader.h>

RenderMainLoop::RenderMainLoop(const int width, const int height, QObject* parent)
	:width_(width),height_(height)
{
}

RenderMainLoop::~RenderMainLoop()
{

}

void RenderMainLoop::mainLoop()
{
	using namespace aba;
	//Model model("obj/african_head.obj");
	Model model("obj/clay_vase.obj");
	int width = width_, height = height_;
	TGAImage* cur_image = new TGAImage(width, height, RGB);
	TGAImage* tmp_image = new  TGAImage(width, height, RGB);
	ZBuffer* zbuffer = new ZBuffer(width, height);
	light_dir = Vec3f(1, 1, 1);
	Vec3f eye(0, 0, 100);
	Vec3f gaze(0, 0, 0);
	Vec3f up(0, 1, 0);
	camera = new Camera(eye, gaze, up, width * 1.0f / height);
	ModelTranfer tranfer(model, width, height);
	Shader* full_shader = new Shader(model, tranfer);
	Shader* phone_shader = new PhoneShader(model, tranfer);
	Shader* light_shader = new LightShader(model, tranfer);
	while (!is_stop_render_) {		
		if (is_phone_shader) {
			if (is_tex_shader) shader = full_shader;
			else shader = phone_shader;
		}
		else { shader = light_shader; }
		if (is_zbuffer) {
			renderModel(model, *cur_image, zbuffer);
		}
		else {
			renderModel(model, *cur_image, NULL);
		}

		emit syn(cur_image->getImageBuffer());
		std::swap(cur_image, tmp_image);
		cur_image->clear();
		fps++;
	}
	//drawTheModel(image, "obj/clay_vase.obj");
	//image.writeTGAFile("out.tga");
}

void RenderMainLoop::renderModel(const aba::Model& model, aba::TGAImage& image, aba::ZBuffer* zbuffer)
{
	using namespace aba;
	uint32_t width = image.getWidth(), height = image.getHeight();
	shader->setLightDir(light_dir);
	shader->camera_matrix_ = camera->getCameraMatrix();
	shader->project_matrix_ = camera->getPerspectiveMatrix();
	//printf("faces:{%llu},verts:{%llu}\n", model.getFacesNum(), model.getVertsNum());
	for (int i = 0; i < model.getFacesNum(); i++) {
		Vertex screen_coord[3];
		Vec3f pos[3];
		for (int j = 0; j < 3; j++) {
			screen_coord[j] = shader->vetexShade(i, j);
			pos[j].x = screen_coord[j].x;
			pos[j].y = screen_coord[j].y;
			pos[j].z = screen_coord[j].z;
		}
		if (!is_phone_shader) {
			Vec3f n = cross((pos[2] - pos[0]), (pos[1] - pos[0]));
			float t = std::min(1.0f, std::max(0.3f, dot(n.normalize(), Vec3f(1,0, -1).normalize())));
			screen_coord[0].light_intensity = t;
			screen_coord[1].light_intensity = t;
			screen_coord[2].light_intensity = t;
		}
		// Draw
		if (is_wireframe) {
			drawWireframe(screen_coord, Color(255,255,255), image, zbuffer);
		}
		else {
			drawTriangle(screen_coord, shader, image, zbuffer);
		}
	}
	if (zbuffer != NULL) {
		zbuffer->clear();
	}
}

void RenderMainLoop::stop()
{
	is_stop_render_ = true;
}



