#ifndef GRAPHCANVAS_H
#define GRAPHCANVAS_H

#include <QWidget>
#include <QTimer>

class GraphCanvas : public QWidget {
    Q_OBJECT

public:
    explicit GraphCanvas(QWidget* parent = nullptr);
    ~GraphCanvas();

    void setXValue(double newX);     // ����� ��� ��������� ������ �������� x
    void setFunction(int function); // ����� ��� ��������� ��������� �������

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QTimer* timer;
    double time;
    double xValue; // ������� �������� x
    int functionType; // ��� �������: 0 - sin(x), 1 - cos(x), 2 - x

private slots:
    void updateTime(); // ���� ��� ���������� �������
};

#endif // GRAPHCANVAS_H
