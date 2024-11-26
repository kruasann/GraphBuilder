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

#include <algorithm> // Для std::max

GraphWindow::GraphWindow(QWidget* parent)
    : QMainWindow(parent) {

    qRegisterMetaType<FunctionItem*>("FunctionItem*");

    // Создаем центральный виджет и основной макет
    QWidget* centralWidget = new QWidget(this);
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

    // Левая часть: список функций и кнопки управления
    QWidget* leftWidget = new QWidget(this);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftWidget);

    // Список функций
    functionListWidget = new QListWidget(this);
    leftLayout->addWidget(functionListWidget);

    // Кнопка добавления функции
    addFunctionButton = new QPushButton("Add Function", this);
    leftLayout->addWidget(addFunctionButton);
    connect(addFunctionButton, &QPushButton::clicked, this, &GraphWindow::onAddFunctionClicked);

    mainLayout->addWidget(leftWidget);

    // Правая часть: график
    QWidget* rightWidget = new QWidget(this);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightWidget);

    // Создаем график
    chart = new QChart();
    chart->setTitle("Graph Viewer");

    // Создаем серии для пересечений и наведения
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

    // Создаем нулевые линии осей
    xAxisZeroLine = new QLineSeries();
    yAxisZeroLine = new QLineSeries();
    xAxisZeroLine->setPen(QPen(Qt::black));
    yAxisZeroLine->setPen(QPen(Qt::black));
    chart->addSeries(xAxisZeroLine);
    chart->addSeries(yAxisZeroLine);

    // Настраиваем отображение графика
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    rightLayout->addWidget(chartView);

    mainLayout->addWidget(rightWidget);

    // Устанавливаем центральный виджет
    setCentralWidget(centralWidget);
    setWindowTitle("Qt Charts Graph Viewer");
    resize(1000, 600);

    // Создаем оси
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

    // Инициализируем таймер обновления
    updateTimer = new QTimer(this);
    updateTimer->setSingleShot(true);
    connect(updateTimer, &QTimer::timeout, this, &GraphWindow::updateGraph);

    // Подключаем сигналы изменения диапазона осей
    connect(axisX, &QValueAxis::rangeChanged, this, &GraphWindow::onAxisRangeChanged);
    connect(axisY, &QValueAxis::rangeChanged, this, &GraphWindow::onAxisRangeChanged);

    // Подключаем изменения в списке функций
    connect(functionListWidget, &QListWidget::itemChanged, this, &GraphWindow::onFunctionItemChanged);

    // Инициализируем с одной функцией
    addFunction();
}

GraphWindow::~GraphWindow() {
    // Очищаем функции
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

    // Добавляем функцию в список функций с элементами управления
    QListWidgetItem* item = new QListWidgetItem(functionListWidget);
    item->setFlags(item->flags() | Qt::ItemIsSelectable);
    item->setData(Qt::UserRole, QVariant::fromValue(func));

    QWidget* itemWidget = new QWidget();
    QHBoxLayout* itemLayout = new QHBoxLayout(itemWidget);
    itemLayout->setContentsMargins(0, 0, 0, 0);

    // Чекбокс для видимости функции
    QCheckBox* checkBox = new QCheckBox();
    checkBox->setChecked(true);

    // Поле ввода для выражения функции
    QLineEdit* expressionEdit = new QLineEdit(func->expression);

    // Кнопка выбора цвета
    QPushButton* colorButton = new QPushButton();
    colorButton->setFixedSize(20, 20);
    colorButton->setStyleSheet(QString("background-color: %1").arg(func->color.name()));

    // Кнопка удаления функции
    QPushButton* removeButton = new QPushButton("X");
    removeButton->setFixedSize(20, 20);

    itemLayout->addWidget(checkBox);
    itemLayout->addWidget(expressionEdit);
    itemLayout->addWidget(colorButton);
    itemLayout->addWidget(removeButton);

    item->setSizeHint(itemWidget->sizeHint());
    functionListWidget->setItemWidget(item, itemWidget);

    // Подключаем сигналы к элементам управления функции
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

    // Удаляем серию с графика
    chart->removeSeries(func->series);
    delete func->series;

    // Удаляем из списка функций
    functions.removeAt(index);
    delete func;

    // Удаляем элемент из списка виджета
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
    // Обработчик изменений в элементе списка функций
    FunctionItem* func = item->data(Qt::UserRole).value<FunctionItem*>();
    if (!func) {
        qDebug() << "Error: FunctionItem pointer is null.";
        return;
    }

    // Обновляем состояние функции и графика
    updateGraph();
}

void GraphWindow::updateGraph() {
    // Очищаем серии пересечений и наведения
    intersectionSeries->clear();
    hoverSeries->clear();

    // Получаем текущие диапазоны осей
    double xMin = axisX->min();
    double xMax = axisX->max();

    // Определяем количество точек на основе текущего диапазона оси X
    double pixelsPerPoint = 1.0; // Можно настроить для оптимизации
    double axisWidthInPixels = chartView->size().width();
    int numPoints = std::max(static_cast<int>((axisWidthInPixels / pixelsPerPoint)), 500);

    double step = (xMax - xMin) / numPoints;

    // Флаг для отслеживания ошибок разбора функций
    bool parsingSuccess = true;

    // Вычисляем значения каждой функции
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
            // Выделяем элемент функции красным цветом
            item->setBackground(Qt::red);
        }
        else {
            // Возвращаем стандартный фон
            item->setBackground(Qt::white);
        }

        func->series->replace(points);
    }

    if (!parsingSuccess) {
        QMessageBox::warning(this, "Parsing Error", "One or more functions could not be parsed. Please check your expressions.");
    }

    // Обновляем пересечения и нулевые линии
    updateIntersections();
    updateZeroLines();
}

void GraphWindow::updateIntersections() {
    intersectionSeries->clear();

    // Для каждой пары функций
    for (int i = 0; i < functions.size(); ++i) {
        FunctionItem* func1 = functions.at(i);
        if (!func1->visible)
            continue;

        for (int j = i + 1; j < functions.size(); ++j) {
            FunctionItem* func2 = functions.at(j);
            if (!func2->visible)
                continue;

            // Предполагаем, что обе функции имеют одинаковое количество точек
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

                // Проверяем, пересекаются ли функции между этими точками
                double f1 = y1 - y2;
                double f2 = y1_next - y2_next;
                if (f1 * f2 < 0) {
                    // Оцениваем точку пересечения
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

    // Проходим по всем функциям
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
    // Запускаем или перезапускаем таймер обновления
    updateTimer->start(updateInterval);
}
