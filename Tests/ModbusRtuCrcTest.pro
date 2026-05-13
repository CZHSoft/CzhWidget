QT += core testlib
QT -= gui

CONFIG += c++20
CONFIG += testcase

TARGET = ModbusRtuCrcTest
TEMPLATE = app

INCLUDEPATH += $$PWD/..

SOURCES += \
    ModbusRtuCrcTest.cpp \
    ../Services/Protocol/ProtocolType.cpp \
    ../Services/Protocol/ProtocolParserFactory.cpp \
    ../Services/Protocol/ModbusRtuParser.cpp \
    ../Services/Protocol/ModbusTcpParser.cpp \
    ../Services/Protocol/CanParser.cpp \
    ../Services/Protocol/CanOpenParser.cpp

HEADERS += \
    ../Services/Protocol/ProtocolType.h \
    ../Services/Protocol/ParseResult.h \
    ../Services/Protocol/ProtocolParser.h \
    ../Services/Protocol/ProtocolParserFactory.h \
    ../Services/Protocol/ModbusRtuParser.h \
    ../Services/Protocol/ModbusTcpParser.h \
    ../Services/Protocol/CanParser.h \
    ../Services/Protocol/CanOpenParser.h