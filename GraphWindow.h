#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QToolTip>
#include <QTimer>

class GraphWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit GraphWindow(QWidget* parent = nullptr);
    ~GraphWindow();

protected:
    void wheelEvent(QWheelEvent* event) override;    // Handle mouse wheel events
    void mousePressEvent(QMouseEvent* event) override; // Handle mouse press events
    void mouseMoveEvent(QMouseEvent* event) override;  // Handle mouse move events
    void mouseReleaseEvent(QMouseEvent* event) override; // Handle mouse release events

private:
    QChart* chart = nullptr;
    QChartView* chartView = nullptr;
    QLineSeries* series = nullptr;
    QScatterSeries* intersectionSeries = nullptr;
    QScatterSeries* hoverSeries = nullptr;
    QLineSeries* xAxisZeroLine = nullptr;
    QLineSeries* yAxisZeroLine = nullptr;
    QLineEdit* functionInput = nullptr;
    QPushButton* plotButton = nullptr;
    QValueAxis* axisX = nullptr;
    QValueAxis* axisY = nullptr;
    QString userFunction;

    QPoint lastMousePos; // Last mouse position
    bool isLeftMousePressed = false;

    QTimer* updateTimer = nullptr; // Timer to throttle updates
    int updateInterval = 50; // Update interval in milliseconds

private:
    void updateGraph();                   // Update the graph
    void updateIntersections();           // Find intersections with axes
    void updateZeroLines();               // Update zero lines
    double evaluateExpression(double x);  // Evaluate the user function
    QPointF findClosestPoint(const QPointF& chartPos); // Find the closest data point

private slots:
    void plotGraph();                     // Slot to plot the graph
    void onAxisRangeChanged();            // Slot for axis range changes
};

#endif // GRAPHWINDOW_H
