#include "GraphWindow.h"
#include <QtCharts>
#include <QVBoxLayout>
#include <QDebug>
#include "muParser.h" // Подключаем muparser

GraphWindow::GraphWindow(QWidget* parent)
    : QMainWindow(parent), userFunction("sin(x)") {

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

    // Поле ввода пользовательской функции
    functionInput = new QLineEdit(this);
    functionInput->setPlaceholderText("Enter function: e.g., y=sin(x), y=x^2");
    functionInput->setText("y=sin(x)");
    layout->addWidget(functionInput);

    // Кнопка для построения графика
    plotButton = new QPushButton("Plot", this);
    layout->addWidget(plotButton);
    connect(plotButton, &QPushButton::clicked, this, &GraphWindow::plotGraph);

    // Устанавливаем центральный виджет
    setCentralWidget(centralWidget);
    setWindowTitle("Qt Charts Graph Viewer");
    resize(800, 600);

    // Создаем оси
    axisX = new QValueAxis();
    axisY = new QValueAxis();

    // Инициализируем график
    plotGraph();
}

GraphWindow::~GraphWindow() {
    // Все объекты автоматически удаляются
}

void GraphWindow::plotGraph() {
    userFunction = functionInput->text().trimmed();

    if (userFunction.startsWith("y=")) {
        userFunction = userFunction.mid(2).trimmed();
    }

    if (userFunction.isEmpty()) {
        qDebug() << "Function input is empty.";
        return;
    }

    // Проверяем, что выражение содержит переменную 'x'
    if (!userFunction.contains("x")) {
        qDebug() << "Function does not contain variable 'x'.";
        return;
    }

    // Обновляем график
    updateGraph();
}


void GraphWindow::updateGraph() {
    series->clear();
    chart->removeAxis(axisX);
    chart->removeAxis(axisY);
    chart->addSeries(series);

    double step = 0.1;

    QVector<QPointF> points;

    for (double x = -10; x <= 10; x += step) {
        double y = evaluateExpression(x);
        points.append(QPointF(x, y));
    }

    series->replace(points);

    axisX = new QValueAxis();
    axisX->setTitleText("X Axis");
    axisX->setRange(-10, 10);
    axisX->setTickCount(21);

    axisY = new QValueAxis();
    axisY->setTitleText("Y Axis");
    axisY->setRange(-10, 10);
    axisY->setTickCount(21);

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    series->attachAxis(axisX);
    series->attachAxis(axisY);

    chart->setTitle(QString("y=%1").arg(userFunction));
}

double GraphWindow::evaluateExpression(double x) {
    try {
        mu::Parser parser;

        parser.DefineVar("x", &x);

        std::string expr = userFunction.toUtf8().constData();

        qDebug() << "Evaluating expression:" << QString::fromStdString(expr) << "with x=" << x;

        parser.SetExpr(expr);

        double result = parser.Eval();

        return result;
    }
    catch (mu::Parser::exception_type& e) {
        qDebug() << "Error evaluating expression:" << e.GetMsg().c_str();
        return 0.0; // Ошибка: возвращаем 0
    }
}
