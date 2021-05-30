#pragma once
#include <renderGui/RenderMainLoop.h>
#include <qthread.h>
#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>
#include <QQueue>

namespace Ui {
    class RenderGui;
}
class RenderGui : public QWidget
{
    Q_OBJECT
public:
    explicit RenderGui(const int width, const int height,QWidget* parent = 0);
    ~RenderGui();
private slots:
    void GetFps();
protected slots:
    void syn(unsigned char* frame_buffer);
private:
    //Ui::RenderGui* ui;
    QImage* canvas_ = nullptr;
    RenderMainLoop* render_;
    QThread* render_thread_;
    QTimer* fps_timer;
    int fps;
protected:
    void paintEvent(QPaintEvent* event);
    virtual void keyPressEvent(QKeyEvent* eve);
};