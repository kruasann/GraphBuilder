#ifndef GRAPHCANVAS_H
#define GRAPHCANVAS_H

#include <QWidget>
#include <QTimer>

class GraphCanvas : public QWidget {
    Q_OBJECT

public:
    explicit GraphCanvas(QWidget* parent = nullptr);
    ~GraphCanvas();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QTimer* timer;
    double time;
    double xValue; // Значение x по умолчанию

private slots:
    void updateTime();
};

#endif // GRAPHCANVAS_H
