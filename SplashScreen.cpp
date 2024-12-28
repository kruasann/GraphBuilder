#include "SplashScreen.h"

SplashScreen::SplashScreen(QWidget* parent)
    : QWidget(parent) {
    // ����������� ������� ��� ����
    setWindowTitle(tr("Welcome"));
    resize(400, 300);

    // ������� �������� ����������
    titleLabel = new QLabel(tr("GraphBuilder"), this);  // ��������� ���������
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");

    authorLabel = new QLabel(tr("Author: Your Last Name"), this);  // ��� ������
    authorLabel->setAlignment(Qt::AlignCenter);
    authorLabel->setStyleSheet("font-size: 16px;");

    startButton = new QPushButton(tr("Start"), this);  // ������ "�����"
    startButton->setStyleSheet("font-size: 18px;");

    // ��������� �������� ���������� � ������������ ������
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(titleLabel);
    layout->addWidget(authorLabel);
    layout->addStretch();  // �������� ��� ���������
    layout->addWidget(startButton);

    // ���������� ������ �� ������ � ������� startClicked
    connect(startButton, &QPushButton::clicked, this, &SplashScreen::startClicked);
}
