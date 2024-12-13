// CustomChartView.cpp
#include "CustomChartView.h"
#include <QtWidgets/QApplication>
#include <QGraphicsGridLayout>
#include <QValueAxis>
#include <QIcon>

CustomChartView::CustomChartView(QChart* chart, QWidget* parent)
    : QChartView(chart, parent),
    leftMouseButtonPressed(false),
    rightMouseButtonPressed(false) {}

void CustomChartView::wheelEvent(QWheelEvent* event) {
    const qreal zoomFactor = 1.1;
    if (event->modifiers() & Qt::ControlModifier) { 
        event->angleDelta().y() > 0 ? chart()->zoomIn() : chart()->zoomOut();
    }
    else {
        event->angleDelta().y() > 0 ? chart()->zoom(zoomFactor) : chart()->zoom(1 / zoomFactor);
    }
    event->accept();
}

void CustomChartView::toggleGrid() {
    QList<QAbstractAxis*> axes = chart()->axes();
    for (QAbstractAxis* axis : axes) {
        if (auto* valueAxis = dynamic_cast<QValueAxis*>(axis)) {
            valueAxis->setGridLineVisible(!valueAxis->isGridLineVisible());
        }
    }
}

void CustomChartView::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        leftMouseButtonPressed = true;
        lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }
    else if (event->button() == Qt::RightButton) {  
        rightMouseButtonPressed = true;
        lastMousePos = event->pos();
        setCursor(Qt::OpenHandCursor);
    }
    QChartView::mousePressEvent(event);
}

void CustomChartView::mouseMoveEvent(QMouseEvent* event) {
    if (leftMouseButtonPressed || rightMouseButtonPressed) {
        QPoint delta = event->pos() - lastMousePos;
        chart()->scroll(-delta.x(), delta.y());
        lastMousePos = event->pos();
        event->accept();
    }
    else {
        QPointF chartPos = chart()->mapToValue(event->pos());
        emit mouseMoved(chartPos.x(), chartPos.y());
        QToolTip::showText(event->globalPosition().toPoint(),
            QString("X: %1, Y: %2").arg(chartPos.x()).arg(chartPos.y()));
        QChartView::mouseMoveEvent(event);
    }
}

void CustomChartView::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        leftMouseButtonPressed = false;
        setCursor(Qt::ArrowCursor);
    }
    else if (event->button() == Qt::RightButton) {
        rightMouseButtonPressed = false;
        setCursor(Qt::ArrowCursor);
    }
    QChartView::mouseReleaseEvent(event);
}

void CustomChartView::contextMenuEvent(QContextMenuEvent* event) {
    QMenu contextMenu(this);

    QAction* resetZoomAction = new QAction(QIcon(":/icons/images/reset_zoom.png"), "Reset zoom", this);
    connect(resetZoomAction, &QAction::triggered, this, &CustomChartView::resetZoom);
    contextMenu.addAction(resetZoomAction);

    QAction* toggleGridAction = new QAction(QIcon(":/icons/images/toggle_grid.png"), "Toggle grid", this);
    connect(toggleGridAction, &QAction::triggered, this, &CustomChartView::toggleGrid);
    contextMenu.addAction(toggleGridAction);

    QAction* changeThemeAction = new QAction(QIcon(":/icons/images/change_theme.png"), "Change theme", this);
    connect(changeThemeAction, &QAction::triggered, this, &CustomChartView::changeTheme);
    contextMenu.addAction(changeThemeAction);

    contextMenu.exec(event->globalPos());
}

void CustomChartView::resetZoom() {
    chart()->zoomReset();  
}

void CustomChartView::changeTheme() {
    static bool isDarkTheme = true;
    if (isDarkTheme) {
        chart()->setBackgroundRoundness(0);
        chart()->setTheme(QChart::ChartThemeDark);
    }
    else {
        chart()->setBackgroundRoundness(10);
        chart()->setTheme(QChart::ChartThemeLight);
    }
    isDarkTheme = !isDarkTheme;
}
