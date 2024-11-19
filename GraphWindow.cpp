#include "GraphWindow.h"
#include <cmath>
#include <QVBoxLayout>

GraphWindow::GraphWindow(QWidget* parent)
    : QMainWindow(parent), xValue(5.0), functionType(0) {
    // Создаем центральный виджет
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    // Создаем график
    chart = new QChart();
    chart->setTitle("Graph Viewer");

    // Создаем серию данных
    series = new QLineSeries();
    chart->addSeries(series);

    // Настраиваем представление графика
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout->addWidget(chartView);

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

    // Устанавливаем центральный виджет
    setCentralWidget(centralWidget);
    setWindowTitle("Qt Charts Graph Viewer");
    resize(800, 600);

    // Создаем оси
    axisX = new QValueAxis();
    axisY = new QValueAxis();

    // Обновляем график
    updateGraph();
}

GraphWindow::~GraphWindow() {
    // Все объекты автоматически удаляются
}

void GraphWindow::updateXValue() {
    // Считываем значение из поля ввода
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
    // Обновляем тип функции
    functionType = functionSelector->currentIndex();
    updateGraph();
}

void GraphWindow::updateGraph() {
    // Очищаем серию данных
    series->clear();

    // Очищаем оси графика
    chart->removeAxis(axisX);
    chart->removeAxis(axisY);

    // Добавляем серию данных
    chart->addSeries(series);

    // Задаем шаг построения
    double step = 0.1;

    // Добавляем точки для графика
    for (double x = -10; x <= 10; x += step) {
        double y;
        switch (functionType) {
        case 0: y = std::sin(x); break;   // sin(x)
        case 1: y = std::cos(x); break;   // cos(x)
        case 2: y = x; break;             // Линейная функция x
        default: y = 0;
        }
        series->append(x, y);
    }

    // Создаем и настраиваем ось X
    axisX = new QValueAxis();
    axisX->setTitleText("X Axis");
    axisX->setRange(-10, 10);
    axisX->setTickCount(21); // 21 отметка (по умолчанию равномерные)

    // Создаем и настраиваем ось Y
    axisY = new QValueAxis();
    axisY->setTitleText("Y Axis");
    axisY->setRange(-10, 10);
    axisY->setTickCount(21); // 21 отметка

    // Добавляем оси к графику
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    // Привязываем оси к серии
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    // Выводим значение функции в точке xValue
    double yValue;
    switch (functionType) {
    case 0: yValue = std::sin(xValue); break;
    case 1: yValue = std::cos(xValue); break;
    case 2: yValue = xValue; break;
    default: yValue = 0;
    }
    chart->setTitle(QString("f(%1) = %2").arg(xValue).arg(yValue));
}
