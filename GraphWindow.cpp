#include "GraphWindow.h"
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QVBoxLayout>
#include <QDebug>
#include "muParser.h"

GraphWindow::GraphWindow(QWidget* parent)
    : QMainWindow(parent), userFunction("sin(x)") {

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

    // ���� ����� ���������������� �������
    functionInput = new QLineEdit(this);
    functionInput->setPlaceholderText("Enter function: e.g., y=sin(x), y=x^2");
    functionInput->setText("y=sin(x)");
    layout->addWidget(functionInput);

    // ������ ��� ���������� �������
    plotButton = new QPushButton("Plot", this);
    layout->addWidget(plotButton);
    connect(plotButton, &QPushButton::clicked, this, &GraphWindow::plotGraph);

    // ������������� ����������� ������
    setCentralWidget(centralWidget);
    setWindowTitle("Qt Charts Graph Viewer");
    resize(800, 600);

    // ������� ���
    axisX = new QValueAxis();
    axisX->setTitleText("X Axis");
    axisX->setRange(-10, 10);

    axisY = new QValueAxis();
    axisY->setTitleText("Y Axis");
    axisY->setRange(-10, 10);

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    series->attachAxis(axisX);
    series->attachAxis(axisY);

    // �������������� ������
    plotGraph();
}

GraphWindow::~GraphWindow() {
    // ��� ������� ������������� ���������
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

    // ���������, ��� ��������� �������� ���������� 'x'
    if (!userFunction.contains("x")) {
        qDebug() << "Function does not contain variable 'x'.";
        return;
    }

    // ��������� ������
    updateGraph();
}

void GraphWindow::updateGraph() {
    // ������� ������� �����, ���� ��� ����
    if (chart->series().contains(series)) {
        chart->removeSeries(series);
    }

    series->clear(); // ������� ������ �����

    // �������� ������� ������� ��� X
    double xMin = axisX->min();
    double xMax = axisX->max();

    // ������ ���������� ��� ��� ���������� ����� �������
    double step = (xMax - xMin) / 500; // 500 ����� �� ������� �������

    QVector<QPointF> points;

    // ��������� ����� ������� � ������� �������
    for (double x = xMin; x <= xMax; x += step) {
        double y = evaluateExpression(x);
        points.append(QPointF(x, y));
    }

    // ��������� ����� � �����
    series->replace(points);

    // ��������� ����� ������� � ������
    chart->addSeries(series);

    // ����������� ���
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    // ������������� �������� �������
    chart->setTitle(QString("y=%1").arg(userFunction));
}

double GraphWindow::evaluateExpression(double x) {
    try {
        mu::Parser parser;
        parser.DefineVar("x", &x);
        parser.SetExpr(userFunction.toUtf8().constData());
        return parser.Eval();
    }
    catch (mu::Parser::exception_type& e) {
        qDebug() << "Error evaluating expression:" << e.GetMsg().c_str();
        return 0.0;
    }
}

void GraphWindow::wheelEvent(QWheelEvent* event) {
    qreal factor = 1.1;
    if (event->angleDelta().y() > 0) {
        chart->zoom(factor); // ����������� �������
    }
    else {
        chart->zoom(1 / factor); // ��������� �������
    }
    updateGraph(); // ��������� ������ ��� ����� ������ ����
    event->accept();
}

void GraphWindow::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        lastMousePos = chartView->mapToScene(event->pos());
    }
}

void GraphWindow::mouseMoveEvent(QMouseEvent* event) {
    if (event->buttons() & Qt::LeftButton) {
        QPointF delta = chartView->mapToScene(event->pos()) - lastMousePos;

        double dx = -delta.x() * (axisX->max() - axisX->min()) / chart->plotArea().width();
        double dy = delta.y() * (axisY->max() - axisY->min()) / chart->plotArea().height();

        axisX->setRange(axisX->min() + dx, axisX->max() + dx);
        axisY->setRange(axisY->min() + dy, axisY->max() + dy);

        updateGraph(); // ��������� ������

        lastMousePos = chartView->mapToScene(event->pos());
    }
}
