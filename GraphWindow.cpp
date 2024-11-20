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

    // Создаем центральный виджет
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    // Создаем график
    chart = new QChart();
    chart->setTitle("Graph Viewer");

    // Создаем серию данных
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
}

void GraphWindow::updateIntersections() {
    double xMin = axisX->min();
    double xMax = axisX->max();
    double step = (xMax - xMin) / 500;

    for (double x = xMin; x <= xMax; x += step) {
        double y = evaluateExpression(x);
        if (qFuzzyCompare(qAbs(y), 0.0)) {
            intersectionSeries->append(x, y);
        }
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

// Реализация wheelEvent
void GraphWindow::wheelEvent(QWheelEvent* event) {
    qreal factor = 1.1;
    if (event->angleDelta().y() > 0) {
        chart->zoom(factor); // Увеличиваем масштаб
    }
    else {
        chart->zoom(1 / factor); // Уменьшаем масштаб
    }
    updateGraph(); // Обновляем график для новых границ осей
    event->accept();
}

// Реализация mousePressEvent
void GraphWindow::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        lastMousePos = chartView->mapToScene(event->pos());
    }
    QMainWindow::mousePressEvent(event); // Важно вызвать базовый метод
}

QPointF GraphWindow::findClosestPoint(const QPointF& scenePos) {
    QPointF closestPoint;
    double minDistance = std::numeric_limits<double>::max();

    for (const QPointF& point : series->points()) {
        QPointF chartPoint = chart->mapToValue(point, series);
        double distance = std::hypot(chartPoint.x() - scenePos.x(), chartPoint.y() - scenePos.y());
        if (distance < minDistance) {
            minDistance = distance;
            closestPoint = point;
        }
    }

    return closestPoint;
}

void GraphWindow::mouseMoveEvent(QMouseEvent* event) {
    QPointF scenePos = chartView->mapToScene(event->pos());
    QPointF chartPos = chart->mapToValue(scenePos);

    QPointF closestPoint = findClosestPoint(chartPos);

    if (!closestPoint.isNull()) {
        hoverSeries->clear();
        hoverSeries->append(closestPoint);

        QString tooltip = QString("(%1, %2)").arg(closestPoint.x(), 0, 'f', 2).arg(closestPoint.y(), 0, 'f', 2);
        QToolTip::showText(event->globalPosition().toPoint(), tooltip, this); // Используем globalPosition()
    }
}

