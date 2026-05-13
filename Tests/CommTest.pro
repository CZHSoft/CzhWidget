QT += core network testlib serialport
QT -= gui

CONFIG += c++20
CONFIG += testcase

TARGET = CommTest
TEMPLATE = app

INCLUDEPATH += $$PWD/..

SOURCES += \
    CommTest.cpp \
    ../Services/Comm/SerialComm.cpp \
    ../Services/Comm/TcpClientComm.cpp \
    ../Services/Comm/TcpServerComm.cpp \
    ../Services/Comm/UdpComm.cpp \
    ../Services/Comm/CommFactory.cpp \
    ../Services/Comm/CommManager.cpp

HEADERS += \
    ../Services/Comm/CommProtocol.h \
    ../Services/Comm/CommConfig.h \
    ../Services/Comm/ICommInterface.h \
    ../Services/Comm/SerialComm.h \
    ../Services/Comm/TcpClientComm.h \
    ../Services/Comm/TcpServerComm.h \
    ../Services/Comm/UdpComm.h \
    ../Services/Comm/CommFactory.h \
    ../Services/Comm/CommManager.h
