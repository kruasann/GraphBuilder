#include "GraphWindow.h"

GraphWindow::GraphWindow(QWidget* parent) : QMainWindow(parent) {
    // Создаем центральный виджет и основной макет
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    // Создаем область для рисования графика
    canvas = new GraphCanvas(this);
    layout->addWidget(canvas);

    // Поле ввода значения x
    inputX = new QLineEdit(this);
    inputX->setPlaceholderText("Enter value x");
    layout->addWidget(inputX);

    // Кнопка для обновления значения x
    updateButton = new QPushButton("Reload", this);
    layout->addWidget(updateButton);
    connect(updateButton, &QPushButton::clicked, this, &GraphWindow::updateXValue);

    // Выпадающий список для выбора функции
    functionSelector = new QComboBox(this);
    functionSelector->addItem("sin(x)");
    functionSelector->addItem("cos(x)");
    functionSelector->addItem("x (linear)");
    layout->addWidget(functionSelector);
    connect(functionSelector, &QComboBox::currentIndexChanged, this, &GraphWindow::updateFunction);

    // Устанавливаем параметры окна
    setCentralWidget(centralWidget);
    setWindowTitle("Graph Drawer");
    resize(800, 600);
}

GraphWindow::~GraphWindow() {
    // Все элементы удаляются автоматически
}

void GraphWindow::updateXValue() {
    // Считываем значение из поля ввода и передаем его в GraphCanvas
    bool ok;
    double newX = inputX->text().toDouble(&ok);
    if (ok) {
        canvas->setXValue(newX);
    }
    else {
        inputX->setText("");
        inputX->setPlaceholderText("Enter correct number!");
    }
}

void GraphWindow::updateFunction() {
    // Считываем выбранную функцию и передаем ее в GraphCanvas
    int functionIndex = functionSelector->currentIndex();
    canvas->setFunction(functionIndex);
}
