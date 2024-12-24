#ifndef CUSTOMCHARTVIEW_H
#define CUSTOMCHARTVIEW_H

// ���������� ����������� ������������ ����� ��� ������ � ��������� � ���������� ������� � Qt
#include <QtCharts/QChartView>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QToolTip>

// ����� ��� ������������������ ����������� ������� �� ������ QChartView
class CustomChartView : public QChartView {
    Q_OBJECT  // ����-������ Qt ��� ������ � ��������� � �������

public:
    // ����������� ������ CustomChartView, ��������� ��������� �� ������ (QChart)
    explicit CustomChartView(QChart* chart, QWidget* parent = nullptr);

signals:
    // ������ ��� �������� ��������� ����, ����� ���� ��������� �� �������
    void mouseMoved(double x, double y);

protected:
    // ���������������� ������� ��� ��������� �������������� � ����� � ���������
    void wheelEvent(QWheelEvent* event) override;  // ���������� ������� �������� ���� (��� ����)
    void mousePressEvent(QMouseEvent* event) override;  // ���������� ������� ������� ������ ����
    void mouseMoveEvent(QMouseEvent* event) override;  // ���������� ������� ����������� ����
    void mouseReleaseEvent(QMouseEvent* event) override;  // ���������� ������� ���������� ������ ����
    void contextMenuEvent(QContextMenuEvent* event) override;  // ���������� ������������ ����

private:
    // ���������� ��������� ��� ������������ ��������� ���� � ������� ������
    QPoint lastMousePos;  // ��������� ������� ����
    bool leftMouseButtonPressed = false;  // ����, �����������, ������ �� ����� ������ ����
    bool rightMouseButtonPressed = false;  // ����, �����������, ������ �� ������ ������ ����

    // ��������� ������, ����������� �������� �� �������
    void resetZoom();        // ����� ��� ������ ���� �������
    void toggleGrid();       // ����� ��� ������������ ����������� �����
    void changeTheme();      // ����� ��� ������������ ���� ������� (������/�������)
};

#endif // CUSTOMCHARTVIEW_H
