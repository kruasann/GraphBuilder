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
#include <algorithm> // Для std::max

GraphWindow::GraphWindow(QWidget* parent)
    : QMainWindow(parent) {

    setWindowIcon(QIcon(":/images/GraphBuilderLogo.png"));

    qRegisterMetaType<FunctionItem*>("FunctionItem*");

    // Создание QSplitter и установка его как центрального виджета
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);

    // Левая часть: список функций и кнопки управления
    QWidget* leftWidget = new QWidget();
    QVBoxLayout* leftLayout = new QVBoxLayout(leftWidget);

    // Список функций
    functionListWidget = new QListWidget(this);
    functionListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    functionListWidget->setDragDropMode(QAbstractItemView::InternalMove);
    functionListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    leftLayout->addWidget(functionListWidget);

    // Подключение сигналов для контекстного меню и перетаскивания
    connect(functionListWidget, &QListWidget::customContextMenuRequested, this, &GraphWindow::onFunctionListContextMenu);
    connect(functionListWidget, &QListWidget::itemChanged, this, &GraphWindow::onFunctionItemChanged);
    connect(functionListWidget->model(), &QAbstractItemModel::rowsMoved, this, &GraphWindow::updateGraph);

    // Кнопка добавления функции
    addFunctionButton = new QPushButton(QIcon(":/icons/add.png"), tr("Add function"), this);
    leftLayout->addWidget(addFunctionButton);
    connect(addFunctionButton, &QPushButton::clicked, this, &GraphWindow::onAddFunctionClicked);

    // Кнопка сохранения графика
    saveGraphButton = new QPushButton(QIcon(":/icons/save.png"), tr("Save plot"), this);
    leftLayout->addWidget(saveGraphButton);
    connect(saveGraphButton, &QPushButton::clicked, this, &GraphWindow::saveGraph);

    // Добавляем растяжку, чтобы кнопки занимали минимально необходимое пространство
    leftLayout->addStretch();

    // Устанавливаем политику размеров для левой панели
    leftWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    splitter->addWidget(leftWidget);

    // Правая часть: график
    QWidget* rightWidget = new QWidget();
    QVBoxLayout* rightLayout = new QVBoxLayout(rightWidget);

    // Создаем график
    chart = new QChart();
    chart->setTitle(tr("Plot Viewer"));

    // Создаем серии для пересечений и наведения
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

    // Создаем нулевые линии осей
    xAxisZeroLine = new QLineSeries();
    yAxisZeroLine = new QLineSeries();
    xAxisZeroLine->setPen(QPen(Qt::black, 1, Qt::DashLine));
    yAxisZeroLine->setPen(QPen(Qt::black, 1, Qt::DashLine));
    xAxisZeroLine->setName(tr("X = 0"));
    yAxisZeroLine->setName(tr("Y = 0"));
    chart->addSeries(xAxisZeroLine);
    chart->addSeries(yAxisZeroLine);

    // Настраиваем отображение графика
    chartView = new CustomChartView(chart, this);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    rightLayout->addWidget(chartView);

    // Устанавливаем политику размеров для правой панели
    rightWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    splitter->addWidget(rightWidget);

    // Устанавливаем коэффициенты растяжения: левая панель занимает 20%, правая — 80%
    splitter->setStretchFactor(0, 1); // Левая панель
    splitter->setStretchFactor(1, 4); // Правая панель (график)

    // Устанавливаем центральный виджет
    setCentralWidget(splitter);
    setWindowTitle(tr("Plot Viewer Qt Charts"));
    resize(1200, 700);

    // Создаем оси
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

    // Инициализируем таймер обновления
    updateTimer = new QTimer(this);
    updateTimer->setSingleShot(true);
    connect(updateTimer, &QTimer::timeout, this, &GraphWindow::updateGraph);

    // Подключаем сигналы изменения диапазона осей
    connect(axisX, &QValueAxis::rangeChanged, this, &GraphWindow::onAxisRangeChanged);
    connect(axisY, &QValueAxis::rangeChanged, this, &GraphWindow::onAxisRangeChanged);

    // Создание меню и панели инструментов
    createMenus();
    createToolBar();

    // Подключаем сигнал из CustomChartView для обновления координат мыши
    connect(chartView, &CustomChartView::mouseMoved, this, &GraphWindow::updateMouseCoordinates);

    // Инициализируем с одной функцией
    addFunction();
}

GraphWindow::~GraphWindow() {
    // Очищаем функции
    qDeleteAll(functions);
}

void GraphWindow::createMenus() {
    // Меню уже создано в main.cpp, дополнительные меню можно добавить здесь при необходимости
}

void GraphWindow::createToolBar() {
    // Панель инструментов уже создана в конструкторе
}

void GraphWindow::addFunction(const QString& expression) {
    FunctionItem* func = new FunctionItem;
    func->expression = expression;
    func->color = getNextColor();
    func->series = new QLineSeries();
    func->series->setColor(func->color);
    func->visible = true;

    func->series->setName(func->expression); // Устанавливаем имя серии

    chart->addSeries(func->series);
    func->series->attachAxis(axisX);
    func->series->attachAxis(axisY);

    functions.append(func);

    // Добавляем функцию в список функций с элементами управления
    QListWidgetItem* item = new QListWidgetItem(functionListWidget);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    item->setCheckState(Qt::Checked);
    item->setData(Qt::UserRole, QVariant::fromValue(func));
    item->setText(func->expression);

    // Установка иконки цвета
    QPixmap colorPixmap(20, 20);
    colorPixmap.fill(func->color);
    item->setIcon(QIcon(colorPixmap));
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
        Qt::blue, Qt::green, Qt::magenta, Qt::cyan, QColor("#FFA500"), // Оранжевый
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
    // Обработчик изменений в элементе списка функций
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
            item->setBackground(QColor("#FFCCCC"));
        }
        else {
            // Возвращаем стандартный фон
            item->setBackground(Qt::white);
        }

        func->series->replace(points);
    }

    if (!parsingSuccess) {
        QMessageBox::warning(this, tr("Parsing error"), tr("One or more functions could not be parsed. Please check your expressions."));
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

void GraphWindow::updateMouseCoordinates(double x, double y) {
    // Обновляем статусную строку с текущими координатами мыши
    statusBar()->showMessage(QString("X: %1, Y: %2").arg(x, 0, 'f', 2).arg(y, 0, 'f', 2));
}

void GraphWindow::onAxisRangeChanged() {
    // Запускаем или перезапускаем таймер обновления
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
