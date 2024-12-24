#include "GraphWindow.h"
#include "CustomChartView.h"

#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>

#include <QVBoxLayout>
#include <QGraphicsGridLayout>
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

    // Добавим обработку двойного клика для редактирования функции
    connect(functionListWidget, &QListWidget::itemDoubleClicked, this, &GraphWindow::editFunctionByItem);

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
    // Проверяем, что индекс функции находится в допустимом диапазоне
    if (index < 0 || index >= functions.size())
        return;  // Если индекс некорректный, выходим из метода

    // Получаем указатель на объект FunctionItem по индексу
    FunctionItem* func = functions.at(index);

    // Удаляем серию с графика
    chart->removeSeries(func->series);  // Убираем серию, связанную с этой функцией, с графика
    delete func->series;  // Удаляем сам объект серии, освобождая память

    // Удаляем функцию из списка функций
    functions.removeAt(index);  // Убираем функцию из списка
    delete func;  // Освобождаем память, занятую объектом FunctionItem

    // Удаляем элемент из списка виджетов (функций)
    delete functionListWidget->takeItem(index);  // Удаляем элемент из QListWidget

    // Обновляем график, чтобы отобразить изменения
    updateGraph();
}


QColor GraphWindow::getNextColor() {
    // Статическая переменная для отслеживания индекса текущего цвета
    static int colorIndex = 0;

    // Статический список цветов для использования в графиках
    static QList<QColor> colors = {
        Qt::blue, Qt::green, Qt::magenta, Qt::cyan, QColor("#FFA500"), // Оранжевый
        Qt::darkRed, Qt::darkCyan, Qt::darkMagenta, Qt::darkGreen
    };

    // Выбираем цвет на основе текущего индекса, используя операцию по модулю, чтобы циклически перебирать список
    QColor color = colors.at(colorIndex % colors.size());

    // Увеличиваем индекс, чтобы следующий вызов метода вернул следующий цвет
    colorIndex++;

    // Возвращаем выбранный цвет
    return color;
}

void GraphWindow::onAddFunctionClicked() {
    bool ok;
    // Открываем диалог для ввода нового выражения функции
    QString expression = QInputDialog::getText(this, tr("Add function"),
        tr("Enter function expression:"), QLineEdit::Normal,
        "sin(x)", &ok);  // В поле по умолчанию будет "sin(x)"

    // Если пользователь подтвердил ввод и не оставил поле пустым
    if (ok && !expression.isEmpty()) {
        addFunction(expression);  // Добавляем новую функцию
        updateGraph();  // Обновляем график, чтобы отобразить изменения
    }
}


void GraphWindow::onRemoveSelectedFunction() {
    // Получаем выбранный элемент из списка функций
    QListWidgetItem* selectedItem = functionListWidget->currentItem();

    // Если элемент выбран
    if (selectedItem) {
        // Получаем индекс выбранного элемента
        int index = functionListWidget->row(selectedItem);
        // Удаляем функцию по индексу
        removeFunction(index);
    }
    else {
        // Если элемент не выбран, выводим информационное сообщение
        QMessageBox::information(this, tr("Deleting function"), tr("Please select the function to delete."));
    }
}


void GraphWindow::onFunctionItemChanged(QListWidgetItem* item) {
    // Обработчик изменений в элементе списка функций
    // Извлекаем указатель на объект FunctionItem, связанный с элементом списка
    FunctionItem* func = item->data(Qt::UserRole).value<FunctionItem*>();

    // Если указатель на функцию пустой (ошибка), выводим сообщение и выходим
    if (!func) {
        qDebug() << "Error: FunctionItem pointer is null.";  // Отладочное сообщение
        return;  // Выходим из метода, так как функция не найдена
    }

    // Изменяем видимость функции в зависимости от состояния флажка (отметка в списке)
    func->visible = (item->checkState() == Qt::Checked);  // Если элемент отмечен, видимость становится true
    func->series->setVisible(func->visible);  // Устанавливаем видимость соответствующей серии на графике

    // Обновляем график, чтобы отобразить изменения
    updateGraph();
}


