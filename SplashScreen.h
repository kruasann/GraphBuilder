#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class SplashScreen : public QWidget {
    Q_OBJECT
public:
    explicit SplashScreen(QWidget* parent = nullptr);

signals:
    void startClicked();  // ������, ������� ����� ��������� ��� ������� ������ "�����"

private:
    QPushButton* startButton;  // ������ ��� ������ ������
    QLabel* titleLabel;        // ��������� ���������
    QLabel* authorLabel;       // ������� ������
};

#endif // SPLASHSCREEN_H
