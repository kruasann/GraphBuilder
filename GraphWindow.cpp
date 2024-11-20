#include "GraphWindow.h"
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QtCharts/QScatterSeries>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QToolTip>
#include <QColorDialog>
#include "muParser.h"
#include <algorithm> // For std::max
#include <QListWidgetItem>

GraphWindow::GraphWindow(QWidget* parent)
    : QMainWindow(parent) {

    qRegisterMetaType<FunctionItem*>("FunctionItem*");

    // Create central widget
    QWidget* centralWidget = new QWidget(this);
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

    // Left side: function list and controls
    QWidget* leftWidget = new QWidget(this);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftWidget);

    // Function list
    functionListWidget = new QListWidget(this);
    leftLayout->addWidget(functionListWidget);

    // Add function button
    addFunctionButton = new QPushButton("Add Function", this);
    leftLayout->addWidget(addFunctionButton);
    connect(addFunctionButton, &QPushButton::clicked, this, &GraphWindow::onAddFunctionClicked);

    mainLayout->addWidget(leftWidget);

    // Right side: chart
    QWidget* rightWidget = new QWidget(this);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightWidget);

    // Create the chart
    chart = new QChart();
    chart->setTitle("Graph Viewer");

    // Create intersection and hover series
    intersectionSeries = new QScatterSeries();
    intersectionSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    intersectionSeries->setMarkerSize(10.0);
    intersectionSeries->setColor(Qt::red);

    hoverSeries = new QScatterSeries();
    hoverSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    hoverSeries->setMarkerSize(10.0);
    hoverSeries->setColor(Qt::blue);

    chart->addSeries(intersectionSeries);
    chart->addSeries(hoverSeries);

    // Create zero lines
    xAxisZeroLine = new QLineSeries();
    yAxisZeroLine = new QLineSeries();
    xAxisZeroLine->setPen(QPen(Qt::black));
    yAxisZeroLine->setPen(QPen(Qt::black));
    chart->addSeries(xAxisZeroLine);
    chart->addSeries(yAxisZeroLine);

    // Configure the chart view
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    rightLayout->addWidget(chartView);

    mainLayout->addWidget(rightWidget);

    // Set central widget
    setCentralWidget(centralWidget);
    setWindowTitle("Qt Charts Graph Viewer");
    resize(1000, 600);

    // Create axes
    axisX = new QValueAxis();
    axisX->setTitleText("X Axis");
    axisX->setRange(-10, 10);

    axisY = new QValueAxis();
    axisY->setTitleText("Y Axis");
    axisY->setRange(-10, 10);

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    intersectionSeries->attachAxis(axisX);
    intersectionSeries->attachAxis(axisY);
    hoverSeries->attachAxis(axisX);
    hoverSeries->attachAxis(axisY);
    xAxisZeroLine->attachAxis(axisX);
    xAxisZeroLine->attachAxis(axisY);
    yAxisZeroLine->attachAxis(axisX);
    yAxisZeroLine->attachAxis(axisY);

    // Initialize the update timer
    updateTimer = new QTimer(this);
    updateTimer->setSingleShot(true);
    connect(updateTimer, &QTimer::timeout, this, &GraphWindow::updateGraph);

    // Connect axis range changed signals
    connect(axisX, &QValueAxis::rangeChanged, this, &GraphWindow::onAxisRangeChanged);
    connect(axisY, &QValueAxis::rangeChanged, this, &GraphWindow::onAxisRangeChanged);

    // Connect function list item changes
    connect(functionListWidget, &QListWidget::itemChanged, this, &GraphWindow::onFunctionItemChanged);

    // Initialize with one function
    addFunction();
}

GraphWindow::~GraphWindow() {
    // Clean up function items
    qDeleteAll(functions);
}

void GraphWindow::addFunction(const QString& expression) {
    FunctionItem* func = new FunctionItem;
    func->expression = expression;
    func->color = getNextColor();
    func->series = new QLineSeries();
    func->series->setColor(func->color);
    func->visible = true;

    chart->addSeries(func->series);
    func->series->attachAxis(axisX);
    func->series->attachAxis(axisY);

    functions.append(func);

    // Add to the function list widget
    QListWidgetItem* item = new QListWidgetItem(functionListWidget);
    item->setFlags(item->flags() | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);
    item->setText(func->expression);
    item->setCheckState(Qt::Checked);
    item->setData(Qt::UserRole, QVariant::fromValue(func));

    // Set item color
    item->setForeground(func->color);
}


