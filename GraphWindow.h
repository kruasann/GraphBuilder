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
    void wheelEvent(QWheelEvent* event) override;    // ��������� �������� ����
    void mousePressEvent(QMouseEvent* event) override; // ��������� ������� ����
    void mouseMoveEvent(QMouseEvent* event) override;  // ��������� ����������� ����
    void mouseReleaseEvent(QMouseEvent* event) override; // ��������� ���������� ����

private:
    QChart* chart = nullptr;
    QChartView* chartView = nullptr;
    QLineSeries* series = nullptr;
    QScatterSeries* intersectionSeries = nullptr;
    QScatterSeries* hoverSeries = nullptr;
    QLineSeries* xAxisZeroLine = nullptr;
    QLineSeries* yAxisZeroLine = nullptr;
    QLineEdit* functionInput = nullptr;
    QPushButton* plotButton = nullptr;
    QValueAxis* axisX = nullptr;
    QValueAxis* axisY = nullptr;
    QString userFunction;

    QPoint lastMousePos; // ��������� ������� ����
    bool isLeftMousePressed = false;

private:
    void updateGraph();                   // ����� ��� ���������� �������
    void updateIntersections();           // ����� ��� ������ ����� ����������� � �����
    void updateZeroLines();               // ����� ��� ���������� ������� �����
    double evaluateExpression(double x);  // ���������� �������� �������
    QPointF findClosestPoint(const QPointF& chartPos); // ����� ��� ���������� ��������� �����

private slots:
    void plotGraph();                     // ���� ��� ���������� �������
};

#endif // GRAPHWINDOW_H
