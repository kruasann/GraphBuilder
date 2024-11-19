#include "GraphCanvas.h"
#include <QPainter>
#include <QPainterPath>
#include <cmath>

GraphCanvas::GraphCanvas(QWidget* parent)
    : QWidget(parent), time(0.0), xValue(5.0), functionType(0) {
    // Создаем таймер для анимации
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GraphCanvas::updateTime);

    // Запускаем таймер с интервалом 50 мс
    timer->start(50);
}

GraphCanvas::~GraphCanvas() {
    if (timer->isActive()) {
        timer->stop();
    }
}

void GraphCanvas::setXValue(double newX) {
    xValue = newX;
    update();
}

void GraphCanvas::setFunction(int function) {
    functionType = function;
    update();
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

    // Параметры масштаба и делений
    double scaleX = 50.0; // Масштаб по оси X
    double scaleY = 50.0; // Масштаб по оси Y
    int numXTicks = 10;   // Количество отметок на оси X
    int numYTicks = 8;    // Количество отметок на оси Y

    // Центр системы координат
    double centerX = width / 2;
    double centerY = height / 2;

    // Рисуем оси
    painter.setPen(Qt::black);
    painter.drawLine(0, centerY, width, centerY); // Ось X
    painter.drawLine(centerX, 0, centerX, height); // Ось Y

    // Рисуем отметки и подписываем ось X
    for (int i = -numXTicks; i <= numXTicks; ++i) {
        double x = centerX + i * scaleX;
        if (x >= 0 && x <= width) {
            painter.drawLine(x, centerY - 5, x, centerY + 5); // Маленькая черточка
            if (i != 0) { // Подпись делений (не подписываем 0 на оси X)
                painter.drawText(x - 10, centerY + 20, QString::number(i));
            }
        }
    }

    // Рисуем отметки и подписываем ось Y
    for (int i = -numYTicks; i <= numYTicks; ++i) {
        double y = centerY - i * scaleY;
        if (y >= 0 && y <= height) {
            painter.drawLine(centerX - 5, y, centerX + 5, y); // Маленькая черточка
            if (i != 0) { // Подпись делений (не подписываем 0 на оси Y)
                painter.drawText(centerX + 10, y + 5, QString::number(i));
            }
        }
    }

    // Рисуем график функции
    QPainterPath path;
    double step = 0.01;

    double xStart = -width / 2 / scaleX;
    double xEnd = width / 2 / scaleX;

    auto computeY = [&](double x) {
        switch (functionType) {
        case 0: return std::sin(x + time);  // sin(x)
        case 1: return std::cos(x + time);  // cos(x)
        case 2: return x;                   // x (линейная)
        default: return 0.0;
        }
        };

    path.moveTo(centerX + xStart * scaleX, centerY - computeY(xStart) * scaleY);

    for (double x = xStart + step; x <= xEnd; x += step) {
        double y = computeY(x);
        double pixelX = centerX + x * scaleX;
        double pixelY = centerY - y * scaleY;
        path.lineTo(pixelX, pixelY);
    }

    painter.setPen(Qt::blue);
    painter.drawPath(path);

    // Подписываем оси
    painter.setPen(Qt::black);
    painter.drawText(width - 40, centerY - 10, "X"); // Подпись оси X
    painter.drawText(centerX + 10, 10, "Y");        // Подпись оси Y

    // Отображаем значение функции в точке xValue
    double yValue = computeY(xValue);
    QString text = QString("f(%1) = %2").arg(xValue).arg(yValue);
    painter.setPen(Qt::red);
    painter.drawText(10, 20, text);
}
