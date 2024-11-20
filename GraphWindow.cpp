#include "GraphWindow.h"
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QtCharts/QScatterSeries>
#include <QVBoxLayout>
#include <QDebug>
#include <QToolTip>
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
    intersectionSeries = new QScatterSeries();
    intersectionSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    intersectionSeries->setMarkerSize(10.0);
    intersectionSeries->setColor(Qt::red);

    hoverSeries = new QScatterSeries();
    hoverSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    hoverSeries->setMarkerSize(10.0);
    hoverSeries->setColor(Qt::blue);

    chart->addSeries(series);
    chart->addSeries(intersectionSeries);
    chart->addSeries(hoverSeries);

    // ������� ������� �����
    xAxisZeroLine = new QLineSeries();
    yAxisZeroLine = new QLineSeries();
    xAxisZeroLine->setPen(QPen(Qt::black));
    yAxisZeroLine->setPen(QPen(Qt::black));
    chart->addSeries(xAxisZeroLine);
    chart->addSeries(yAxisZeroLine);

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
    intersectionSeries->attachAxis(axisX);
    intersectionSeries->attachAxis(axisY);
    hoverSeries->attachAxis(axisX);
    hoverSeries->attachAxis(axisY);
    xAxisZeroLine->attachAxis(axisX);
    xAxisZeroLine->attachAxis(axisY);
    yAxisZeroLine->attachAxis(axisX);
    yAxisZeroLine->attachAxis(axisY);

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

    if (!userFunction.contains("x")) {
        qDebug() << "Function does not contain variable 'x'.";
        return;
    }

    updateGraph();
}

void GraphWindow::updateGraph() {
    if (chart->series().contains(series)) {
        chart->removeSeries(series);
    }

    series->clear();
    intersectionSeries->clear();
    hoverSeries->clear();

    double xMin = axisX->min();
    double xMax = axisX->max();
    double step = (xMax - xMin) / 500;

    QVector<QPointF> points;

    for (double x = xMin; x <= xMax; x += step) {
        double y = evaluateExpression(x);
        points.append(QPointF(x, y));
    }

    series->replace(points);
    chart->addSeries(series);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    updateIntersections();
    updateZeroLines();
}

void GraphWindow::updateIntersections() {
    double xMin = axisX->min();
    double xMax = axisX->max();
    int numSteps = 500;
    double step = (xMax - xMin) / numSteps;

    double prevX = xMin;
    double prevY = evaluateExpression(prevX);

    for (int i = 1; i <= numSteps; ++i) {
        double x = xMin + i * step;
        double y = evaluateExpression(x);

        if ((prevY <= 0 && y >= 0) || (prevY >= 0 && y <= 0)) {
            // Estimate zero crossing
            double xZero = prevX - prevY * (x - prevX) / (y - prevY);
            double yZero = 0.0;
            intersectionSeries->append(xZero, yZero);
        }

        prevX = x;
        prevY = y;
    }
}

void GraphWindow::updateZeroLines() {
    double xMin = axisX->min();
    double xMax = axisX->max();
    double yMin = axisY->min();
    double yMax = axisY->max();

    xAxisZeroLine->clear();
    yAxisZeroLine->clear();

    if (yMin <= 0 && yMax >= 0) {
        xAxisZeroLine->append(xMin, 0);
        xAxisZeroLine->append(xMax, 0);
    }

    if (xMin <= 0 && xMax >= 0) {
        yAxisZeroLine->append(0, yMin);
        yAxisZeroLine->append(0, yMax);
    }
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
        chart->zoom(factor);
    }
    else {
        chart->zoom(1 / factor);
    }
    updateZeroLines(); // Update zero lines after zooming
    event->accept();
}

void GraphWindow::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        isLeftMousePressed = true;
        lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }
    QMainWindow::mousePressEvent(event);
}

QPointF GraphWindow::findClosestPoint(const QPointF& chartPos) {
    QPointF closestPoint;
    double minDistance = std::numeric_limits<double>::max();

    for (const QPointF& point : series->points()) {
        double distance = std::hypot(point.x() - chartPos.x(), point.y() - chartPos.y());
        if (distance < minDistance) {
            minDistance = distance;
            closestPoint = point;
        }
    }

    return closestPoint;
}

void GraphWindow::mouseMoveEvent(QMouseEvent* event) {
    if (isLeftMousePressed) {
        QPointF delta = chart->mapToValue(chartView->mapToScene(lastMousePos)) - chart->mapToValue(chartView->mapToScene(event->pos()));
        axisX->setRange(axisX->min() + delta.x(), axisX->max() + delta.x());
        axisY->setRange(axisY->min() + delta.y(), axisY->max() + delta.y());
        lastMousePos = event->pos();
        updateZeroLines(); // Update zero lines after panning
        event->accept();
    }
    else {
        QPointF scenePos = chartView->mapToScene(event->pos());
        QPointF chartPos = chart->mapToValue(scenePos);

        QPointF closestPoint = findClosestPoint(chartPos);

        if (!closestPoint.isNull()) {
            hoverSeries->clear();
            hoverSeries->append(closestPoint);

            QString tooltip = QString("(%1, %2)").arg(closestPoint.x(), 0, 'f', 2).arg(closestPoint.y(), 0, 'f', 2);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            QToolTip::showText(event->globalPosition().toPoint(), tooltip, this);
#else
            QToolTip::showText(event->globalPos(), tooltip, this);
#endif
        }
    }
    QMainWindow::mouseMoveEvent(event);
}

void GraphWindow::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        isLeftMousePressed = false;
        setCursor(Qt::ArrowCursor);
    }
    QMainWindow::mouseReleaseEvent(event);
}
