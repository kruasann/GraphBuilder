// CustomChartView.cpp
#include "CustomChartView.h"

CustomChartView::CustomChartView(QChart* chart, QWidget* parent)
    : QChartView(chart, parent) {}

void CustomChartView::wheelEvent(QWheelEvent* event) {
    qreal factor = 1.1;
    if (event->angleDelta().y() > 0) {
        chart()->zoom(factor);
    }
    else {
        chart()->zoom(1 / factor);
    }
    event->accept();
}

void CustomChartView::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        isLeftMousePressed = true;
        lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }
    QChartView::mousePressEvent(event);
}

void CustomChartView::mouseMoveEvent(QMouseEvent* event) {
    if (isLeftMousePressed) {
        QPoint delta = event->pos() - lastMousePos;
        chart()->scroll(-delta.x(), delta.y());
        lastMousePos = event->pos();
        event->accept();
    }
    else {
        // Передаём координаты мыши в график
        QPointF chartPos = chart()->mapToValue(event->pos());
        emit mouseMoved(chartPos.x(), chartPos.y());
        QChartView::mouseMoveEvent(event);
    }
}

void CustomChartView::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        isLeftMousePressed = false;
        setCursor(Qt::ArrowCursor);
    }
    QChartView::mouseReleaseEvent(event);
}