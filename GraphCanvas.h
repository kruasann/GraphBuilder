#ifndef GRAPHCANVAS_H
#define GRAPHCANVAS_H

#include <QWidget>
#include <QTimer>

class GraphCanvas : public QWidget {
    Q_OBJECT

public:
    explicit GraphCanvas(QWidget* parent = nullptr);
    ~GraphCanvas();

    void setXValue(double newX);     // Метод для установки нового значения x
    void setFunction(int function); // Метод для установки выбранной функции

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QTimer* timer;
    double time;
    double xValue; // Текущее значение x
    int functionType; // Тип функции: 0 - sin(x), 1 - cos(x), 2 - x

private slots:
    void updateTime(); // Слот для обновления времени
};

#endif // GRAPHCANVAS_H
