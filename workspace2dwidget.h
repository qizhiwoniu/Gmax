#ifndef WORKSPACE2DWIDGET_H
#define WORKSPACE2DWIDGET_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QPainter>


class Workspace2DWidget : public QGraphicsView
{
    Q_OBJECT

public:
    explicit Workspace2DWidget(QWidget* parent = nullptr);

protected:
    void wheelEvent(QWheelEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void drawBackground(QPainter* painter, const QRectF& rect) override;

private:
    QGraphicsScene* scene;
    QPointF lastMousePos;
    int gridSize;

    void drawGrid();
};

#endif // WORKSPACE2DWIDGET_H
