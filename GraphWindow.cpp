#include "GraphWindow.h"

GraphWindow::GraphWindow(QWidget* parent) : QMainWindow(parent) {
    // ������� ����������� ������ � �������� �����
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    // ������� ������� ��� ��������� �������
    canvas = new GraphCanvas(this);
    layout->addWidget(canvas);

    // ���� ����� �������� x
    inputX = new QLineEdit(this);
    inputX->setPlaceholderText("Enter value x");
    layout->addWidget(inputX);

    // ������ ��� ���������� �������� x
    updateButton = new QPushButton("Reload", this);
    layout->addWidget(updateButton);
    connect(updateButton, &QPushButton::clicked, this, &GraphWindow::updateXValue);

    // ���������� ������ ��� ������ �������
    functionSelector = new QComboBox(this);
    functionSelector->addItem("sin(x)");
    functionSelector->addItem("cos(x)");
    functionSelector->addItem("x (linear)");
    layout->addWidget(functionSelector);
    connect(functionSelector, &QComboBox::currentIndexChanged, this, &GraphWindow::updateFunction);

    // ������������� ��������� ����
    setCentralWidget(centralWidget);
    setWindowTitle("Graph Drawer");
    resize(800, 600);
}

GraphWindow::~GraphWindow() {
    // ��� �������� ��������� �������������
}

void GraphWindow::updateXValue() {
    // ��������� �������� �� ���� ����� � �������� ��� � GraphCanvas
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
    // ��������� ��������� ������� � �������� �� � GraphCanvas
    int functionIndex = functionSelector->currentIndex();
    canvas->setFunction(functionIndex);
}
