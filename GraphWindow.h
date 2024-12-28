#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QMainWindow>  // ����������� �������� ������ ��� �������� ���� ����������
#include <QList>        // ����������� ������ ��� �������� �������
#include <QStatusBar>   // ����������� ��� ����������� ������ ���������
#include <QColor>       // ����������� ��� ������ � �������
#include <QInputDialog> // ����������� ��� �������� �����
#include <QPointF>      // ����������� ��� ������ � �������
#include <QTimer>       // ����������� ��� ������������� ��������
#include <QFileDialog>  // ����������� ��� �������� ���������� ���� ������
#include <QListWidgetItem>  // ����������� ��� ������ � ���������� ������
#include <QMenu>        // ����������� ��� ������ � ������������ ����
#include <QSplitter>    // ����������� ��� ������������� ������������ � ����������
#include <QAction>      // ����������� ��� ������ � ���������� (����, ������)
#include <QPushButton>  // ����������� ��� ������������� ������
#include <QListWidget>  // ����������� ��� ������ �� �������� ��������
#include <QtCharts/QChart>  // ����������� ��� ������������� ��������
#include <QtCharts/QChartView>  // ����������� ��� ����������� ��������
#include <QtCharts/QLegend>  // ����������� ��� ������ � ��������� �� ��������
#include <QtCharts/QLineSeries>  // ����������� ��� ����������� ����� �� ��������
#include <QtCharts/QScatterSeries>  // ����������� ��� ����������� ����� �� ��������
#include <QtCharts/QValueAxis>  // ����������� ��� ������ � ����� ��������

#include "muParser.h"  // ����������� ���������� ��� �������� �������������� ���������

// ��������� ��� �������� ���������� � �������
struct FunctionItem {
    QString expression;                // �������������� ��������� �������
    QColor color;                      // ���� ����� ������� �� �������
    QLineSeries* series;               // ����� ������, �������������� ������ �������
    bool visible;                      // ���� ��������� ������� �� �������
};
Q_DECLARE_METATYPE(FunctionItem*)  // ���������� ����-���� ��� ������������� � Qt

class CustomChartView;  // ��������������� ���������� ������ CustomChartView (������� ����� �������������� ��� ����������� ��������)

class GraphWindow : public QMainWindow {
    Q_OBJECT  // ������ ��� ����������, ��� ����� ���������� ��������� �������� � ������ Qt

public:
    explicit GraphWindow(QWidget* parent = nullptr);  // ����������� ������ � ������������ ������ ������������ ������
    ~GraphWindow();  // ���������� ������ ��� ������� ��������

protected:
    // � ������ ������ ��� ���������������� ������������ ������� ����

private:
    QChart* chart = nullptr;  // ��������� �� ������ ������� (��� ����������� ������)
    QLegend* legend = nullptr;  // ��������� �� ������� ������� (��� ����������� ��������)
    CustomChartView* chartView = nullptr;  // ��������� �� ������ ����������� �������
    QList<FunctionItem*> functions;  // ������ ���� �������, ������������ �� �������
    QScatterSeries* intersectionSeries = nullptr;  // ����� ��� ����������� ����� ����������� �������
    QScatterSeries* hoverSeries = nullptr;  // ����� ��� ����������� ����� ��� ��������� ����
    QLineSeries* xAxisZeroLine = nullptr;  // ����� ��� ����������� ������� ��� X
    QLineSeries* yAxisZeroLine = nullptr;  // ����� ��� ����������� ������� ��� Y
    QListWidget* functionListWidget = nullptr;  // ������ ������ ��� ����������� �������
    QPushButton* addFunctionButton = nullptr;  // ������ ��� ���������� ����� �������
    QPushButton* saveGraphButton = nullptr;  // ������ ��� ���������� �������
    QValueAxis* axisX = nullptr;  // ��� X
    QValueAxis* axisY = nullptr;  // ��� Y

    QTimer* updateTimer = nullptr;  // ������ ��� ����������� ���������� �������
    int updateInterval = 50;  // �������� ���������� � �������������

private:
    void updateGraph();  // ����� ��� ���������� �������
    void updateIntersections();  // ����� ��� ������ � ���������� ����� ����������� �������
    void updateZeroLines();  // ����� ��� ���������� �����, ������������ ��� X � Y
    double evaluateExpression(const QString& expression, double x, bool& ok);  // ����� ��� ���������� �������� ������� � ����� x
    QPointF findClosestPoint(const QPointF& chartPos);  // ����� ��� ������ ��������� ����� �� �������
    void addFunction(const QString& expression = "sin(x)");  // ����� ��� ���������� ����� �������
    void removeFunction(int index);  // ����� ��� �������� ������� �� �������
    QColor getNextColor();  // ����� ��� ��������� ���������� ����� ��� �������
    void createMenus();  // ����� ��� �������� ����
    void createToolBar();  // ����� ��� �������� ������ ������������

private slots:
    void onAddFunctionClicked();  // ���� ��� ��������� ����� �� ������ ���������� �������
    void onRemoveSelectedFunction();  // ���� ��� �������� ��������� �������
    void onFunctionItemChanged(QListWidgetItem* item);  // ���� ��� ��������� ��������� �������� ������ �������
    void onAxisRangeChanged();  // ���� ��� ��������� ��������� ��������� ����
    void saveGraph();  // ���� ��� ���������� �������
    void onFunctionListContextMenu(const QPoint& pos);  // ���� ��� ����������� ������������ ���� ��� ������ �������
    void updateMouseCoordinates(double x, double y);  // ���� ��� ���������� ��������� ���� �� �������

    // ����� �����
    void editFunction(int index);  // ���� ��� �������������� ������� �� �������
    void changeFunctionColor(int index);  // ���� ��� ��������� ����� �������
    void editFunctionByItem(QListWidgetItem* item);  // ���� ��� �������������� ������� ����� ������� ���� �� �������� ������
};

#endif // GRAPHWINDOW_H
