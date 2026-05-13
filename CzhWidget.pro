QT       += core gui network serialport

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

CONFIG += c++20

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    Models/MonitorModel.cpp \
    Models/AutoSendModel.cpp \
    Models/CommandTableModel.cpp \
    Presenters/MonitorPresenter.cpp \
    Presenters/AutoSendPresenter.cpp \
    Views/Panels/WaveformPanel.cpp \
    main.cpp \
    Views/MainWindow.cpp \
    Views/SplashWindow.cpp \
    Views/Pages/DashboardPage.cpp \
    Views/Pages/SettingPage.cpp \
    Views/Pages/AboutPage.cpp \
    Views/Pages/LogsPage.cpp \
    Views/Pages/MonitorPage.cpp \
    Views/Pages/ProtocolPage.cpp \
    Views/Pages/ToolsPage.cpp \
    Views/Panels/AutoSendPanel.cpp \
    Presenters/MainPresenter.cpp \
    Models/AppModel.cpp \
    Services/Log/LogService.cpp \
    Services/Comm/SerialComm.cpp \
    Services/Comm/TcpClientComm.cpp \
    Services/Comm/TcpServerComm.cpp \
    Services/Comm/UdpComm.cpp \
    Services/Comm/CommFactory.cpp \
    Services/Comm/CommManager.cpp \
    Services/Protocol/ProtocolType.cpp \
    Services/Protocol/ProtocolParserFactory.cpp \
    Services/Protocol/ModbusRtuParser.cpp \
    Services/Protocol/ModbusTcpParser.cpp \
    Services/Protocol/CanParser.cpp \
    Services/Protocol/CanOpenParser.cpp \
    Services/I18n/LanguageManager.cpp \
    Widgets/ToggleSwitch.cpp \
    Widgets/ComboBox.cpp \
    Widgets/GroupBox.cpp \
    Widgets/WaveformPlotter.cpp \
    Widgets/FunctionNavCard.cpp

HEADERS += \
    Models/MonitorModel.h \
    Models/AutoSendModel.h \
    Models/CommandTableModel.h \
    Presenters/MonitorPresenter.h \
    Presenters/AutoSendPresenter.h \
    Views/Panels/WaveformPanel.h \
    Views/SplashWindow.h \
    Views/MainWindow.h \
    Views/Pages/DashboardPage.h \
    Views/Pages/SettingPage.h \
    Views/Pages/AboutPage.h \
    Views/Pages/LogsPage.h \
    Views/Pages/MonitorPage.h \
    Views/Pages/ProtocolPage.h \
    Views/Pages/ToolsPage.h \
    Views/Panels/AutoSendPanel.h \
    Presenters/MainPresenter.h \
    Models/AppModel.h \
    Services/Log/LogService.h \
    Services/Comm/CommProtocol.h \
    Services/Comm/CommConfig.h \
    Services/Comm/ICommInterface.h \
    Services/Comm/SerialComm.h \
    Services/Comm/TcpClientComm.h \
    Services/Comm/TcpServerComm.h \
    Services/Comm/UdpComm.h \
    Services/Comm/CommFactory.h \
    Services/Comm/CommManager.h \
    Services/Protocol/ProtocolType.h \
    Services/Protocol/ParseResult.h \
    Services/Protocol/ProtocolParser.h \
    Services/Protocol/ProtocolParserFactory.h \
    Services/Protocol/ModbusRtuParser.h \
    Services/Protocol/ModbusTcpParser.h \
    Services/Protocol/CanParser.h \
    Services/Protocol/CanOpenParser.h \
    Services/I18n/LanguageManager.h \
    Widgets/ToggleSwitch.h \
    Widgets/ComboBox.h \
    Widgets/GroupBox.h \
    Widgets/WaveformPlotter.h \
    Widgets/FunctionNavCard.h

INCLUDEPATH += \
    . \
    ElaWidgetTools/include \
    Log4Qt/include \
    Widgets \
    Models \
    Presenters \
    Views/Pages \
    Views/Panels \
    Views/Widgets \
    Services/Log \
    Services/Comm \
    Services/Protocol \
    Services/I18n

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


win32 {
    # 依赖库路径
    LIBS += -L$$PWD/ElaWidgetTools/lib -lElaWidgetTools
    LIBS += -L$$PWD/log4qt/lib -llog4qt

    # 输出目录
    DESTDIR = $$PWD/debug

    # 复制 DLL
    QMAKE_POST_LINK += copy /Y \"$$PWD/ElaWidgetTools/lib/ElaWidgetTools.dll\" \"$$DESTDIR\\\" 2>nul || exit 0
    QMAKE_POST_LINK += copy /Y \"$$PWD/Log4Qt/lib/liblog4qt.dll\" \"$$DESTDIR\\\" 2>nul || exit 0

    # 复制 log4qt.properties 配置文件到输出目录
    QMAKE_POST_LINK += copy /Y \"$$PWD/log4qt.properties\" \"$$DESTDIR\\\" 2>nul || exit 0

    # 应用程序图标
    RC_ICONS = Resources/pic/logo.ico
}

TRANSLATIONS += \
    CzhWidget_ja_JP.ts \
    CzhWidget_ko_KR.ts \
    CzhWidget_zh_CN.ts \
    CzhWidget_en.ts

RESOURCES += \
   Resources.qrc \
   i18n.qrc
