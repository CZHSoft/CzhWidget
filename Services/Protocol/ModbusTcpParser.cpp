#include "ModbusTcpParser.h"
#include <QDateTime>
#include <QVariantList>

namespace Services {
namespace Protocol {

ParseResult ModbusTcpParser::parse(const QByteArray& data)
{
    ParseResult result;
    result.protocolType = getProtocolType();
    result.protocolName = getProtocolName();
    result.parseTime = QDateTime::currentDateTime();
    result.rawData = data;

    if (data.size() < 7) {
        result.success = false;
        result.errorMessage = "数据长度不足，至少需要7字节(MBAP头6字节+PDU最小1字节)";
        result.formattedResult = QString("[解析失败] %1").arg(result.errorMessage);
        return result;
    }

    uint16_t transactionId = (static_cast<uint8_t>(data[0]) << 8) | static_cast<uint8_t>(data[1]);
    uint16_t protocolId = (static_cast<uint8_t>(data[2]) << 8) | static_cast<uint8_t>(data[3]);
    uint16_t pduLength = (static_cast<uint8_t>(data[4]) << 8) | static_cast<uint8_t>(data[5]);
    uint8_t unitId = static_cast<uint8_t>(data[6]);

    if (protocolId != 0) {
        result.success = false;
        result.errorMessage = QString("无效的协议ID: 0x%1，Modbus TCP协议ID应为0").arg(protocolId, 4, 16, QChar('0'));
        result.formattedResult = QString("[解析失败] %1").arg(result.errorMessage);
        return result;
    }

    if (data.size() != 6 + pduLength) {
        result.success = false;
        result.errorMessage = QString("数据长度不匹配，期望%1字节，实际%2字节").arg(6 + pduLength).arg(data.size());
        result.formattedResult = QString("[解析失败] %1").arg(result.errorMessage);
        return result;
    }

    QByteArray pduData = data.mid(7);
    uint8_t functionCode = static_cast<uint8_t>(pduData[0]);

    result.parsedData[ParseResult::KEY_TRANSACTION_ID] = transactionId;
    result.parsedData[ParseResult::KEY_PROTOCOL_ID] = protocolId;
    result.parsedData[ParseResult::KEY_PDU_LENGTH] = pduLength;
    result.parsedData[ParseResult::KEY_NODE_ID] = unitId;
    result.parsedData[ParseResult::KEY_SLAVE_ADDRESS] = unitId;
    result.parsedData[ParseResult::KEY_FUNCTION_CODE] = functionCode;

    QString formatted = QString(
        "【Modbus TCP 帧解析】\n"
        "MBAP 头部:\n"
        "  事务处理标识: %1 (0x%2)\n"
        "  协议标识: %3 (0x%4)\n"
        "  长度: %5 字节\n"
        "  单元标识符: %6 (0x%7)\n"
    ).arg(transactionId).arg(transactionId, 4, 16, QChar('0'))
     .arg(protocolId).arg(protocolId, 4, 16, QChar('0'))
     .arg(pduLength).arg(unitId).arg(unitId, 2, 16, QChar('0'));

    if (functionCode >= 0x80) {
        uint8_t exceptionCode = static_cast<uint8_t>(pduData[1]);
        result.parsedData[ParseResult::KEY_FUNCTION_NAME] = QString("异常响应 (0x%1)").arg(functionCode - 0x80, 2, 16, QChar('0'));
        result.parsedData[ParseResult::KEY_ERROR_CODE] = exceptionCode;
        result.parsedData[ParseResult::KEY_ERROR_DESCRIPTION] = modbusErrorToString(exceptionCode);

        formatted += QString(
            "PDU (异常响应):\n"
            "  功能码: 0x%1 (异常)\n"
            "  异常码: 0x%2\n"
            "  异常描述: %3"
        ).arg(functionCode, 2, 16, QChar('0'))
         .arg(exceptionCode, 2, 16, QChar('0'))
         .arg(modbusErrorToString(exceptionCode));

        result.success = true;
        result.formattedResult = formatted;
        return result;
    }

    QString funcName = functionCodeToString(functionCode);
    result.parsedData[ParseResult::KEY_FUNCTION_NAME] = funcName;

    formatted += QString("\nPDU:\n  功能码: 0x%1 - %2").arg(functionCode, 2, 16, QChar('0')).arg(funcName);

    QByteArray pduPayload = pduData.mid(1);

    switch (functionCode) {
    case 0x01:
    case 0x02: {
        int byteCount = static_cast<uint8_t>(pduPayload[0]);
        QVariantList values;
        QString bitsStr;
        for (int i = 0; i < byteCount && i + 1 < pduPayload.size(); ++i) {
            uint8_t byteVal = static_cast<uint8_t>(pduPayload[i + 1]);
            for (int j = 7; j >= 0; --j) {
                bool bit = (byteVal >> j) & 0x01;
                values.append(bit);
                bitsStr += bit ? "1" : "0";
                if (j > 0) bitsStr += " ";
            }
            if (i < byteCount - 1) bitsStr += "\n          ";
        }
        result.parsedData[ParseResult::KEY_DATA_LENGTH] = byteCount * 8;
        result.parsedData[ParseResult::KEY_DATA_VALUES] = values;
        formatted += QString("\n  字节计数: %1\n  线圈/离散输入状态:\n          %2").arg(byteCount).arg(bitsStr);
        break;
    }
    case 0x03:
    case 0x04: {
        int byteCount = static_cast<uint8_t>(pduPayload[0]);
        int registerCount = byteCount / 2;
        QVariantList values;
        QString regStr;
        for (int i = 0; i < registerCount && i * 2 + 1 < pduPayload.size(); ++i) {
            uint16_t value = (static_cast<uint8_t>(pduPayload[1 + i * 2]) << 8) |
                              static_cast<uint8_t>(pduPayload[2 + i * 2]);
            values.append(value);
            regStr += QString("寄存器%1: %2 (0x%3)\n          ").arg(i + 1).arg(value).arg(value, 4, 16, QChar('0'));
        }
        result.parsedData[ParseResult::KEY_DATA_LENGTH] = registerCount;
        result.parsedData[ParseResult::KEY_DATA_VALUES] = values;
        formatted += QString("\n  字节计数: %1\n  保持寄存器/输入寄存器值:\n          %2").arg(byteCount).arg(regStr);
        break;
    }
    case 0x05: {
        uint16_t outputAddress = (static_cast<uint8_t>(pduPayload[0]) << 8) |
                                  static_cast<uint8_t>(pduPayload[1]);
        uint16_t value = (static_cast<uint8_t>(pduPayload[2]) << 8) |
                          static_cast<uint8_t>(pduPayload[3]);
        QString state = (value == 0xFF00) ? "ON" : "OFF";
        result.parsedData[ParseResult::KEY_REGISTER_ADDRESS] = outputAddress;
        result.parsedData[ParseResult::KEY_DATA_VALUE] = state;
        formatted += QString("\n  输出线圈地址: %1\n  输出值: %2").arg(outputAddress).arg(state);
        break;
    }
    case 0x06: {
        uint16_t registerAddress = (static_cast<uint8_t>(pduPayload[0]) << 8) |
                                    static_cast<uint8_t>(pduPayload[1]);
        uint16_t value = (static_cast<uint8_t>(pduPayload[2]) << 8) |
                          static_cast<uint8_t>(pduPayload[3]);
        result.parsedData[ParseResult::KEY_REGISTER_ADDRESS] = registerAddress;
        result.parsedData[ParseResult::KEY_DATA_VALUE] = value;
        formatted += QString("\n  寄存器地址: %1\n  写入值: %2 (0x%3)").arg(registerAddress).arg(value).arg(value, 4, 16, QChar('0'));
        break;
    }
    case 0x07: {
        uint8_t status = static_cast<uint8_t>(pduPayload[0]);
        QString statusStr;
        if (status == 0x00) statusStr = "正常";
        else if (status == 0xFF) statusStr = "故障";
        else statusStr = QString("未知 (0x%1)").arg(status, 2, 16, QChar('0'));
        result.parsedData[ParseResult::KEY_DATA_VALUE] = statusStr;
        formatted += QString("\n  诊断状态: %1").arg(statusStr);
        break;
    }
    case 0x0F:
    case 0x10: {
        uint16_t startAddress = (static_cast<uint8_t>(pduPayload[0]) << 8) |
                                 static_cast<uint8_t>(pduPayload[1]);
        uint16_t quantity = (static_cast<uint8_t>(pduPayload[2]) << 8) |
                             static_cast<uint8_t>(pduPayload[3]);
        result.parsedData[ParseResult::KEY_REGISTER_ADDRESS] = startAddress;
        result.parsedData[ParseResult::KEY_DATA_LENGTH] = quantity;
        formatted += QString("\n  起始地址: %1\n  写入数量: %2").arg(startAddress).arg(quantity);
        break;
    }
    default:
        if (!pduPayload.isEmpty()) {
            formatted += QString("\n  数据字段: %1").arg(pduPayload.toHex(' '));
        }
    }

    result.success = true;
    result.formattedResult = formatted;
    return result;
}

bool ModbusTcpParser::canParse(const QByteArray& data) const
{
    if (data.size() < 7) return false;
    uint16_t protocolId = (static_cast<uint8_t>(data[2]) << 8) | static_cast<uint8_t>(data[3]);
    return protocolId == 0;
}

QString ModbusTcpParser::functionCodeToString(uint8_t code)
{
    switch (code) {
    case 0x01: return "读线圈状态";
    case 0x02: return "读离散输入";
    case 0x03: return "读保持寄存器";
    case 0x04: return "读输入寄存器";
    case 0x05: return "写单个线圈";
    case 0x06: return "写单个寄存器";
    case 0x07: return "读取异常状态";
    case 0x0B: return "获取事件计数器";
    case 0x0F: return "写多个线圈";
    case 0x10: return "写多个寄存器";
    default: return QString("未知功能码");
    }
}

QString ModbusTcpParser::modbusErrorToString(uint8_t exceptionCode)
{
    switch (exceptionCode) {
    case 0x01: return "非法功能码";
    case 0x02: return "非法数据地址";
    case 0x03: return "非法数据值";
    case 0x04: return "从站设备故障";
    case 0x05: return "确认";
    case 0x06: return "从站设备忙";
    default: return QString("未知异常 (0x%1)").arg(exceptionCode, 2, 16, QChar('0'));
    }
}

}
}