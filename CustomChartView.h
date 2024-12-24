#ifndef CUSTOMCHARTVIEW_H
#define CUSTOMCHARTVIEW_H

// Подключаем необходимые заголовочные файлы для работы с графиками и обработкой событий в Qt
#include <QtCharts/QChartView>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QToolTip>

// Класс для кастомизированного отображения графика на основе QChartView
class CustomChartView : public QChartView {
    Q_OBJECT  // Мета-объект Qt для работы с сигналами и слотами

public:
    // Конструктор класса CustomChartView, принимает указатель на график (QChart)
    explicit CustomChartView(QChart* chart, QWidget* parent = nullptr);

signals:
    // Сигнал для передачи координат мыши, когда мышь двигается по графику
    void mouseMoved(double x, double y);

protected:
    // Переопределенные события для обработки взаимодействия с мышью и колесиком
    void wheelEvent(QWheelEvent* event) override;  // Обработчик события колесика мыши (для зума)
    void mousePressEvent(QMouseEvent* event) override;  // Обработчик события нажатия кнопки мыши
    void mouseMoveEvent(QMouseEvent* event) override;  // Обработчик события перемещения мыши
    void mouseReleaseEvent(QMouseEvent* event) override;  // Обработчик события отпускания кнопки мыши
    void contextMenuEvent(QContextMenuEvent* event) override;  // Обработчик контекстного меню

private:
    // Переменные состояния для отслеживания положения мыши и нажатия кнопок
    QPoint lastMousePos;  // Последняя позиция мыши
    bool leftMouseButtonPressed = false;  // Флаг, указывающий, нажата ли левая кнопка мыши
    bool rightMouseButtonPressed = false;  // Флаг, указывающий, нажата ли правая кнопка мыши

    // Приватные методы, выполняющие операции на графике
    void resetZoom();        // Метод для сброса зума графика
    void toggleGrid();       // Метод для переключения отображения сетки
    void changeTheme();      // Метод для переключения темы графика (темная/светлая)
};

#endif // CUSTOMCHARTVIEW_H
