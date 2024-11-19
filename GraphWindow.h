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
    QChart* chart;                // График
    QChartView* chartView;        // Представление графика
    QLineSeries* series;          // Серия данных для графика
    QLineEdit* inputX;            // Поле ввода значения x
    QPushButton* updateButton;    // Кнопка для обновления значения x
    QComboBox* functionSelector;  // Выпадающий список для выбора функции
    QValueAxis* axisX;            // Ось X
    QValueAxis* axisY;            // Ось Y
    double xValue;                // Текущее значение X
    int functionType;             // Выбранная функция: 0 - sin(x), 1 - cos(x), 2 - x (линейная)

private:
    void updateGraph();           // Метод для обновления графика

private slots:
    void updateXValue();          // Слот для обновления значения x
    void updateFunction();        // Слот для выбора функции
};

#endif // GRAPHWINDOW_H
