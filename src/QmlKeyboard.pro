TEMPLATE = app

QT += qml quick widgets

CONFIG += release qml_release 

#Additional import path used to resolve QML modules in Qt Creator code model
QML_IMPORT_PATH =

TEMPLATE = app
TARGET = QmlKeyboard

DESTDIR = ../bin
UI_DIR = ../form
RCC_DIR = ../qrc
MOC_DIR = ../moc
OBJECTS_DIR = ../obj

include(./Keyboard/Keyboard.pri)

SOURCES += main.cpp

RESOURCES += qml.qrc
