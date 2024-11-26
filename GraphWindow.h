#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QColor>
#include <QPointF>
#include <QTimer>
#include <QListWidgetItem>

// Подключаем необходимые заголовочные файлы из модуля QtCharts с префиксом QtCharts/
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>

#include <QPushButton>
#include <QListWidget>
#include <QWheelEvent>
#include <QMouseEvent>

// Структура для хранения информации о функции
struct FunctionItem {
    QString expression;                       // Выражение функции
    QColor color;                             // Цвет линии функции
    QLineSeries* series;            // Серия данных для отображения на графике
    bool visible;                             // Флаг видимости функции
};
Q_DECLARE_METATYPE(FunctionItem*)

class GraphWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit GraphWindow(QWidget* parent = nullptr);
    ~GraphWindow();

protected:
    // Переопределяем обработчики событий для взаимодействия с мышью и колесом прокрутки
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    QChart* chart = nullptr;
    QChartView* chartView = nullptr;
    QList<FunctionItem*> functions;
    QScatterSeries* intersectionSeries = nullptr;
    QScatterSeries* hoverSeries = nullptr;
    QLineSeries* xAxisZeroLine = nullptr;
    QLineSeries* yAxisZeroLine = nullptr;
    QListWidget* functionListWidget = nullptr;
    QPushButton* addFunctionButton = nullptr;
    QValueAxis* axisX = nullptr;
    QValueAxis* axisY = nullptr;

    QPoint lastMousePos; // Последняя позиция мыши
    bool isLeftMousePressed = false;

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

private slots:
    void onAddFunctionClicked();          // Слот для добавления функции
    void onFunctionItemChanged(QListWidgetItem* item); // Слот при изменении функции
    void onAxisRangeChanged();            // Слот при изменении диапазона осей
};

#endif // GRAPHWINDOW_H
