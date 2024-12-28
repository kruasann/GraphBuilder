#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QMainWindow>  // Подключение базового класса для главного окна приложения
#include <QList>        // Подключение списка для хранения функций
#include <QStatusBar>   // Подключение для отображения строки состояния
#include <QColor>       // Подключение для работы с цветами
#include <QInputDialog> // Подключение для диалогов ввода
#include <QPointF>      // Подключение для работы с точками
#include <QTimer>       // Подключение для использования таймеров
#include <QFileDialog>  // Подключение для открытия диалоговых окон файлов
#include <QListWidgetItem>  // Подключение для работы с элементами списка
#include <QMenu>        // Подключение для работы с контекстными меню
#include <QSplitter>    // Подключение для использования разделителей в интерфейсе
#include <QAction>      // Подключение для работы с действиями (меню, кнопки)
#include <QPushButton>  // Подключение для использования кнопок
#include <QListWidget>  // Подключение для работы со списками виджетов
#include <QtCharts/QChart>  // Подключение для использования графиков
#include <QtCharts/QChartView>  // Подключение для отображения графиков
#include <QtCharts/QLegend>  // Подключение для работы с легендами на графиках
#include <QtCharts/QLineSeries>  // Подключение для отображения линий на графиках
#include <QtCharts/QScatterSeries>  // Подключение для отображения точек на графиках
#include <QtCharts/QValueAxis>  // Подключение для работы с осями графиков

#include "muParser.h"  // Подключение библиотеки для парсинга математических выражений

// Структура для хранения информации о функции
struct FunctionItem {
    QString expression;                // Математическое выражение функции
    QColor color;                      // Цвет линии функции на графике
    QLineSeries* series;               // Серия данных, представляющая график функции
    bool visible;                      // Флаг видимости функции на графике
};
Q_DECLARE_METATYPE(FunctionItem*)  // Объявление мета-типа для использования в Qt

class CustomChartView;  // Предварительное объявление класса CustomChartView (который будет использоваться для отображения графиков)

class GraphWindow : public QMainWindow {
    Q_OBJECT  // Макрос для объявления, что класс использует механизмы сигналов и слотов Qt

public:
    explicit GraphWindow(QWidget* parent = nullptr);  // Конструктор класса с возможностью задать родительский виджет
    ~GraphWindow();  // Деструктор класса для очистки ресурсов

protected:
    // В данном классе нет переопределенных обработчиков событий мыши

private:
    QChart* chart = nullptr;  // Указатель на объект графика (для отображения данных)
    QLegend* legend = nullptr;  // Указатель на легенду графика (для отображения подписей)
    CustomChartView* chartView = nullptr;  // Указатель на объект отображения графика
    QList<FunctionItem*> functions;  // Список всех функций, отображаемых на графике
    QScatterSeries* intersectionSeries = nullptr;  // Серия для отображения точек пересечений функций
    QScatterSeries* hoverSeries = nullptr;  // Серия для отображения точек при наведении мыши
    QLineSeries* xAxisZeroLine = nullptr;  // Линия для отображения нулевой оси X
    QLineSeries* yAxisZeroLine = nullptr;  // Линия для отображения нулевой оси Y
    QListWidget* functionListWidget = nullptr;  // Виджет списка для отображения функций
    QPushButton* addFunctionButton = nullptr;  // Кнопка для добавления новой функции
    QPushButton* saveGraphButton = nullptr;  // Кнопка для сохранения графика
    QValueAxis* axisX = nullptr;  // Ось X
    QValueAxis* axisY = nullptr;  // Ось Y

    QTimer* updateTimer = nullptr;  // Таймер для регулярного обновления графика
    int updateInterval = 50;  // Интервал обновления в миллисекундах

private:
    void updateGraph();  // Метод для обновления графика
    void updateIntersections();  // Метод для поиска и обновления точек пересечения функций
    void updateZeroLines();  // Метод для обновления линий, отображающих оси X и Y
    double evaluateExpression(const QString& expression, double x, bool& ok);  // Метод для вычисления значения функции в точке x
    QPointF findClosestPoint(const QPointF& chartPos);  // Метод для поиска ближайшей точки на графике
    void addFunction(const QString& expression = "sin(x)");  // Метод для добавления новой функции
    void removeFunction(int index);  // Метод для удаления функции по индексу
    QColor getNextColor();  // Метод для получения следующего цвета для функции
    void createMenus();  // Метод для создания меню
    void createToolBar();  // Метод для создания панели инструментов

private slots:
    void onAddFunctionClicked();  // Слот для обработки клика по кнопке добавления функции
    void onRemoveSelectedFunction();  // Слот для удаления выбранной функции
    void onFunctionItemChanged(QListWidgetItem* item);  // Слот для обработки изменения элемента списка функций
    void onAxisRangeChanged();  // Слот для обработки изменения диапазона осей
    void saveGraph();  // Слот для сохранения графика
    void onFunctionListContextMenu(const QPoint& pos);  // Слот для отображения контекстного меню для списка функций
    void updateMouseCoordinates(double x, double y);  // Слот для обновления координат мыши на графике

    // Новые слоты
    void editFunction(int index);  // Слот для редактирования функции по индексу
    void changeFunctionColor(int index);  // Слот для изменения цвета функции
    void editFunctionByItem(QListWidgetItem* item);  // Слот для редактирования функции через двойной клик по элементу списка
};

#endif // GRAPHWINDOW_H
