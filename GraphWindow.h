#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include "GraphCanvas.h"

class GraphWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit GraphWindow(QWidget* parent = nullptr);
    ~GraphWindow();

private:
    GraphCanvas* canvas; // Полотно для графика
    QLineEdit* inputX;   // Поле ввода значения x
    QPushButton* updateButton; // Кнопка для обновления значения x

private slots:
    void updateXValue(); // Слот для обновления значения x
};

#endif // GRAPHWINDOW_H
