#include <renderGui/RenderUi.h>
#include <QApplication>

RenderGui::RenderGui(const int width, const int height,QWidget* parent) : QWidget(parent)
{
	setFixedSize(width, height);
	printf("%d %d",this->width(),this->height());
	render_thread_ = new QThread(this);
	render_ = new RenderMainLoop(this->width(), this->height());
	render_->moveToThread(render_thread_);
	connect(render_thread_, &QThread::started, render_, &RenderMainLoop::mainLoop);
	connect(render_, &RenderMainLoop::syn, this, &RenderGui::syn);
	
	// FPS
	fps_timer = new QTimer();
	connect(fps_timer, &QTimer::timeout,this,&RenderGui::GetFps);

	//Start
	render_thread_->start();
	fps_timer->start(1000);
}

RenderGui::~RenderGui()
{
	render_->stop();
	render_thread_->requestInterruption();
	render_thread_->quit();
	render_thread_->wait();
	render_ = nullptr;
	render_thread_ = nullptr;
}

void RenderGui::GetFps()
{
	fps = render_->fps;
	render_->fps = 0;
}

void RenderGui::syn(unsigned char* frame_buffer)
{
	if (canvas_)
		delete canvas_;
	canvas_ = new QImage(frame_buffer, this->width(), this->height(), QImage::Format_BGR888);
	this->setWindowTitle("AbaRender" + QString(" FPS: %1").arg(fps));
	update();
}

void RenderGui::paintEvent(QPaintEvent* event)
{
	if (canvas_) {
		QPainter painter(this);
		painter.drawImage(QRect(0, 0, this->width(), this->height()), canvas_->mirrored(false,false));
	}
	QWidget::paintEvent(event);
}

void RenderGui::keyPressEvent(QKeyEvent* eve)
{
	if (eve->key() == Qt::Key_Escape) {
		render_->stop();
		QApplication* app;
		app->quit();
	}
	switch (eve->key()) {
		case Qt::Key_W:
			render_->camera->rotateX(-1);
			break;
		case Qt::Key_S:
			render_->camera->rotateX(1);
			break;
		case Qt::Key_A:
			render_->camera->rotateY(-1);
			break;
		case Qt::Key_D:
			render_->camera->rotateY(1);
			break;
		case Qt::Key_1:
			render_->is_wireframe = !render_->is_wireframe;
			break;
		case Qt::Key_2:
			render_->is_zbuffer = !render_->is_zbuffer;
			break;
		case Qt::Key_3:
			render_->is_phone_shader = !render_->is_phone_shader;
			break;
		case Qt::Key_4:
			render_->is_tex_shader = !render_->is_tex_shader;
			break;
	}
}
