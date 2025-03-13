#include "workspace3dwidget.h"
#include <QOpenGLShaderProgram>
#include <QDebug>

Workspace3DWidget::Workspace3DWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
}

Workspace3DWidget::~Workspace3DWidget()
{
}

void Workspace3DWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 设置黑色背景
    glEnable(GL_DEPTH_TEST);  // 开启深度测试
}

void Workspace3DWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Workspace3DWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // TODO: 添加 3D 渲染代码（如 OpenGL 立方体）
    glLoadIdentity(); // 重置变换矩阵

    // 绘制 XYZ 坐标轴
    drawXYZAxis();
}
void Workspace3DWidget::drawXYZAxis()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    // 设置正交投影，使 XYZ 轴固定在左下角
    glOrtho(0, width(), 0, height(), -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glLineWidth(2.0f);

    glBegin(GL_LINES);

    // X 轴 (红色)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(20, 20);
    glVertex2f(50, 20);

    // Y 轴 (绿色)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(20, 20);
    glVertex2f(20, 50);

    // Z 轴 (蓝色)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(20, 20);
    glVertex2f(35, 35);

    glEnd();

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}
