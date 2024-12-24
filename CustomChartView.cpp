// CustomChartView.cpp
#include "CustomChartView.h"
#include <QtWidgets/QApplication>
#include <QGraphicsGridLayout>
#include <QValueAxis>
#include <QIcon>

// Конструктор класса CustomChartView
CustomChartView::CustomChartView(QChart* chart, QWidget* parent)
    : QChartView(chart, parent),
    leftMouseButtonPressed(false),  // Переменная для отслеживания нажатия левой кнопки мыши
    rightMouseButtonPressed(false) {}  // Переменная для отслеживания нажатия правой кнопки мыши

// Обработчик события колесика мыши (для зума)
void CustomChartView::wheelEvent(QWheelEvent* event) {
    const qreal zoomFactor = 1.1;  // Коэффициент увеличения при зуме
    if (event->modifiers() & Qt::ControlModifier) {  // Если зажата клавиша Ctrl, увеличиваем или уменьшаем масштаб
        event->angleDelta().y() > 0 ? chart()->zoomIn() : chart()->zoomOut();  // В зависимости от направления колесика
    }
    else {
        // Если Ctrl не зажата, применяем зум на графике
        event->angleDelta().y() > 0 ? chart()->zoom(zoomFactor) : chart()->zoom(1 / zoomFactor);
    }
    event->accept();  // Указываем, что событие обработано
}

// Метод для переключения видимости сетки на графике
void CustomChartView::toggleGrid() {
    QList<QAbstractAxis*> axes = chart()->axes();  // Получаем список всех осей графика
    for (QAbstractAxis* axis : axes) {
        // Для каждой оси проверяем, является ли она осью значений (QValueAxis)
        if (auto* valueAxis = dynamic_cast<QValueAxis*>(axis)) {
            valueAxis->setGridLineVisible(!valueAxis->isGridLineVisible());  // Переключаем видимость сетки
        }
    }
}

// Обработчик события нажатия кнопок мыши
void CustomChartView::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {  // Если нажата левая кнопка мыши
        leftMouseButtonPressed = true;  // Отмечаем, что левая кнопка нажата
        lastMousePos = event->pos();  // Запоминаем позицию мыши
        setCursor(Qt::ClosedHandCursor);  // Меняем курсор на руку (для перемещения графика)
    }
    else if (event->button() == Qt::RightButton) {  // Если нажата правая кнопка мыши
        rightMouseButtonPressed = true;  // Отмечаем, что правая кнопка нажата
        lastMousePos = event->pos();  // Запоминаем позицию мыши
        setCursor(Qt::OpenHandCursor);  // Меняем курсор на открытую руку (для перемещения)
    }
    QChartView::mousePressEvent(event);  // Передаем обработку в базовый класс
}

// Обработчик события перемещения мыши
void CustomChartView::mouseMoveEvent(QMouseEvent* event) {
    if (leftMouseButtonPressed || rightMouseButtonPressed) {  // Если нажата хотя бы одна из кнопок мыши
        QPoint delta = event->pos() - lastMousePos;  // Вычисляем изменение положения мыши
        chart()->scroll(-delta.x(), delta.y());  // Перемещаем график на основе изменений координат мыши
        lastMousePos = event->pos();  // Обновляем позицию мыши
        event->accept();  // Указываем, что событие обработано
    }
    else {
        // Если кнопки мыши не нажаты, отображаем координаты точки на графике
        QPointF chartPos = chart()->mapToValue(event->pos());  // Преобразуем координаты мыши в координаты графика
        emit mouseMoved(chartPos.x(), chartPos.y());  // Эмитируем сигнал с координатами
        // Показываем всплывающую подсказку с координатами X и Y
        QToolTip::showText(event->globalPosition().toPoint(),
            QString("X: %1, Y: %2").arg(chartPos.x()).arg(chartPos.y()));
        QChartView::mouseMoveEvent(event);  // Передаем обработку в базовый класс
    }
}

// Обработчик события отпускания кнопки мыши
void CustomChartView::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {  // Если отпущена левая кнопка мыши
        leftMouseButtonPressed = false;  // Снимаем флаг нажатия левой кнопки
        setCursor(Qt::ArrowCursor);  // Возвращаем курсор в обычное состояние
    }
    else if (event->button() == Qt::RightButton) {  // Если отпущена правая кнопка мыши
        rightMouseButtonPressed = false;  // Снимаем флаг нажатия правой кнопки
        setCursor(Qt::ArrowCursor);  // Возвращаем курсор в обычное состояние
    }
    QChartView::mouseReleaseEvent(event);  // Передаем обработку в базовый класс
}

// Обработчик контекстного меню
void CustomChartView::contextMenuEvent(QContextMenuEvent* event) {
    QMenu contextMenu(this);  // Создаем контекстное меню

    // Действие сброса зума
    QAction* resetZoomAction = new QAction(QIcon(":/icons/images/reset_zoom.png"), "Reset zoom", this);
    connect(resetZoomAction, &QAction::triggered, this, &CustomChartView::resetZoom);
    contextMenu.addAction(resetZoomAction);

    // Действие для переключения сетки
    QAction* toggleGridAction = new QAction(QIcon(":/icons/images/toggle_grid.png"), "Toggle grid", this);
    connect(toggleGridAction, &QAction::triggered, this, &CustomChartView::toggleGrid);
    contextMenu.addAction(toggleGridAction);

    // Действие для смены темы
    QAction* changeThemeAction = new QAction(QIcon(":/icons/images/change_theme.png"), "Change theme", this);
    connect(changeThemeAction, &QAction::triggered, this, &CustomChartView::changeTheme);
    contextMenu.addAction(changeThemeAction);

    contextMenu.exec(event->globalPos());  // Показываем контекстное меню в позиции указателя мыши
}

// Сброс зума на графике
void CustomChartView::resetZoom() {
    chart()->zoomReset();  // Сбрасываем все изменения зума
}

// Метод для смены темы графика (светлая/темная)
void CustomChartView::changeTheme() {
    static bool isDarkTheme = true;  // Флаг для отслеживания текущей темы
    if (isDarkTheme) {
        chart()->setBackgroundRoundness(0);  // Убираем закругление фона для темной темы
        chart()->setTheme(QChart::ChartThemeDark);  // Устанавливаем темную тему
    }
    else {
        chart()->setBackgroundRoundness(10);  // Добавляем закругление фона для светлой темы
        chart()->setTheme(QChart::ChartThemeLight);  // Устанавливаем светлую тему
    }
    isDarkTheme = !isDarkTheme;  // Переключаем флаг темы
}
