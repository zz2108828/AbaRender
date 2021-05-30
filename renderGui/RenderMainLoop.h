#pragma once
#include <QObject>
#include <tgaimage.h>
#include <core/model.h>
#include <core/zbuffer.h>
#include <core/Camera.h>
#include <core/Shader.h>

class RenderMainLoop : public QObject
{
	Q_OBJECT
public:
    explicit RenderMainLoop(const int width, const int height,QObject* parent = 0);
    ~RenderMainLoop();
    void mainLoop();// 线程处理函数
    void stop();
signals:
    void syn(unsigned char* frame_buffer);
public:
    aba::Camera* camera;
    aba::Shader* shader;
    aba::Vec3f light_dir;
    int fps = 0;
    bool is_wireframe = false;
    bool is_zbuffer = true;
    bool is_phone_shader = true;
    bool is_tex_shader = false;
private:
    int width_, height_;
    bool is_update_camera_matrix = false;
    bool is_stop_render_ = false;    
private:
    void renderModel(const aba::Model& model, aba::TGAImage& image, aba::ZBuffer* zbuffer);
};