void GraphWindow::updateGraph() {
    // Очищаем серии пересечений и наведения перед новым вычислением
    intersectionSeries->clear();
    hoverSeries->clear();

    // Получаем текущие минимальные и максимальные значения для оси X
    double xMin = axisX->min();  // Минимальное значение оси X
    double xMax = axisX->max();  // Максимальное значение оси X

    // Определяем количество точек для построения графиков
    double pixelsPerPoint = 1.0;  // Параметр, который можно настроить для оптимизации
    double axisWidthInPixels = chartView->size().width();  // Ширина области графика в пикселях
    int numPoints = std::max(static_cast<int>((axisWidthInPixels / pixelsPerPoint)), 500);  // Число точек для вычисления графика (минимум 500 точек)

    double step = (xMax - xMin) / numPoints;  // Шаг, с которым будем вычислять значения по оси X

    // Флаг для отслеживания ошибок при разборе функций
    bool parsingSuccess = true;

    // Перебираем все функции
    for (FunctionItem* func : functions) {
        // Пропускаем функции, которые не отображаются
        if (!func->visible)
            continue;

        QVector<QPointF> points;  // Вектор для хранения вычисленных точек функции
        bool funcParsingSuccess = true;  // Флаг для отслеживания ошибок для каждой функции

        // Для каждой функции вычисляем точки на графике
        for (double x = xMin; x <= xMax; x += step) {
            bool ok;
            // Вычисляем значение функции в точке x
            double y = evaluateExpression(func->expression, x, ok);

            // Если не удалось вычислить значение функции, прекращаем обработку этой функции
            if (!ok) {
                funcParsingSuccess = false;
                break;
            }

            // Пропускаем недопустимые значения (NaN или бесконечности)
            if (std::isnan(y) || std::isinf(y))
                continue;

            // Добавляем точку (x, y) в список точек функции
            points.append(QPointF(x, y));
        }

        // Получаем индекс текущей функции в списке
        int index = functions.indexOf(func);
        // Получаем элемент списка для этой функции
        QListWidgetItem* item = functionListWidget->item(index);

        // Если функция не была успешно разобрана, выделяем ее в списке
        if (!funcParsingSuccess) {
            parsingSuccess = false;
            // Выделяем элемент функции красным цветом, чтобы показать ошибку
            item->setBackground(QColor("#FFCCCC"));
            item->setToolTip(tr("Error parsing expression: %1").arg(func->expression));  // Отображаем ошибку в подсказке
        }
        else {
            // Если функция успешно разобрана, восстанавливаем стандартный фон
            item->setBackground(Qt::white);
            item->setToolTip("");  // Убираем подсказку об ошибке
        }

        // Заменяем старые точки на новые для этой функции
        func->series->replace(points);
    }

    // Если были ошибки при разборе хотя бы одной функции, показываем предупреждение
    if (!parsingSuccess) {
        QMessageBox::warning(this, tr("Parsing error"), tr("One or more functions could not be parsed. Please check your expressions."));
    }

    // Обновляем пересечения и нулевые линии после обновления графиков
    updateIntersections();
    updateZeroLines();
}


void GraphWindow::updateIntersections() {
    // Очищаем серию пересечений перед вычислением новых
    intersectionSeries->clear();

    // Проходим по всем возможным парам функций
    for (int i = 0; i < functions.size(); ++i) {
        // Получаем указатель на первую функцию
        FunctionItem* func1 = functions.at(i);

        // Пропускаем невидимые функции
        if (!func1->visible)
            continue;

        // Внутренний цикл для сравнения с каждой следующей функцией
        for (int j = i + 1; j < functions.size(); ++j) {
            // Получаем указатель на вторую функцию
            FunctionItem* func2 = functions.at(j);

            // Пропускаем невидимые функции
            if (!func2->visible)
                continue;

            // Получаем точки на графиках обеих функций
            const auto& points1 = func1->series->points();
            const auto& points2 = func2->series->points();

            // Количество точек для проверки пересечений ограничено минимальным количеством точек в обеих функциях
            int numPoints = std::min(points1.size(), points2.size());

            // Проходим по всем точкам обеих функций, за исключением последней
            for (int k = 0; k < numPoints - 1; ++k) {
                // Получаем координаты текущей и следующей точки для обеих функций
                double x1 = points1.at(k).x();
                double y1 = points1.at(k).y();
                double y2 = points2.at(k).y();

                double x2 = points1.at(k + 1).x();
                double y1_next = points1.at(k + 1).y();
                double y2_next = points2.at(k + 1).y();

                // Проверяем, пересекаются ли функции между текущими и следующими точками
                // Это можно определить, если значения функций с обеих сторон от текущей точки имеют разные знаки
                double f1 = y1 - y2;
                double f2 = y1_next - y2_next;
                if (f1 * f2 < 0) {
                    // Оцениваем точку пересечения с помощью линейной интерполяции
                    double t = f1 / (f1 - f2);
                    double xIntersect = x1 + t * (x2 - x1);  // Находим x-координату точки пересечения
                    double yIntersect = y1 + t * (y1_next - y1);  // Находим y-координату точки пересечения

                    // Добавляем найденную точку пересечения в серию
                    intersectionSeries->append(xIntersect, yIntersect);
                }
            }
        }
    }
}


