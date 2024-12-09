// GraphWindow.cpp
#include "GraphWindow.h"
#include "CustomChartView.h"

#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QToolTip>
#include <QColorDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QMessageBox>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QListWidgetItem>
#include <QContextMenuEvent>
#include <algorithm> // ��� std::max

GraphWindow::GraphWindow(QWidget* parent)
    : QMainWindow(parent) {

    setWindowIcon(QIcon(":/images/GraphBuilderLogo.png"));

    qRegisterMetaType<FunctionItem*>("FunctionItem*");

    // �������� QSplitter � ��������� ��� ��� ������������ �������
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);

    // ����� �����: ������ ������� � ������ ����������
    QWidget* leftWidget = new QWidget();
    QVBoxLayout* leftLayout = new QVBoxLayout(leftWidget);

    // ������ �������
    functionListWidget = new QListWidget(this);
    functionListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    functionListWidget->setDragDropMode(QAbstractItemView::InternalMove);
    functionListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    leftLayout->addWidget(functionListWidget);

    // ����������� �������� ��� ������������ ���� � ��������������
    connect(functionListWidget, &QListWidget::customContextMenuRequested, this, &GraphWindow::onFunctionListContextMenu);
    connect(functionListWidget, &QListWidget::itemChanged, this, &GraphWindow::onFunctionItemChanged);
    connect(functionListWidget->model(), &QAbstractItemModel::rowsMoved, this, &GraphWindow::updateGraph);

    // ������ ���������� �������
    addFunctionButton = new QPushButton(QIcon(":/icons/add.png"), tr("Add function"), this);
    leftLayout->addWidget(addFunctionButton);
    connect(addFunctionButton, &QPushButton::clicked, this, &GraphWindow::onAddFunctionClicked);

    // ������ ���������� �������
    saveGraphButton = new QPushButton(QIcon(":/icons/save.png"), tr("Save plot"), this);
    leftLayout->addWidget(saveGraphButton);
    connect(saveGraphButton, &QPushButton::clicked, this, &GraphWindow::saveGraph);

    // ��������� ��������, ����� ������ �������� ���������� ����������� ������������
    leftLayout->addStretch();

    // ������������� �������� �������� ��� ����� ������
    leftWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    splitter->addWidget(leftWidget);

    // ������ �����: ������
    QWidget* rightWidget = new QWidget();
    QVBoxLayout* rightLayout = new QVBoxLayout(rightWidget);

    // ������� ������
    chart = new QChart();
    chart->setTitle(tr("Plot Viewer"));

    // ������� ����� ��� ����������� � ���������
    intersectionSeries = new QScatterSeries();
    intersectionSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    intersectionSeries->setMarkerSize(10.0);
    intersectionSeries->setColor(Qt::red);
    intersectionSeries->setName(tr("Intersections"));

    hoverSeries = new QScatterSeries();
    hoverSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    hoverSeries->setMarkerSize(10.0);
    hoverSeries->setColor(Qt::blue);
    hoverSeries->setName(tr("Hovers"));

    chart->addSeries(intersectionSeries);
    chart->addSeries(hoverSeries);

    // ������� ������� ����� ����
    xAxisZeroLine = new QLineSeries();
    yAxisZeroLine = new QLineSeries();
    xAxisZeroLine->setPen(QPen(Qt::black, 1, Qt::DashLine));
    yAxisZeroLine->setPen(QPen(Qt::black, 1, Qt::DashLine));
    xAxisZeroLine->setName(tr("X = 0"));
    yAxisZeroLine->setName(tr("Y = 0"));
    chart->addSeries(xAxisZeroLine);
    chart->addSeries(yAxisZeroLine);

    // ����������� ����������� �������
    chartView = new CustomChartView(chart, this);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    rightLayout->addWidget(chartView);

    // ������������� �������� �������� ��� ������ ������
    rightWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    splitter->addWidget(rightWidget);

    // ������������� ������������ ����������: ����� ������ �������� 20%, ������ � 80%
    splitter->setStretchFactor(0, 1); // ����� ������
    splitter->setStretchFactor(1, 4); // ������ ������ (������)

    // ������������� ����������� ������
    setCentralWidget(splitter);
    setWindowTitle(tr("Plot Viewer Qt Charts"));
    resize(1200, 700);

    // ������� ���
    axisX = new QValueAxis();
    axisX->setTitleText(tr("X Axis"));
    axisX->setRange(-10, 10);

    axisY = new QValueAxis();
    axisY->setTitleText(tr("Y Axis"));
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

    legend = chart->legend();
    legend->setVisible(true);
    legend->setAlignment(Qt::AlignBottom);

    // �������������� ������ ����������
    updateTimer = new QTimer(this);
    updateTimer->setSingleShot(true);
    connect(updateTimer, &QTimer::timeout, this, &GraphWindow::updateGraph);

    // ���������� ������� ��������� ��������� ����
    connect(axisX, &QValueAxis::rangeChanged, this, &GraphWindow::onAxisRangeChanged);
    connect(axisY, &QValueAxis::rangeChanged, this, &GraphWindow::onAxisRangeChanged);

    // �������� ���� � ������ ������������
    createMenus();
    createToolBar();

    // ���������� ������ �� CustomChartView ��� ���������� ��������� ����
    connect(chartView, &CustomChartView::mouseMoved, this, &GraphWindow::updateMouseCoordinates);

    // �������������� � ����� ��������
    addFunction();
}

