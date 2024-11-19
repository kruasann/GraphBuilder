#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

class GraphWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit GraphWindow(QWidget* parent = nullptr);
    ~GraphWindow();

private:
    QChart* chart;                // ������
    QChartView* chartView;        // ������������� �������
    QLineSeries* series;          // ����� ������ ��� �������
    QLineEdit* inputX;            // ���� ����� �������� x
    QPushButton* updateButton;    // ������ ��� ���������� �������� x
    QComboBox* functionSelector;  // ���������� ������ ��� ������ �������
    QValueAxis* axisX;            // ��� X
    QValueAxis* axisY;            // ��� Y
    double xValue;                // ������� �������� X
    int functionType;             // ��������� �������: 0 - sin(x), 1 - cos(x), 2 - x (��������)

private:
    void updateGraph();           // ����� ��� ���������� �������

private slots:
    void updateXValue();          // ���� ��� ���������� �������� x
    void updateFunction();        // ���� ��� ������ �������
};

#endif // GRAPHWINDOW_H