void GraphWindow::updateZeroLines() {
    // Получаем текущие минимальные и максимальные значения осей X и Y
    double xMin = axisX->min();  // Минимальное значение для оси X
    double xMax = axisX->max();  // Максимальное значение для оси X
    double yMin = axisY->min();  // Минимальное значение для оси Y
    double yMax = axisY->max();  // Максимальное значение для оси Y

    // Очищаем серии для нулевых линий
    xAxisZeroLine->clear();  // Очищаем данные на серии для оси X
    yAxisZeroLine->clear();  // Очищаем данные на серии для оси Y

    // Если ось Y пересекает 0 (то есть значения по оси Y лежат как ниже, так и выше нуля)
    if (yMin <= 0 && yMax >= 0) {
        // Добавляем точки на оси X, где y = 0 (по сути, линия по оси X, которая представляет y = 0)
        xAxisZeroLine->append(xMin, 0);  // Добавляем точку с минимальным значением X и y = 0
        xAxisZeroLine->append(xMax, 0);  // Добавляем точку с максимальным значением X и y = 0
    }

    // Если ось X пересекает 0 (то есть значения по оси X лежат как слева, так и справа от нуля)
    if (xMin <= 0 && xMax >= 0) {
        // Добавляем точки на оси Y, где x = 0 (по сути, линия по оси Y, которая представляет x = 0)
        yAxisZeroLine->append(0, yMin);  // Добавляем точку с минимальным значением Y и x = 0
        yAxisZeroLine->append(0, yMax);  // Добавляем точку с максимальным значением Y и x = 0
    }
}


double GraphWindow::evaluateExpression(const QString& expression, double x, bool& ok) {
    try {
        // Создаем объект парсера для обработки математических выражений
        mu::Parser parser;

        // Определяем переменную x, которая будет использоваться в выражении
        parser.DefineVar("x", &x);  // Привязываем переменную "x" к значению, переданному в параметре

        // Устанавливаем математическое выражение для парсера
        parser.SetExpr(expression.toUtf8().constData());  // Преобразуем QString в формат UTF-8 для парсера

        // Вычисляем результат выражения
        double result = parser.Eval();  // Выполняем вычисление выражения
        ok = true;  // Если вычисление прошло успешно, устанавливаем флаг ok в true
        return result;  // Возвращаем результат вычисления
    }
    catch (mu::Parser::exception_type& e) {  // Обрабатываем исключения, возникающие при вычислении
        // qDebug() << "Error evaluating expression:" << e.GetMsg().c_str();  // Закомментированная отладочная строка для вывода ошибки
        ok = false;  // Если произошла ошибка, устанавливаем флаг ok в false
        return std::numeric_limits<double>::quiet_NaN();  // Возвращаем NaN (Not-a-Number) как индикатор ошибки
    }
}


