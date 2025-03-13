#ifndef WORKSPACE3DWIDGET_H
#define WORKSPACE3DWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>

class Workspace3DWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit Workspace3DWidget(QWidget* parent = nullptr);
    ~Workspace3DWidget() override;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void drawXYZAxis();
};

#endif // WORKSPACE3DWIDGET_H
