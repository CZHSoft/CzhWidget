#include <QTest>
#include <QString>
#include <QMap>
#include <QWidget>

class NavigationTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase() {
        qDebug() << "Initializing NavigationTest...";
    }

    void cleanupTestCase() {
        qDebug() << "Cleaning up NavigationTest...";
    }

    void testPageKeyMapping() {
        QMap<QString, QString> pageNameToKeyMap;
        
        QString dashKey = "test-dash-key";
        QString monitorKey = "test-monitor-key";
        QString protocolKey = "test-protocol-key";
        QString toolsKey = "test-tools-key";
        QString settingKey = "test-setting-key";
        QString logsKey = "test-logs-key";
        QString aboutKey = "test-about-key";

        pageNameToKeyMap["Dashboard"] = dashKey;
        pageNameToKeyMap["Monitor"] = monitorKey;
        pageNameToKeyMap["Protocol"] = protocolKey;
        pageNameToKeyMap["Tools"] = toolsKey;
        pageNameToKeyMap["Setting"] = settingKey;
        pageNameToKeyMap["Logs"] = logsKey;
        pageNameToKeyMap["About"] = aboutKey;

        QCOMPARE(pageNameToKeyMap.contains("Dashboard"), true);
        QCOMPARE(pageNameToKeyMap.contains("Monitor"), true);
        QCOMPARE(pageNameToKeyMap.contains("Protocol"), true);
        QCOMPARE(pageNameToKeyMap.contains("Tools"), true);
        QCOMPARE(pageNameToKeyMap.contains("Setting"), true);
        QCOMPARE(pageNameToKeyMap.contains("Logs"), true);
        QCOMPARE(pageNameToKeyMap.contains("About"), true);

        QCOMPARE(pageNameToKeyMap["Dashboard"], dashKey);
        QCOMPARE(pageNameToKeyMap["Monitor"], monitorKey);
        QCOMPARE(pageNameToKeyMap["Tools"], toolsKey);

        QCOMPARE(pageNameToKeyMap.contains("Unknown"), false);
    }

    void testEmptyKeyValidation() {
        QString emptyKey = "";
        
        QVERIFY(emptyKey.isEmpty());
        QVERIFY(!emptyKey.isNull());

        QString validKey = "valid-uuid-key";
        QVERIFY(!validKey.isEmpty());
        QVERIFY(!validKey.isNull());
    }

    void testPageNameValidation() {
        QStringList validPageNames = {"Dashboard", "Monitor", "Protocol", "Tools", "Setting", "Logs", "About"};
        QStringList invalidPageNames = {"dashboard", "MONITOR", "Protocolv2", "Tool", "Settings", "Log", "Abouts", "Unknown"};

        QMap<QString, QString> pageNameToKeyMap = {
            {"Dashboard", "key1"},
            {"Monitor", "key2"},
            {"Protocol", "key3"},
            {"Tools", "key4"},
            {"Setting", "key5"},
            {"Logs", "key6"},
            {"About", "key7"}
        };

        for (const QString& name : validPageNames) {
            QVERIFY(pageNameToKeyMap.contains(name));
        }

        for (const QString& name : invalidPageNames) {
            QVERIFY(!pageNameToKeyMap.contains(name));
        }
    }

    void testKeyExtractionFromWidget() {
        QWidget* widget = new QWidget();
        QString testKey = "test-widget-key";
        
        widget->setProperty("ElaPageKey", testKey);
        
        QString extractedKey = widget->property("ElaPageKey").toString();
        
        QCOMPARE(extractedKey, testKey);
        
        delete widget;
    }

    void testKeyExtractionFromWidgetWithNullProperty() {
        QWidget* widget = new QWidget();
        
        QString extractedKey = widget->property("ElaPageKey").toString();
        
        QVERIFY(extractedKey.isEmpty());
        
        delete widget;
    }

    void testNavigationParameterValidation() {
        QMap<QString, QString> pageNameToKeyMap = {
            {"Dashboard", "key1"},
            {"Monitor", "key2"},
            {"Tools", "key3"}
        };

        QString testPageName = "Monitor";
        
        bool isValid = pageNameToKeyMap.contains(testPageName);
        QVERIFY(isValid);
        
        QString pageKey = pageNameToKeyMap.value(testPageName);
        QVERIFY(!pageKey.isEmpty());
        QCOMPARE(pageKey, QString("key2"));

        testPageName = "Unknown";
        isValid = pageNameToKeyMap.contains(testPageName);
        QVERIFY(!isValid);
    }

    void testUuidFormatValidation() {
        QString validUuid = "6e304ecbfb1b47aaa1953b1b7108cb8a";
        
        QVERIFY(validUuid.length() == 32);
        
        bool isHex = true;
        for (QChar c : validUuid) {
            if (!c.isDigit() && !((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))) {
                isHex = false;
                break;
            }
        }
        QVERIFY(isHex);

        QString invalidUuid = "invalid-uuid";
        QVERIFY(invalidUuid.length() != 32);
    }
};

QTEST_MAIN(NavigationTest)
#include "NavigationTest.moc"