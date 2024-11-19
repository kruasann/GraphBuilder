#include "GraphWindow.h"

GraphWindow::GraphWindow(QWidget* parent) : QMainWindow(parent) {
    // ������� ����������� ������ � �������� �����
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    // ������� ������� ��� ��������� �������
    canvas = new GraphCanvas(this);
    layout->addWidget(canvas);

    // ������� �������� ����������: ���� ����� � ������
    inputX = new QLineEdit(this);
    inputX->setPlaceholderText("Enter Value x"); // ��������� ��� ������������
    layout->addWidget(inputX);

    updateButton = new QPushButton("Reload", this);
    layout->addWidget(updateButton);

    // ���������� ������ � ����� ��� ���������� �������� x
    connect(updateButton, &QPushButton::clicked, this, &GraphWindow::updateXValue);

    // ������������� ��������� ����
    setCentralWidget(centralWidget);
    setWindowTitle("Graph of sin(x)");
    resize(800, 600);
}

GraphWindow::~GraphWindow() {
    // ��� �������� ��������� �������������, ��� ��� ����������� � ����� ��� ��� ����������� ������
}

void GraphWindow::updateXValue() {
    // ��������� �������� �� ���� ����� � �������� ��� � GraphCanvas
    bool ok;
    double newX = inputX->text().toDouble(&ok); // ����������� ������ � �����
    if (ok) {
        canvas->setXValue(newX); // ������������� ����� �������� x
    }
    else {
        inputX->setText(""); // ������� ����, ���� ������� ������������ ��������
        inputX->setPlaceholderText("Enter Correct Number!");
    }
}