QPointF GraphWindow::findClosestPoint(const QPointF& chartPos) {
    // Инициализируем переменную для хранения ближайшей точки
    QPointF closestPoint;

    // Инициализируем минимальное расстояние как максимально возможное значение
    // Это позволяет найти ближайшую точку (когда расстояние будет меньше этого значения)
    double minDistance = std::numeric_limits<double>::max();

    // Проходим по всем функциям
    for (FunctionItem* func : functions) {
        // Пропускаем невидимые функции
        if (!func->visible)
            continue;

        // Проходим по всем точкам текущей функции
        for (const QPointF& point : func->series->points()) {
            // Вычисляем расстояние от текущей точки функции до точки на графике (chartPos)
            double distance = std::hypot(point.x() - chartPos.x(), point.y() - chartPos.y());

            // Если найдено более близкое расстояние, обновляем минимальное расстояние и точку
            if (distance < minDistance) {
                minDistance = distance;  // Обновляем минимальное расстояние
                closestPoint = point;    // Обновляем ближайшую точку
            }
        }
    }

    // Возвращаем точку, которая была наиболее близка к chartPos
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
    // Получаем элемент списка, в который был произведен клик правой кнопкой мыши
    QListWidgetItem* item = functionListWidget->itemAt(pos);

    // Если элемент был найден (не NULL)
    if (item) {
        // Создаем контекстное меню
        QMenu contextMenu(this);

        // Добавляем действия в контекстное меню
        QAction* removeAction = contextMenu.addAction(tr("Delete function"));  // Удалить функцию
        QAction* editAction = contextMenu.addAction(tr("Edit function"));     // Редактировать функцию
        QAction* changeColorAction = contextMenu.addAction(tr("Change color")); // Изменить цвет функции

        // Отображаем контекстное меню и получаем выбранное действие
        QAction* selectedAction = contextMenu.exec(functionListWidget->viewport()->mapToGlobal(pos));

        // Если выбрано действие "Удалить функцию"
        if (selectedAction == removeAction) {
            int index = functionListWidget->row(item);  // Получаем индекс выбранного элемента
            removeFunction(index);  // Удаляем функцию по индексу
        }
        // Если выбрано действие "Редактировать функцию"
        else if (selectedAction == editAction) {
            int index = functionListWidget->row(item);  // Получаем индекс выбранного элемента
            editFunction(index);  // Открываем окно редактирования функции по индексу
        }
        // Если выбрано действие "Изменить цвет"
        else if (selectedAction == changeColorAction) {
            int index = functionListWidget->row(item);  // Получаем индекс выбранного элемента
            changeFunctionColor(index);  // Открываем диалог для изменения цвета функции по индексу
        }
    }
}


void GraphWindow::editFunction(int index) {
    // Проверяем, что индекс действителен и находится в пределах массива функций
    if (index < 0 || index >= functions.size())
        return;  // Если индекс неверный, выходим из функции

    // Получаем указатель на объект FunctionItem по индексу
    FunctionItem* func = functions.at(index);
    if (!func)  // Если указатель на функцию равен nullptr, выходим
        return;

    // Ожидаем новый ввод от пользователя
    bool ok;
    // Открываем диалоговое окно для ввода нового выражения функции
    QString newExpression = QInputDialog::getText(this, tr("Edit function"),
        tr("Enter new function expression:"), QLineEdit::Normal,
        func->expression, &ok);  // Пре-установим текущее выражение как значение по умолчанию

    // Если пользователь подтвердил ввод и ввел непустое выражение
    if (ok && !newExpression.isEmpty()) {
        func->expression = newExpression;  // Обновляем выражение функции
        func->series->setName(func->expression);  // Обновляем имя серии на графике

        // Обновляем текст в QListWidgetItem, который отображает название функции
        QListWidgetItem* item = functionListWidget->item(index);
        if (item) {
            item->setText(func->expression);  // Устанавливаем новый текст (выражение) в элемент списка
        }

        // Обновляем график после изменения выражения функции
        updateGraph();
    }
}


void GraphWindow::changeFunctionColor(int index) {
    // Проверяем, что индекс функции валидный (в пределах списка функций)
    if (index < 0 || index >= functions.size())
        return;  // Если индекс некорректный, выходим из метода

    // Получаем указатель на объект функции по индексу
    FunctionItem* func = functions.at(index);

    // Если функция не существует (невалидный указатель), выходим из метода
    if (!func)
        return;

    // Открываем диалог для выбора нового цвета
    QColor newColor = QColorDialog::getColor(func->color, this, tr("Select Function Color"));

    // Если выбран корректный цвет
    if (newColor.isValid()) {
        // Обновляем цвет функции
        func->color = newColor;

        // Обновляем цвет линии на графике для этой функции
        func->series->setColor(func->color);

        // Обновляем иконку цвета в элементе списка функций (QListWidgetItem)
        QListWidgetItem* item = functionListWidget->item(index);
        if (item) {
            // Создаем пиксельное изображение с выбранным цветом
            QPixmap colorPixmap(20, 20);
            colorPixmap.fill(func->color);  // Заполняем изображение выбранным цветом

            // Устанавливаем эту иконку в элемент списка
            item->setIcon(QIcon(colorPixmap));
        }

        // Обновляем график, чтобы отобразить изменения цвета
        updateGraph();
    }
}


void GraphWindow::editFunctionByItem(QListWidgetItem* item) {
    // Проверяем, что переданный элемент не пустой (не nullptr)
    if (!item)
        return;  // Если элемент не существует, выходим из метода

    // Получаем индекс элемента в списке функций
    int index = functionListWidget->row(item);

    // Вызываем метод для редактирования функции по индексу
    editFunction(index);
}

