#include "GraphWindow.h"

#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QToolTip>
#include <QColorDialog>
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QMessageBox>

#include "muParser.h"

#include <algorithm> // ��� std::max

GraphWindow::GraphWindow(QWidget* parent)
    : QMainWindow(parent) {

    qRegisterMetaType<FunctionItem*>("FunctionItem*");

    // ������� ����������� ������ � �������� �����
    QWidget* centralWidget = new QWidget(this);
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

    // ����� �����: ������ ������� � ������ ����������
    QWidget* leftWidget = new QWidget(this);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftWidget);

    // ������ �������
    functionListWidget = new QListWidget(this);
    leftLayout->addWidget(functionListWidget);

    // ������ ���������� �������
    addFunctionButton = new QPushButton("Add Function", this);
    leftLayout->addWidget(addFunctionButton);
    connect(addFunctionButton, &QPushButton::clicked, this, &GraphWindow::onAddFunctionClicked);

    mainLayout->addWidget(leftWidget);

    // ������ �����: ������
    QWidget* rightWidget = new QWidget(this);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightWidget);

    // ������� ������
    chart = new QChart();
    chart->setTitle("Graph Viewer");

    // ������� ����� ��� ����������� � ���������
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

    // ������� ������� ����� ����
    xAxisZeroLine = new QLineSeries();
    yAxisZeroLine = new QLineSeries();
    xAxisZeroLine->setPen(QPen(Qt::black));
    yAxisZeroLine->setPen(QPen(Qt::black));
    chart->addSeries(xAxisZeroLine);
    chart->addSeries(yAxisZeroLine);

    // ����������� ����������� �������
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    rightLayout->addWidget(chartView);

    mainLayout->addWidget(rightWidget);

    // ������������� ����������� ������
    setCentralWidget(centralWidget);
    setWindowTitle("Qt Charts Graph Viewer");
    resize(1000, 600);

    // ������� ���
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

    // �������������� ������ ����������
    updateTimer = new QTimer(this);
    updateTimer->setSingleShot(true);
    connect(updateTimer, &QTimer::timeout, this, &GraphWindow::updateGraph);

    // ���������� ������� ��������� ��������� ����
    connect(axisX, &QValueAxis::rangeChanged, this, &GraphWindow::onAxisRangeChanged);
    connect(axisY, &QValueAxis::rangeChanged, this, &GraphWindow::onAxisRangeChanged);

    // ���������� ��������� � ������ �������
    connect(functionListWidget, &QListWidget::itemChanged, this, &GraphWindow::onFunctionItemChanged);

    // �������������� � ����� ��������
    addFunction();
}

GraphWindow::~GraphWindow() {
    // ������� �������
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

    // ��������� ������� � ������ ������� � ���������� ����������
    QListWidgetItem* item = new QListWidgetItem(functionListWidget);
    item->setFlags(item->flags() | Qt::ItemIsSelectable);
    item->setData(Qt::UserRole, QVariant::fromValue(func));

    QWidget* itemWidget = new QWidget();
    QHBoxLayout* itemLayout = new QHBoxLayout(itemWidget);
    itemLayout->setContentsMargins(0, 0, 0, 0);

    // ������� ��� ��������� �������
    QCheckBox* checkBox = new QCheckBox();
    checkBox->setChecked(true);

    // ���� ����� ��� ��������� �������
    QLineEdit* expressionEdit = new QLineEdit(func->expression);

    // ������ ������ �����
    QPushButton* colorButton = new QPushButton();
    colorButton->setFixedSize(20, 20);
    colorButton->setStyleSheet(QString("background-color: %1").arg(func->color.name()));

    // ������ �������� �������
    QPushButton* removeButton = new QPushButton("X");
    removeButton->setFixedSize(20, 20);

    itemLayout->addWidget(checkBox);
    itemLayout->addWidget(expressionEdit);
    itemLayout->addWidget(colorButton);
    itemLayout->addWidget(removeButton);

    item->setSizeHint(itemWidget->sizeHint());
    functionListWidget->setItemWidget(item, itemWidget);

    // ���������� ������� � ��������� ���������� �������
    connect(checkBox, &QCheckBox::stateChanged, [this, func](int state) {
        func->visible = (state == Qt::Checked);
        func->series->setVisible(func->visible);
        updateGraph();
        });

    connect(expressionEdit, &QLineEdit::editingFinished, [this, func, expressionEdit]() {
        func->expression = expressionEdit->text();
        updateGraph();
        });

    connect(colorButton, &QPushButton::clicked, [this, func, colorButton]() {
        QColor color = QColorDialog::getColor(func->color, this, "Select Function Color");
        if (color.isValid()) {
            func->color = color;
            func->series->setColor(color);
            colorButton->setStyleSheet(QString("background-color: %1").arg(color.name()));
        }
        });

    connect(removeButton, &QPushButton::clicked, [this, func]() {
        int index = functions.indexOf(func);
        if (index != -1) {
            removeFunction(index);
        }
        });
}