void GraphWindow::removeFunction(int index) {
    if (index < 0 || index >= functions.size())
        return;

    FunctionItem* func = functions.at(index);

    // Remove from chart
    chart->removeSeries(func->series);
    delete func->series;

    // Remove from list
    functions.removeAt(index);
    delete func;

    // Remove from function list widget
    delete functionListWidget->takeItem(index);
}

QColor GraphWindow::getNextColor() {
    static int colorIndex = 0;
    static QList<QColor> colors = { Qt::blue, Qt::green, Qt::magenta, Qt::cyan, Qt::darkYellow };
    QColor color = colors.at(colorIndex % colors.size());
    colorIndex++;
    return color;
}

void GraphWindow::onAddFunctionClicked() {
    addFunction();
    updateGraph();
}

void GraphWindow::onFunctionItemChanged(QListWidgetItem* item) {
    FunctionItem* func = item->data(Qt::UserRole).value<FunctionItem*>();
    if (!func) {
        qDebug() << "Error: FunctionItem pointer is null.";
        return;
    }

    func->expression = item->text();
    func->visible = (item->checkState() == Qt::Checked);

    // Show or hide the series
    func->series->setVisible(func->visible);

    updateGraph();
}

void GraphWindow::updateGraph() {
    // Clear intersection and hover series
    intersectionSeries->clear();
    hoverSeries->clear();

    // Get current axis ranges
    double xMin = axisX->min();
    double xMax = axisX->max();

    // Determine the number of points based on the current axis range
    double pixelsPerPoint = 1.0; // Adjust this value as needed
    double axisWidthInPixels = chartView->size().width();
    int numPoints = std::max(static_cast<int>((axisWidthInPixels / pixelsPerPoint)), 500);

    double step = (xMax - xMin) / numPoints;

    // Evaluate each function
    for (FunctionItem* func : functions) {
        if (!func->visible)
            continue;

        QVector<QPointF> points;

        for (double x = xMin; x <= xMax; x += step) {
            double y = evaluateExpression(func->expression, x);
            if (std::isnan(y) || std::isinf(y))
                continue;
            points.append(QPointF(x, y));
        }

        func->series->replace(points);
    }

    // Update intersections and zero lines
    updateIntersections();
    updateZeroLines();
}

void GraphWindow::updateIntersections() {
    intersectionSeries->clear();

    // Compare each pair of functions
    for (int i = 0; i < functions.size(); ++i) {
        FunctionItem* func1 = functions.at(i);
        if (!func1->visible)
            continue;

        for (int j = i + 1; j < functions.size(); ++j) {
            FunctionItem* func2 = functions.at(j);
            if (!func2->visible)
                continue;

            // Find intersections between func1 and func2
            const auto& points1 = func1->series->points();
            const auto& points2 = func2->series->points();

            int index1 = 0;
            int index2 = 0;

            while (index1 < points1.size() - 1 && index2 < points2.size() - 1) {
                double x1 = points1.at(index1).x();
                double x2 = points2.at(index2).x();

                if (std::abs(x1 - x2) < 1e-6) {
                    double y1 = points1.at(index1).y();
                    double y2 = points2.at(index2).y();
                    double y1_next = points1.at(index1 + 1).y();
                    double y2_next = points2.at(index2 + 1).y();

                    // Check if y-values cross between these points
                    if ((y1 - y2) * (y1_next - y2_next) < 0) {
                        // Estimate intersection point
                        double t = (y2 - y1) / ((y1_next - y1) - (y2_next - y2));
                        double xIntersect = x1 + t * (points1.at(index1 + 1).x() - x1);
                        double yIntersect = y1 + t * (y1_next - y1);

                        intersectionSeries->append(xIntersect, yIntersect);
                    }

                    index1++;
                    index2++;
                }
                else if (x1 < x2) {
                    index1++;
                }
                else {
                    index2++;
                }
            }
        }
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

double GraphWindow::evaluateExpression(const QString& expression, double x) {
    try {
        mu::Parser parser;
        parser.DefineVar("x", &x);
        parser.SetExpr(expression.toUtf8().constData());
        return parser.Eval();
    }
    catch (mu::Parser::exception_type& e) {
        // qDebug() << "Error evaluating expression:" << e.GetMsg().c_str();
        return std::numeric_limits<double>::quiet_NaN();
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

    // Iterate over all functions
    for (FunctionItem* func : functions) {
        if (!func->visible)
            continue;

        for (const QPointF& point : func->series->points()) {
            double distance = std::hypot(point.x() - chartPos.x(), point.y() - chartPos.y());
            if (distance < minDistance) {
                minDistance = distance;
                closestPoint = point;
            }
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

void GraphWindow::onAxisRangeChanged() {
    // Start or restart the timer
    updateTimer->start(updateInterval);
}
