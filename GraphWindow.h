#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QColor>
#include <QPointF>
#include <QTimer>
#include <QListWidgetItem>

// ���������� ����������� ������������ ����� �� ������ QtCharts � ��������� QtCharts/
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>

#include <QPushButton>
#include <QListWidget>
#include <QWheelEvent>
#include <QMouseEvent>

// ��������� ��� �������� ���������� � �������
struct FunctionItem {
    QString expression;                       // ��������� �������
    QColor color;                             // ���� ����� �������
    QLineSeries* series;            // ����� ������ ��� ����������� �� �������
    bool visible;                             // ���� ��������� �������
};
Q_DECLARE_METATYPE(FunctionItem*)

class GraphWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit GraphWindow(QWidget* parent = nullptr);
    ~GraphWindow();

protected:
    // �������������� ����������� ������� ��� �������������� � ����� � ������� ���������
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

    QPoint lastMousePos; // ��������� ������� ����
    bool isLeftMousePressed = false;

    QTimer* updateTimer = nullptr; // ������ ��� ���������� �������
    int updateInterval = 50;       // �������� ���������� � �������������

private:
    void updateGraph();                   // ���������� �������
    void updateIntersections();           // ����������� ����������� �������
    void updateZeroLines();               // ���������� ������� ����� ����
    double evaluateExpression(const QString& expression, double x, bool& ok);  // ���������� �������� �������
    QPointF findClosestPoint(const QPointF& chartPos); // ����� ��������� ����� �� �������
    void addFunction(const QString& expression = "sin(x)"); // ���������� ����� �������
    void removeFunction(int index);       // �������� �������
    QColor getNextColor();                // ��������� ���������� ����� ��� �������

private slots:
    void onAddFunctionClicked();          // ���� ��� ���������� �������
    void onFunctionItemChanged(QListWidgetItem* item); // ���� ��� ��������� �������
    void onAxisRangeChanged();            // ���� ��� ��������� ��������� ����
};

#endif // GRAPHWINDOW_H
