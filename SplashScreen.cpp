#include "SplashScreen.h"

SplashScreen::SplashScreen(QWidget* parent)
    : QWidget(parent) {
    // Настраиваем внешний вид окна
    setWindowTitle(tr("Welcome"));
    resize(400, 300);

    // Создаем элементы интерфейса
    titleLabel = new QLabel(tr("GraphBuilder"), this);  // Заголовок программы
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");

    authorLabel = new QLabel(tr("Author: Your Last Name"), this);  // Имя автора
    authorLabel->setAlignment(Qt::AlignCenter);
    authorLabel->setStyleSheet("font-size: 16px;");

    startButton = new QPushButton(tr("Start"), this);  // Кнопка "Старт"
    startButton->setStyleSheet("font-size: 18px;");

    // Размещаем элементы интерфейса в вертикальном макете
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(titleLabel);
    layout->addWidget(authorLabel);
    layout->addStretch();  // Растяжка для центровки
    layout->addWidget(startButton);

    // Подключаем сигнал от кнопки к сигналу startClicked
    connect(startButton, &QPushButton::clicked, this, &SplashScreen::startClicked);
}
