#include <QApplication>
#include <QQmlApplicationEngine>

#include <QWindow>

#include <QtQml>
#include <QFont>

#include "mykeyboard.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //Register C++ Classes in Qml Enviroment
    qmlRegisterType<MyKeyboard>("mykeyboard", 1, 0, "MyKeyboard");

    QQmlApplicationEngine *engine = new QQmlApplicationEngine();
    engine->load(QUrl(QStringLiteral("qrc:///main.qml")));

    int result = app.exec();

    delete engine;

    return result;
}