void GraphWindow::removeFunction(int index) {
    if (index < 0 || index >= functions.size())
        return;

    FunctionItem* func = functions.at(index);

    // ������� ����� � �������
    chart->removeSeries(func->series);
    delete func->series;

    // ������� �� ������ �������
    functions.removeAt(index);
    delete func;

    // ������� ������� �� ������ �������
    delete functionListWidget->takeItem(index);

    updateGraph();
}

QColor GraphWindow::getNextColor() {
    static int colorIndex = 0;
    static QList<QColor> colors = {
        Qt::blue, Qt::green, Qt::magenta, Qt::cyan, Qt::darkYellow,
        Qt::darkRed, Qt::darkCyan, Qt::darkMagenta, Qt::darkGreen
    };
    QColor color = colors.at(colorIndex % colors.size());
    colorIndex++;
    return color;
}

void GraphWindow::onAddFunctionClicked() {
    addFunction();
    updateGraph();
}

void GraphWindow::onFunctionItemChanged(QListWidgetItem* item) {
    // ���������� ��������� � �������� ������ �������
    FunctionItem* func = item->data(Qt::UserRole).value<FunctionItem*>();
    if (!func) {
        qDebug() << "Error: FunctionItem pointer is null.";
        return;
    }

    // ��������� ��������� ������� � �������
    updateGraph();
}

void GraphWindow::updateGraph() {
    // ������� ����� ����������� � ���������
    intersectionSeries->clear();
    hoverSeries->clear();

    // �������� ������� ��������� ����
    double xMin = axisX->min();
    double xMax = axisX->max();

    // ���������� ���������� ����� �� ������ �������� ��������� ��� X
    double pixelsPerPoint = 1.0; // ����� ��������� ��� �����������
    double axisWidthInPixels = chartView->size().width();
    int numPoints = std::max(static_cast<int>((axisWidthInPixels / pixelsPerPoint)), 500);

    double step = (xMax - xMin) / numPoints;

    // ���� ��� ������������ ������ ������� �������
    bool parsingSuccess = true;

    // ��������� �������� ������ �������
    for (FunctionItem* func : functions) {
        if (!func->visible)
            continue;

        QVector<QPointF> points;
        bool funcParsingSuccess = true;

        for (double x = xMin; x <= xMax; x += step) {
            bool ok;
            double y = evaluateExpression(func->expression, x, ok);
            if (!ok) {
                funcParsingSuccess = false;
                break;
            }
            if (std::isnan(y) || std::isinf(y))
                continue;
            points.append(QPointF(x, y));
        }

        int index = functions.indexOf(func);
        QListWidgetItem* item = functionListWidget->item(index);

        if (!funcParsingSuccess) {
            parsingSuccess = false;
            // �������� ������� ������� ������� ������
            item->setBackground(Qt::red);
        }
        else {
            // ���������� ����������� ���
            item->setBackground(Qt::white);
        }

        func->series->replace(points);
    }

    if (!parsingSuccess) {
        QMessageBox::warning(this, "Parsing Error", "One or more functions could not be parsed. Please check your expressions.");
    }

    // ��������� ����������� � ������� �����
    updateIntersections();
    updateZeroLines();
}

void GraphWindow::updateIntersections() {
    intersectionSeries->clear();

    // ��� ������ ���� �������
    for (int i = 0; i < functions.size(); ++i) {
        FunctionItem* func1 = functions.at(i);
        if (!func1->visible)
            continue;

        for (int j = i + 1; j < functions.size(); ++j) {
            FunctionItem* func2 = functions.at(j);
            if (!func2->visible)
                continue;

            // ������������, ��� ��� ������� ����� ���������� ���������� �����
            const auto& points1 = func1->series->points();
            const auto& points2 = func2->series->points();
            int numPoints = std::min(points1.size(), points2.size());

            for (int k = 0; k < numPoints - 1; ++k) {
                double x1 = points1.at(k).x();
                double y1 = points1.at(k).y();
                double y2 = points2.at(k).y();

                double x2 = points1.at(k + 1).x();
                double y1_next = points1.at(k + 1).y();
                double y2_next = points2.at(k + 1).y();

                // ���������, ������������ �� ������� ����� ����� �������
                double f1 = y1 - y2;
                double f2 = y1_next - y2_next;
                if (f1 * f2 < 0) {
                    // ��������� ����� �����������
                    double t = f1 / (f1 - f2);
                    double xIntersect = x1 + t * (x2 - x1);
                    double yIntersect = y1 + t * (y1_next - y1);
                    intersectionSeries->append(xIntersect, yIntersect);
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

double GraphWindow::evaluateExpression(const QString& expression, double x, bool& ok) {
    try {
        mu::Parser parser;
        parser.DefineVar("x", &x);
        parser.SetExpr(expression.toUtf8().constData());
        double result = parser.Eval();
        ok = true;
        return result;
    }
    catch (mu::Parser::exception_type& e) {
        // qDebug() << "Error evaluating expression:" << e.GetMsg().c_str();
        ok = false;
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

    // �������� �� ���� ��������
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
    // ��������� ��� ������������� ������ ����������
    updateTimer->start(updateInterval);
}
