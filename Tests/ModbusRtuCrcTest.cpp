#include <QTest>
#include <QByteArray>
#include <QString>
#include <QDebug>

#include "../Services/Protocol/ModbusRtuParser.h"

class ModbusRtuCrcTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase() {
        qDebug() << "=== Modbus RTU CRC校验测试初始化 ===";
    }

    void cleanupTestCase() {
        qDebug() << "=== Modbus RTU CRC校验测试完成 ===";
    }

    void testCrcValidation_data() {
        QTest::addColumn<QString>("hexData");
        QTest::addColumn<bool>("expectedResult");
        QTest::addColumn<QString>("description");

        QTest::newRow("Test Case 1: Read Holding Register (01 03 00 00 00 01 84 0A)")
            << "01 03 00 00 00 01 84 0A" << true << "读保持寄存器，地址0，数量1";

        QTest::newRow("Test Case 2: Read Holding Register (01 03 00 00 00 02 C4 0B)")
            << "01 03 00 00 00 02 C4 0B" << true << "读保持寄存器，地址0，数量2";

        QTest::newRow("Test Case 3: Read Input Register (01 04 00 00 00 01 31 CA)")
            << "01 04 00 00 00 01 31 CA" << true << "读输入寄存器，地址0，数量1";

        QTest::newRow("Test Case 4: Write Single Register (01 06 00 01 00 64 D9 E1)")
            << "01 06 00 01 00 64 D9 E1" << true << "写单个寄存器，地址1，值100";

        QTest::newRow("Test Case 5: Invalid CRC - Wrong last byte")
            << "01 03 00 00 00 01 84 0B" << false << "CRC最后一字节错误";

        QTest::newRow("Test Case 6: Invalid CRC - Both bytes wrong")
            << "01 03 00 00 00 01 FF FF" << false << "CRC全部错误";

        QTest::newRow("Test Case 7: Short data (no CRC)")
            << "01 03 00 00" << false << "数据长度不足，缺少CRC";
    }

    void testCrcValidation() {
        QFETCH(QString, hexData);
        QFETCH(bool, expectedResult);
        QFETCH(QString, description);

        qDebug() << "\n--- 测试用例: " << description;
        qDebug() << "输入数据: " << hexData;
        qDebug() << "预期结果: " << (expectedResult ? "CRC校验通过" : "CRC校验失败");

        QString cleanHex = hexData;
        cleanHex.remove(QRegularExpression("[\\s]"));
        QByteArray rawData = QByteArray::fromHex(cleanHex.toUtf8());

        Services::Protocol::ModbusRtuParser parser;
        Services::Protocol::ParseResult result = parser.parse(rawData);

        qDebug() << "实际结果: " << (result.success ? "解析成功" : "解析失败");
        if (!result.success) {
            qDebug() << "错误信息: " << result.errorMessage;
        }

        bool actualResult = result.success;
        QCOMPARE(actualResult, expectedResult);

        if (actualResult == expectedResult) {
            qDebug() << "[PASS] 测试通过";
        } else {
            qDebug() << "[FAIL] 测试失败";
        }
    }

    void testParseResultDetails_data() {
        QTest::addColumn<QString>("hexData");
        QTest::addColumn<int>("expectedSlaveAddress");
        QTest::addColumn<int>("expectedFunctionCode");
        QTest::addColumn<QString>("expectedFunctionName");

        QTest::newRow("Write Single Register")
            << "01 06 00 01 00 64 D9 E1" << 1 << 6 << "写单个寄存器";

        QTest::newRow("Read Input Register")
            << "01 04 00 00 00 01 31 CA" << 1 << 4 << "读输入寄存器";
    }

    void testParseResultDetails() {
        QFETCH(QString, hexData);
        QFETCH(int, expectedSlaveAddress);
        QFETCH(int, expectedFunctionCode);
        QFETCH(QString, expectedFunctionName);

        QString cleanHex = hexData;
        cleanHex.remove(QRegularExpression("[\\s]"));
        QByteArray rawData = QByteArray::fromHex(cleanHex.toUtf8());

        Services::Protocol::ModbusRtuParser parser;
        Services::Protocol::ParseResult result = parser.parse(rawData);

        QVERIFY(result.success);

        QCOMPARE(result.parsedData[Services::Protocol::ParseResult::KEY_SLAVE_ADDRESS].toInt(), expectedSlaveAddress);
        QCOMPARE(result.parsedData[Services::Protocol::ParseResult::KEY_FUNCTION_CODE].toInt(), expectedFunctionCode);
        QCOMPARE(result.parsedData[Services::Protocol::ParseResult::KEY_FUNCTION_NAME].toString(), expectedFunctionName);

        qDebug() << "格式化结果:\n" << result.formattedResult;
    }

    void testFormattedOutput() {
        QString hexData = "01 03 00 00 00 01 84 0A";
        QString cleanHex = hexData;
        cleanHex.remove(QRegularExpression("[\\s]"));
        QByteArray rawData = QByteArray::fromHex(cleanHex.toUtf8());

        Services::Protocol::ModbusRtuParser parser;
        Services::Protocol::ParseResult result = parser.parse(rawData);

        QVERIFY(result.success);
        QVERIFY(!result.formattedResult.isEmpty());

        qDebug() << "\n=== 格式化输出示例 ===";
        qDebug() << result.formattedResult;
    }

    void testCrcAlgorithm() {
        qDebug() << "\n=== CRC16算法验证 ===";

        QByteArray testData1 = QByteArray::fromHex("010300000001");
        uint16_t crc1 = calculateCrc16(testData1);
        qDebug() << "数据 01 03 00 00 00 01 的CRC: 0x" << QString("%1").arg(crc1, 4, 16, QChar('0')).toUpper();
        qDebug() << "  小端序字节: " << QString("%1 %2").arg(static_cast<int>(crc1 & 0xFF), 2, 16, QChar('0')).arg(static_cast<int>(crc1 >> 8), 2, 16, QChar('0')).toUpper();
        QCOMPARE(crc1, (uint16_t)0x0A84);

        QByteArray testData2 = QByteArray::fromHex("010300000002");
        uint16_t crc2 = calculateCrc16(testData2);
        qDebug() << "数据 01 03 00 00 00 02 的CRC: 0x" << QString("%1").arg(crc2, 4, 16, QChar('0')).toUpper();
        qDebug() << "  小端序字节: " << QString("%1 %2").arg(static_cast<int>(crc2 & 0xFF), 2, 16, QChar('0')).arg(static_cast<int>(crc2 >> 8), 2, 16, QChar('0')).toUpper();
        QCOMPARE(crc2, (uint16_t)0x0BC4);

        QByteArray testData3 = QByteArray::fromHex("010400000001");
        uint16_t crc3 = calculateCrc16(testData3);
        qDebug() << "数据 01 04 00 00 00 01 的CRC: 0x" << QString("%1").arg(crc3, 4, 16, QChar('0')).toUpper();
        qDebug() << "  小端序字节: " << QString("%1 %2").arg(static_cast<int>(crc3 & 0xFF), 2, 16, QChar('0')).arg(static_cast<int>(crc3 >> 8), 2, 16, QChar('0')).toUpper();
        QCOMPARE(crc3, (uint16_t)0xCA31);

        QByteArray testData4 = QByteArray::fromHex("010600010064");
        uint16_t crc4 = calculateCrc16(testData4);
        qDebug() << "数据 01 06 00 01 00 64 的CRC: 0x" << QString("%1").arg(crc4, 4, 16, QChar('0')).toUpper();
        qDebug() << "  小端序字节: " << QString("%1 %2").arg(static_cast<int>(crc4 & 0xFF), 2, 16, QChar('0')).arg(static_cast<int>(crc4 >> 8), 2, 16, QChar('0')).toUpper();
        QCOMPARE(crc4, (uint16_t)0xE1D9);
    }

private:
    uint16_t calculateCrc16(const QByteArray& data) {
        uint16_t crc = 0xFFFF;
        for (int i = 0; i < data.size(); ++i) {
            crc ^= static_cast<uint8_t>(data[i]);
            for (int j = 0; j < 8; ++j) {
                crc = (crc >> 1) ^ ((crc & 0x01) ? 0xA001 : 0);
            }
        }
        return crc;
    }
};

QTEST_MAIN(ModbusRtuCrcTest)
#include "ModbusRtuCrcTest.moc"