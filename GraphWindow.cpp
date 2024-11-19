#include "GraphWindow.h"

GraphWindow::GraphWindow(QWidget* parent) : QMainWindow(parent) {
    // Создаем центральный виджет и основной макет
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    // Создаем область для рисования графика
    canvas = new GraphCanvas(this);
    layout->addWidget(canvas);

    // Создаем элементы управления: поле ввода и кнопку
    inputX = new QLineEdit(this);
    inputX->setPlaceholderText("Enter Value x"); // Подсказка для пользователя
    layout->addWidget(inputX);

    updateButton = new QPushButton("Reload", this);
    layout->addWidget(updateButton);

    // Подключаем кнопку к слоту для обновления значения x
    connect(updateButton, &QPushButton::clicked, this, &GraphWindow::updateXValue);

    // Устанавливаем параметры окна
    setCentralWidget(centralWidget);
    setWindowTitle("Graph of sin(x)");
    resize(800, 600);
}

GraphWindow::~GraphWindow() {
    // Все элементы удаляются автоматически, так как установлены в макет или как центральный виджет
}

void GraphWindow::updateXValue() {
    // Считываем значение из поля ввода и передаем его в GraphCanvas
    bool ok;
    double newX = inputX->text().toDouble(&ok); // Преобразуем строку в число
    if (ok) {
        canvas->setXValue(newX); // Устанавливаем новое значение x
    }
    else {
        inputX->setText(""); // Очищаем поле, если введено некорректное значение
        inputX->setPlaceholderText("Enter Correct Number!");
    }
}
