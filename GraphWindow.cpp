#include "GraphWindow.h"

GraphWindow::GraphWindow(QWidget* parent) : QMainWindow(parent) {
    // Создаем область для рисования графика.
    GraphCanvas* canvas = new GraphCanvas(this);
    setCentralWidget(canvas);

    // Устанавливаем параметры окна.
    setWindowTitle("Graph of sin(x)");
    resize(800, 600);
}

GraphWindow::~GraphWindow() {
    // Нет необходимости в явном удалении canvas, так как он является центральным виджетом и будет автоматически удален.
}
