#include "GraphCanvas.h"
#include <QPainter>
#include <QPainterPath>
#include <cmath>

GraphCanvas::GraphCanvas(QWidget* parent) : QWidget(parent), time(0.0), xValue(5.0) {
    // Создаем таймер для анимации (если понадобится)
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GraphCanvas::updateTime);
    // Запускаем таймер (закомментируйте, если анимация не нужна)
    // timer->start(50);
}

GraphCanvas::~GraphCanvas() {
    if (timer->isActive()) {
        timer->stop();
    }
}

void GraphCanvas::updateTime() {
    time += 0.1;
    update();
}

void GraphCanvas::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRectF rect = this->rect();
    double width = rect.width();
    double height = rect.height();

    // Рисуем оси
    painter.setPen(Qt::black);
    painter.drawLine(0, height / 2, width, height / 2); // Ось X
    painter.drawLine(width / 2, 0, width / 2, height);  // Ось Y

    // Рисуем график sin(x)
    QPainterPath path;
    double step = 0.01;
    double scaleX = 50.0;
    double scaleY = 50.0;

    double xStart = -width / 2 / scaleX;
    double xEnd = width / 2 / scaleX;

    path.moveTo(width / 2 + xStart * scaleX, height / 2 - std::sin(xStart + time) * scaleY);

    for (double x = xStart + step; x <= xEnd; x += step) {
        double y = std::sin(x + time);
        double pixelX = width / 2 + x * scaleX;
        double pixelY = height / 2 - y * scaleY;
        path.lineTo(pixelX, pixelY);
    }

    painter.setPen(Qt::blue);
    painter.drawPath(path);

    // Отображаем значение sin(x) при x = xValue
    double yValue = std::sin(xValue + time);
    QString text = QString("sin(%1) = %2").arg(xValue).arg(yValue);
    painter.setPen(Qt::red);
    painter.drawText(10, 20, text);
}
