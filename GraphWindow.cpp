#include "GraphWindow.h"
#include <cmath>
#include <QVBoxLayout>

GraphWindow::GraphWindow(QWidget* parent)
    : QMainWindow(parent), xValue(5.0), functionType(0) {
    // ������� ����������� ������
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    // ������� ������
    chart = new QChart();
    chart->setTitle("Graph Viewer");

    // ������� ����� ������
    series = new QLineSeries();
    chart->addSeries(series);

    // ����������� ������������� �������
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout->addWidget(chartView);

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

    // ������������� ����������� ������
    setCentralWidget(centralWidget);
    setWindowTitle("Qt Charts Graph Viewer");
    resize(800, 600);

    // ������� ���
    axisX = new QValueAxis();
    axisY = new QValueAxis();

    // ��������� ������
    updateGraph();
}

GraphWindow::~GraphWindow() {
    // ��� ������� ������������� ���������
}

void GraphWindow::updateXValue() {
    // ��������� �������� �� ���� �����
    bool ok;
    double newX = inputX->text().toDouble(&ok);
    if (ok) {
        xValue = newX;
        updateGraph();
    }
    else {
        inputX->setText("");
        inputX->setPlaceholderText("Enter correct number!");
    }
}

void GraphWindow::updateFunction() {
    // ��������� ��� �������
    functionType = functionSelector->currentIndex();
    updateGraph();
}

void GraphWindow::updateGraph() {
    // ������� ����� ������
    series->clear();

    // ������� ��� �������
    chart->removeAxis(axisX);
    chart->removeAxis(axisY);

    // ��������� ����� ������
    chart->addSeries(series);

    // ������ ��� ����������
    double step = 0.1;

    // ��������� ����� ��� �������
    for (double x = -10; x <= 10; x += step) {
        double y;
        switch (functionType) {
        case 0: y = std::sin(x); break;   // sin(x)
        case 1: y = std::cos(x); break;   // cos(x)
        case 2: y = x; break;             // �������� ������� x
        default: y = 0;
        }
        series->append(x, y);
    }

    // ������� � ����������� ��� X
    axisX = new QValueAxis();
    axisX->setTitleText("X Axis");
    axisX->setRange(-10, 10);
    axisX->setTickCount(21); // 21 ������� (�� ��������� �����������)

    // ������� � ����������� ��� Y
    axisY = new QValueAxis();
    axisY->setTitleText("Y Axis");
    axisY->setRange(-10, 10);
    axisY->setTickCount(21); // 21 �������

    // ��������� ��� � �������
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    // ����������� ��� � �����
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    // ������� �������� ������� � ����� xValue
    double yValue;
    switch (functionType) {
    case 0: yValue = std::sin(xValue); break;
    case 1: yValue = std::cos(xValue); break;
    case 2: yValue = xValue; break;
    default: yValue = 0;
    }
    chart->setTitle(QString("f(%1) = %2").arg(xValue).arg(yValue));
}
