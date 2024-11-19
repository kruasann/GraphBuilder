#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QComboBox>
#include "GraphCanvas.h"

class GraphWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit GraphWindow(QWidget* parent = nullptr);
    ~GraphWindow();

private:
    GraphCanvas* canvas;       // ������� ��� �������
    QLineEdit* inputX;         // ���� ����� �������� x
    QPushButton* updateButton; // ������ ��� ���������� �������� x
    QComboBox* functionSelector; // ���������� ������ ��� ������ �������

private slots:
    void updateXValue();        // ���� ��� ���������� �������� x
    void updateFunction();      // ���� ��� ���������� ��������� �������
};

#endif // GRAPHWINDOW_H
