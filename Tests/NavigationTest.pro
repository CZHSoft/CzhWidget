QT += core gui widgets testlib

TARGET = NavigationTest
TEMPLATE = app

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

INCLUDEPATH += \
    ..

SOURCES += \
    NavigationTest.cpp

HEADERS +=

CONFIG += c++2a
CONFIG += warn_on
CONFIG += console