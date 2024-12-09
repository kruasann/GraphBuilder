// GraphWindow.h
#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QStatusBar>
#include <QColor>
#include <QInputDialog>
#include <QPointF>
#include <QTimer>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QMenu>
#include <QSplitter>
#include <QAction>
#include <QPushButton>
#include <QListWidget>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLegend>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>

#include "muParser.h"

// Структура для хранения информации о функции
struct FunctionItem {
    QString expression;                // Выражение функции
    QColor color;                      // Цвет линии функции
    QLineSeries* series;     // Серия данных для отображения на графике
    bool visible;                      // Флаг видимости функции
};
Q_DECLARE_METATYPE(FunctionItem*)

class CustomChartView; // Предварительное объявление

class GraphWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit GraphWindow(QWidget* parent = nullptr);
    ~GraphWindow();

protected:
    // Нет переопределенных обработчиков событий мыши

private:
    QChart* chart = nullptr;
    QLegend* legend = nullptr;
    CustomChartView* chartView = nullptr;
    QList<FunctionItem*> functions;
    QScatterSeries* intersectionSeries = nullptr;
    QScatterSeries* hoverSeries = nullptr;
    QLineSeries* xAxisZeroLine = nullptr;
    QLineSeries* yAxisZeroLine = nullptr;
    QListWidget* functionListWidget = nullptr;
    QPushButton* addFunctionButton = nullptr;
    QPushButton* saveGraphButton = nullptr;
    QValueAxis* axisX = nullptr;
    QValueAxis* axisY = nullptr;

    QTimer* updateTimer = nullptr; // Таймер для обновления графика
    int updateInterval = 50;       // Интервал обновления в миллисекундах

private:
    void updateGraph();                   // Обновление графика
    void updateIntersections();           // Обнаружение пересечений функций
    void updateZeroLines();               // Обновление нулевых линий осей
    double evaluateExpression(const QString& expression, double x, bool& ok);  // Вычисление значения функции
    QPointF findClosestPoint(const QPointF& chartPos); // Поиск ближайшей точки на графике
    void addFunction(const QString& expression = "sin(x)"); // Добавление новой функции
    void removeFunction(int index);       // Удаление функции
    QColor getNextColor();                // Получение следующего цвета для функции

    void createMenus();                   // Создание меню
    void createToolBar();                 // Создание панели инструментов

private slots:
    void onAddFunctionClicked();          // Слот для добавления функции
    void onRemoveSelectedFunction();      // Слот для удаления выбранной функции
    void onFunctionItemChanged(QListWidgetItem* item); // Слот при изменении функции
    void onAxisRangeChanged();            // Слот при изменении диапазона осей
    void saveGraph();
    void editFunction(int index);
    void changeFunctionColor(int index);
    void onFunctionListContextMenu(const QPoint& pos); // Слот для контекстного меню
    void updateMouseCoordinates(double x, double y);    // Слот для обновления координат мыши
};

#endif // GRAPHWINDOW_H
