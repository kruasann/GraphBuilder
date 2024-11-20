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
#include <QColor>
#include <QListWidget>
#include <QCheckBox>
#include <QSpinBox>
#include <QMetaType>

struct FunctionItem {
    QString expression;
    QColor color;
    QLineSeries* series;
    bool visible;
};
Q_DECLARE_METATYPE(FunctionItem*)

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
    QList<FunctionItem*> functions;
    QScatterSeries* intersectionSeries = nullptr;
    QScatterSeries* hoverSeries = nullptr;
    QLineSeries* xAxisZeroLine = nullptr;
    QLineSeries* yAxisZeroLine = nullptr;
    QListWidget* functionListWidget = nullptr;
    QPushButton* addFunctionButton = nullptr;
    QValueAxis* axisX = nullptr;
    QValueAxis* axisY = nullptr;

    QPoint lastMousePos; // Last mouse position
    bool isLeftMousePressed = false;

    QTimer* updateTimer = nullptr; // Timer to throttle updates
    int updateInterval = 50; // Update interval in milliseconds

private:
    void updateGraph();                   // Update the graph
    void updateIntersections();           // Find intersections between functions
    void updateZeroLines();               // Update zero lines
    double evaluateExpression(const QString& expression, double x);  // Evaluate a function
    QPointF findClosestPoint(const QPointF& chartPos); // Find the closest data point
    void addFunction(const QString& expression = "sin(x)"); // Add a new function
    void removeFunction(int index);       // Remove a function
    QColor getNextColor();                // Get the next color for a new function

private slots:
    void onAddFunctionClicked();          // Slot to add a function
    void onFunctionItemChanged(QListWidgetItem* item); // Slot when a function item is changed
    void onAxisRangeChanged();            // Slot for axis range changes
};

#endif // GRAPHWINDOW_H
