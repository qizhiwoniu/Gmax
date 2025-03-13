#include "workspace2dwidget.h"

Workspace2DWidget::Workspace2DWidget(QWidget* parent)
    : QGraphicsView(parent), gridSize(20)
{
    scene = new QGraphicsScene(this);
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::NoDrag);
    setMouseTracking(true);
    drawGrid();
}

void Workspace2DWidget::wheelEvent(QWheelEvent* event)
{
    double scaleFactor = 1.15;
    if (event->angleDelta().y() > 0) {
        scale(scaleFactor, scaleFactor);
    } else {
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
}

void Workspace2DWidget::mouseMoveEvent(QMouseEvent* event)
{
    lastMousePos = mapToScene(event->pos());
    viewport()->update();
    QGraphicsView::mouseMoveEvent(event);
}

void Workspace2DWidget::drawBackground(QPainter* painter, const QRectF& rect)
{
    QGraphicsView::drawBackground(painter, rect);
    painter->setPen(QPen(Qt::gray, 0.5));

    qreal left = int(rect.left()) - (int(rect.left()) % gridSize);
    qreal top = int(rect.top()) - (int(rect.top()) % gridSize);

    for (qreal x = left; x < rect.right(); x += gridSize)
        painter->drawLine(QLineF(x, rect.top(), x, rect.bottom()));

    for (qreal y = top; y < rect.bottom(); y += gridSize)
        painter->drawLine(QLineF(rect.left(), y, rect.right(), y));

    painter->setPen(QPen(Qt::red, 0.8));
    painter->drawLine(QLineF(lastMousePos.x(), rect.top(), lastMousePos.x(), rect.bottom()));
    painter->drawLine(QLineF(rect.left(), lastMousePos.y(), rect.right(), lastMousePos.y()));
}

void Workspace2DWidget::drawGrid()
{
    scene->setSceneRect(-1000, -1000, 2000, 2000);
}
