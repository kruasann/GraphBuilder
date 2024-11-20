#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

class GraphWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit GraphWindow(QWidget* parent = nullptr);
    ~GraphWindow();

private:
    QChart* chart = nullptr;
    QChartView* chartView = nullptr;
    QLineSeries* series = nullptr;
    QLineEdit* functionInput = nullptr;
    QPushButton* plotButton = nullptr;
    QValueAxis* axisX = nullptr;
    QValueAxis* axisY = nullptr;
    QString userFunction;

private:
    void updateGraph();                   // ����� ��� ���������� �������
    double evaluateExpression(double x);  // ���������� �������� �������

private slots:
    void plotGraph();                     // ���� ��� ���������� �������
};

#endif // GRAPHWINDOW_H
