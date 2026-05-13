#include <QTest>
#include <QByteArray>
#include <QString>
#include <QCoreApplication>

#include "../Services/Comm/CommManager.h"
#include "../Services/Comm/CommConfig.h"
#include "../Services/Comm/CommProtocol.h"
#include "../Services/Comm/UdpComm.h"
#include "../Services/Comm/TcpServerComm.h"

class CommTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase() {
        qDebug() << "Initializing CommTest...";
    }

    void cleanupTestCase() {
        qDebug() << "Cleaning up CommTest...";
    }

    void testCommManagerSingleton() {
        Services::Comm::CommManager* instance1 = Services::Comm::CommManager::getInstance();
        Services::Comm::CommManager* instance2 = Services::Comm::CommManager::getInstance();

        QVERIFY(instance1 != nullptr);
        QVERIFY(instance2 != nullptr);
        QCOMPARE(instance1, instance2);
    }

    void testProtocolSwitching() {
        Services::Comm::CommManager* manager = Services::Comm::CommManager::getInstance();

        QCOMPARE(manager->getCurrentProtocol(), Services::Comm::CommProtocol::Serial);

        manager->setProtocol(Services::Comm::CommProtocol::TcpClient);
        QCOMPARE(manager->getCurrentProtocol(), Services::Comm::CommProtocol::TcpClient);

        manager->setProtocol(Services::Comm::CommProtocol::TcpServer);
        QCOMPARE(manager->getCurrentProtocol(), Services::Comm::CommProtocol::TcpServer);

        manager->setProtocol(Services::Comm::CommProtocol::Udp);
        QCOMPARE(manager->getCurrentProtocol(), Services::Comm::CommProtocol::Udp);

        manager->setProtocol(Services::Comm::CommProtocol::Serial);
        QCOMPARE(manager->getCurrentProtocol(), Services::Comm::CommProtocol::Serial);
    }

    void testCommConfig() {
        Services::Comm::CommConfig config;

        config.protocolType = "Serial";
        config.serialPort = "COM1";
        config.baudRate = 115200;
        config.dataBits = 8;
        config.parity = "None";
        config.stopBits = 1;

        QCOMPARE(config.protocolType, QString("Serial"));
        QCOMPARE(config.serialPort, QString("COM1"));
        QCOMPARE(config.baudRate, 115200);
        QCOMPARE(config.dataBits, 8);
        QCOMPARE(config.parity, QString("None"));
        QCOMPARE(config.stopBits, 1);

        config.tcpHost = "192.168.1.100";
        config.tcpPort = 8080;
        QCOMPARE(config.tcpHost, QString("192.168.1.100"));
        QCOMPARE(config.tcpPort, 8080);

        config.udpLocalPort = 5000;
        config.udpRemotePort = 5001;
        QCOMPARE(config.udpLocalPort, 5000);
        QCOMPARE(config.udpRemotePort, 5001);
    }

    void testConfigPersistence() {
        Services::Comm::CommConfig config;
        config.protocolType = "TcpClient";
        config.tcpHost = "127.0.0.1";
        config.tcpPort = 9090;
        config.hexMode = true;
        config.appendNewLine = true;
        config.newLineType = "\n";

        QVariantMap map = config.toVariantMap();

        QCOMPARE(map["protocolType"].toString(), QString("TcpClient"));
        QCOMPARE(map["tcpHost"].toString(), QString("127.0.0.1"));
        QCOMPARE(map["tcpPort"].toInt(), 9090);
        QCOMPARE(map["hexMode"].toBool(), true);
        QCOMPARE(map["appendNewLine"].toBool(), true);
        QCOMPARE(map["newLineType"].toString(), QString("\n"));

        Services::Comm::CommConfig config2;
        config2.fromVariantMap(map);

        QCOMPARE(config2.protocolType, QString("TcpClient"));
        QCOMPARE(config2.tcpHost, QString("127.0.0.1"));
        QCOMPARE(config2.tcpPort, 9090);
        QCOMPARE(config2.hexMode, true);
        QCOMPARE(config2.appendNewLine, true);
        QCOMPARE(config2.newLineType, QString("\n"));
    }

    void testProtocolToString() {
        using namespace Services::Comm;
        QCOMPARE(protocolToString(CommProtocol::Serial), QString("Serial"));
        QCOMPARE(protocolToString(CommProtocol::TcpClient), QString("TcpClient"));
        QCOMPARE(protocolToString(CommProtocol::TcpServer), QString("TcpServer"));
        QCOMPARE(protocolToString(CommProtocol::Udp), QString("Udp"));
    }

    void testStringToProtocol() {
        using namespace Services::Comm;
        QCOMPARE(stringToProtocol("Serial"), CommProtocol::Serial);
        QCOMPARE(stringToProtocol("TcpClient"), CommProtocol::TcpClient);
        QCOMPARE(stringToProtocol("TcpServer"), CommProtocol::TcpServer);
        QCOMPARE(stringToProtocol("Udp"), CommProtocol::Udp);
        QCOMPARE(stringToProtocol("Unknown"), CommProtocol::Serial);
    }

    void testUdpBinding() {
        Services::Comm::UdpComm udp;
        Services::Comm::CommConfig config;
        config.udpLocalAddr = "0.0.0.0";
        config.udpLocalPort = 55555;
        udp.setConfig(config);

        bool result = udp.connect();
        QString error = udp.getErrorString();
        qDebug() << "UDP bind result:" << result << ", error:" << error;
        QVERIFY2(result, qPrintable(error));
        QVERIFY(udp.isConnected());

        QString info = udp.getConnectionInfo();
        QVERIFY(info.contains("0.0.0.0"));
        QVERIFY(info.contains("55555"));

        bool disconnected = udp.disconnect();
        QVERIFY(disconnected);
        QVERIFY(!udp.isConnected());
    }

    void testTcpServerStart() {
        Services::Comm::TcpServerComm server;
        Services::Comm::CommConfig config;
        config.listenPort = 12346;
        server.setConfig(config);

        bool result = server.connect();
        QVERIFY(result);
        QVERIFY(server.isConnected());

        QString info = server.getConnectionInfo();
        QVERIFY(info.contains("12346"));
        QVERIFY(info.contains("0 clients"));

        bool disconnected = server.disconnect();
        QVERIFY(disconnected);
        QVERIFY(!server.isConnected());
    }

    void testSendDataWithoutConnection() {
        Services::Comm::CommManager* manager = Services::Comm::CommManager::getInstance();
        manager->setProtocol(Services::Comm::CommProtocol::Serial);

        qint64 result = manager->sendData("test");
        QCOMPARE(result, qint64(-1));

        QString error = manager->getErrorString();
        QVERIFY(!error.isEmpty());
    }

    void testConnectionInfo() {
        Services::Comm::CommManager* manager = Services::Comm::CommManager::getInstance();

        QString info = manager->getConnectionInfo();
        QVERIFY(info.contains("Not connected") || !info.isEmpty());
    }

    void testClientCount() {
        Services::Comm::CommManager* manager = Services::Comm::CommManager::getInstance();
        manager->setProtocol(Services::Comm::CommProtocol::TcpServer);

        QCOMPARE(manager->getClientCount(), 0);

        Services::Comm::CommConfig config;
        config.listenPort = 12347;
        manager->setConfig(config);
        manager->connect();

        QCOMPARE(manager->getClientCount(), 0);

        manager->disconnect();
    }
};

QTEST_MAIN(CommTest)
#include "CommTest.moc"
