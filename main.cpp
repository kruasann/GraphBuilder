// main.cpp
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QPalette>
#include <QIcon>
#include "GraphWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    app.setWindowIcon(QIcon(":/images/GraphBuilderLogo.png"));

    // ���������� ����� Fusion � ��������� ��������
    app.setStyle("Fusion");

    QPalette palette;
    palette.setColor(QPalette::Window, QColor(53, 53, 53));
    palette.setColor(QPalette::WindowText, Qt::white);
    palette.setColor(QPalette::Base, QColor(25, 25, 25));
    palette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    palette.setColor(QPalette::ToolTipBase, Qt::white);
    palette.setColor(QPalette::ToolTipText, Qt::white);
    palette.setColor(QPalette::Text, Qt::white);
    palette.setColor(QPalette::Button, QColor(53, 53, 53));
    palette.setColor(QPalette::ButtonText, Qt::white);
    palette.setColor(QPalette::BrightText, Qt::red);
    palette.setColor(QPalette::Highlight, QColor(142, 45, 197).lighter());
    palette.setColor(QPalette::HighlightedText, Qt::black);
    app.setPalette(palette);

    // ������� ����������
    QTranslator translator;
    // ��������� ���� ��������. ��������������, ��� ���� .qm ��������� � ��� �� ����������, ��� � ����������� ����
    if (translator.load("myapp_ru.qm")) {
        app.installTranslator(&translator);
    }
    else {
        qDebug() << "�� ������� ��������� ���� ��������.";
    }

    GraphWindow window;
    window.show();
    return app.exec();
}
