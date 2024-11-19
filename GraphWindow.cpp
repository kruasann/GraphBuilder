#include "GraphWindow.h"

GraphWindow::GraphWindow(QWidget* parent) : QMainWindow(parent) {
    // ������� ������� ��� ��������� �������.
    GraphCanvas* canvas = new GraphCanvas(this);
    setCentralWidget(canvas);

    // ������������� ��������� ����.
    setWindowTitle("Graph of sin(x)");
    resize(800, 600);
}

GraphWindow::~GraphWindow() {
    // ��� ������������� � ����� �������� canvas, ��� ��� �� �������� ����������� �������� � ����� ������������� ������.
}
