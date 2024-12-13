// main.cpp
#include <QApplication>  // Подключение класса QApplication для создания и управления приложением
#include <QTranslator>    // Подключение класса QTranslator для перевода приложения
#include <QLocale>        // Подключение класса QLocale для работы с локализацией
#include <QLibraryInfo>   // Подключение класса QLibraryInfo для получения информации о библиотеке
#include <QPalette>       // Подключение класса QPalette для настройки палитры цветов интерфейса
#include <QIcon>          // Подключение класса QIcon для установки иконки окна
#include "GraphWindow.h"  // Подключение заголовочного файла для главного окна приложения

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);  // Создание объекта приложения с аргументами командной строки

    app.setWindowIcon(QIcon(":/images/GraphBuilderLogo.png"));  // Установка иконки окна приложения

    // Применение стиля Fusion с кастомной палитрой
    app.setStyle("Fusion");  // Установка стиля интерфейса приложения (Fusion)

    QPalette palette;  // Создание объекта палитры для настройки цветов
    palette.setColor(QPalette::Window, QColor(53, 53, 53));  // Установка цвета фона окна
    palette.setColor(QPalette::WindowText, Qt::white);  // Установка цвета текста окна
    palette.setColor(QPalette::Base, QColor(25, 25, 25));  // Установка цвета для текстовых полей
    palette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));  // Установка цвета для альтернативного фона
    palette.setColor(QPalette::ToolTipBase, Qt::white);  // Установка цвета фона для подсказок
    palette.setColor(QPalette::ToolTipText, Qt::white);  // Установка цвета текста для подсказок
    palette.setColor(QPalette::Text, Qt::white);  // Установка цвета текста в полях ввода
    palette.setColor(QPalette::Button, QColor(53, 53, 53));  // Установка цвета фона для кнопок
    palette.setColor(QPalette::ButtonText, Qt::white);  // Установка цвета текста для кнопок
    palette.setColor(QPalette::BrightText, Qt::red);  // Установка яркого цвета для текста (красный)
    palette.setColor(QPalette::Highlight, QColor(142, 45, 197).lighter());  // Установка цвета выделенного текста
    palette.setColor(QPalette::HighlightedText, Qt::black);  // Установка цвета текста в выделенных элементах
    app.setPalette(palette);  // Применение кастомной палитры к приложению

    // Создаем переводчик
    QTranslator translator;  // Создание объекта переводчика
    // Загружаем файл перевода. Предполагается, что файл .qm находится в той же директории, что и исполняемый файл
    if (translator.load("myapp_ru.qm")) {  // Если удалось загрузить файл перевода
        app.installTranslator(&translator);  // Устанавливаем переводчик в приложение
    }
    else {
        qDebug() << "Не удалось загрузить файл перевода.";  // Выводим сообщение об ошибке, если файл перевода не найден
    }

    GraphWindow window;  // Создание объекта главного окна приложения
    window.show();  // Отображение главного окна
    return app.exec();  // Запуск основного цикла приложения
}