GraphWindow::~GraphWindow() {
    // ������� �������
    qDeleteAll(functions);
}

void GraphWindow::createMenus() {
    // ���� ��� ������� � main.cpp, �������������� ���� ����� �������� ����� ��� �������������
}

void GraphWindow::createToolBar() {
    // ������ ������������ ��� ������� � ������������
}

void GraphWindow::addFunction(const QString& expression) {
    FunctionItem* func = new FunctionItem;
    func->expression = expression;
    func->color = getNextColor();
    func->series = new QLineSeries();
    func->series->setColor(func->color);
    func->visible = true;

    func->series->setName(func->expression); // ������������� ��� �����

    chart->addSeries(func->series);
    func->series->attachAxis(axisX);
    func->series->attachAxis(axisY);

    functions.append(func);

    // ��������� ������� � ������ ������� � ���������� ����������
    QListWidgetItem* item = new QListWidgetItem(functionListWidget);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    item->setCheckState(Qt::Checked);
    item->setData(Qt::UserRole, QVariant::fromValue(func));
    item->setText(func->expression);

    // ��������� ������ �����
    QPixmap colorPixmap(20, 20);
    colorPixmap.fill(func->color);
    item->setIcon(QIcon(colorPixmap));
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
        Qt::blue, Qt::green, Qt::magenta, Qt::cyan, QColor("#FFA500"), // ���������
        Qt::darkRed, Qt::darkCyan, Qt::darkMagenta, Qt::darkGreen
    };
    QColor color = colors.at(colorIndex % colors.size());
    colorIndex++;
    return color;
}

void GraphWindow::onAddFunctionClicked() {
    bool ok;
    QString expression = QInputDialog::getText(this, tr("Add function"),
        tr("Enter function expression:"), QLineEdit::Normal,
        "sin(x)", &ok);
    if (ok && !expression.isEmpty()) {
        addFunction(expression);
        updateGraph();
    }
}

void GraphWindow::onRemoveSelectedFunction() {
    QListWidgetItem* selectedItem = functionListWidget->currentItem();
    if (selectedItem) {
        int index = functionListWidget->row(selectedItem);
        removeFunction(index);
    }
    else {
        QMessageBox::information(this, tr("Deleting function"), tr("Please select the function to delete."));
    }
}

void GraphWindow::onFunctionItemChanged(QListWidgetItem* item) {
    // ���������� ��������� � �������� ������ �������
    FunctionItem* func = item->data(Qt::UserRole).value<FunctionItem*>();
    if (!func) {
        qDebug() << "Error: FunctionItem pointer is null.";
        return;
    }

    func->visible = (item->checkState() == Qt::Checked);
    func->series->setVisible(func->visible);
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
            item->setBackground(QColor("#FFCCCC"));
        }
        else {
            // ���������� ����������� ���
            item->setBackground(Qt::white);
        }

        func->series->replace(points);
    }

    if (!parsingSuccess) {
        QMessageBox::warning(this, tr("Parsing error"), tr("One or more functions could not be parsed. Please check your expressions."));
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

void GraphWindow::updateMouseCoordinates(double x, double y) {
    // ��������� ��������� ������ � �������� ������������ ����
    statusBar()->showMessage(QString("X: %1, Y: %2").arg(x, 0, 'f', 2).arg(y, 0, 'f', 2));
}

void GraphWindow::onAxisRangeChanged() {
    // ��������� ��� ������������� ������ ����������
    updateTimer->start(updateInterval);
}

void GraphWindow::saveGraph() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save plot"), "", "Image PNG (*.png);;Image JPEG (*.jpg)");
    if (fileName.isEmpty())
        return;
    else {
        QPixmap pixmap = chartView->grab();
        pixmap.save(fileName);
    }
}

void GraphWindow::onFunctionListContextMenu(const QPoint& pos) {
    QListWidgetItem* item = functionListWidget->itemAt(pos);
    if (item) {
        QMenu contextMenu(this);
        QAction* removeAction = contextMenu.addAction(tr("Delete function"));
        QAction* selectedAction = contextMenu.exec(functionListWidget->viewport()->mapToGlobal(pos));
        if (selectedAction == removeAction) {
            int index = functionListWidget->row(item);
            removeFunction(index);
        }
    }
}
