// CustomChartView.h
#ifndef CUSTOMCHARTVIEW_H
#define CUSTOMCHARTVIEW_H

#include <QtCharts/QChartView>
#include <QMouseEvent>
#include <QWheelEvent>

class CustomChartView : public QChartView {
    Q_OBJECT
public:
    explicit CustomChartView(QChart* chart, QWidget* parent = nullptr);

signals:
    void mouseMoved(double x, double y); // Сигнал для передачи координат мыши

protected:
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    QPoint lastMousePos;
    bool isLeftMousePressed = false;
};

#endif // CUSTOMCHARTVIEW_H