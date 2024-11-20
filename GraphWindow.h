#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QToolTip>

class GraphWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit GraphWindow(QWidget* parent = nullptr);
    ~GraphWindow();

protected:
    void wheelEvent(QWheelEvent* event) override;    // Обработка колесика мыши
    void mousePressEvent(QMouseEvent* event) override; // Обработка нажатия мыши
    void mouseMoveEvent(QMouseEvent* event) override;  // Обработка перемещения мыши

private:
    QChart* chart = nullptr;
    QChartView* chartView = nullptr;
    QLineSeries* series = nullptr;
    QScatterSeries* intersectionSeries = nullptr;
    QScatterSeries* hoverSeries = nullptr;
    QLineEdit* functionInput = nullptr;
    QPushButton* plotButton = nullptr;
    QValueAxis* axisX = nullptr;
    QValueAxis* axisY = nullptr;
    QString userFunction;

    QPointF lastMousePos; // Последняя позиция мыши

private:
    void updateGraph();                   // Метод для обновления графика
    void updateIntersections();           // Метод для поиска точек пересечения с осями
    double evaluateExpression(double x);  // Вычисление значения функции
    QPointF findClosestPoint(const QPointF& scenePos); // Метод для нахождения ближайшей точки

private slots:
    void plotGraph();                     // Слот для построения графика
};

#endif // GRAPHWINDOW_H
