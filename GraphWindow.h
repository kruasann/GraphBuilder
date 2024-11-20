#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QMouseEvent>
#include <QWheelEvent>

class GraphWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit GraphWindow(QWidget* parent = nullptr);
    ~GraphWindow();

protected:
    void wheelEvent(QWheelEvent* event) override;    // ��������� �������� ����
    void mousePressEvent(QMouseEvent* event) override; // ��������� ������� ����
    void mouseMoveEvent(QMouseEvent* event) override;  // ��������� ����������� ����

private:
    QChart* chart = nullptr;
    QChartView* chartView = nullptr;
    QLineSeries* series = nullptr;
    QLineEdit* functionInput = nullptr;
    QPushButton* plotButton = nullptr;
    QValueAxis* axisX = nullptr;
    QValueAxis* axisY = nullptr;
    QString userFunction;

    QPointF lastMousePos; // ��������� ������� ����

private:
    void updateGraph();                   // ����� ��� ���������� �������
    double evaluateExpression(double x);  // ���������� �������� �������

private slots:
    void plotGraph();                     // ���� ��� ���������� �������
};

#endif // GRAPHWINDOW_H
