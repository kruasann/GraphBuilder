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
    void startClicked();  // Сигнал, который будет отправлен при нажатии кнопки "Старт"

private:
    QPushButton* startButton;  // Кнопка для начала работы
    QLabel* titleLabel;        // Заголовок программы
    QLabel* authorLabel;       // Фамилия автора
};

#endif // SPLASHSCREEN_H
