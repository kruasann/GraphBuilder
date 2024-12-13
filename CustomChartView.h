#ifndef CUSTOMCHARTVIEW_H
#define CUSTOMCHARTVIEW_H

#include <QtCharts/QChartView>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QToolTip>

class CustomChartView : public QChartView {
    Q_OBJECT
public:
    explicit CustomChartView(QChart* chart, QWidget* parent = nullptr);

signals:
    void mouseMoved(double x, double y); // Сигнал для передачи координат мыши

protected:
    // Переопределенные события
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override; // Контекстное меню

private:
    // Переменные состояния для обработки событий мыши
    QPoint lastMousePos; // Последняя позиция мыши
    bool leftMouseButtonPressed = false; // ЛКМ нажата
    bool rightMouseButtonPressed = false; // ПКМ нажата

    // Частные методы
    void resetZoom();        // Сбросить зум
    void toggleGrid();       // Переключить отображение сетки
    void changeTheme();      // Изменить тему (темная/светлая)
};

#endif // CUSTOMCHARTVIEW_H
