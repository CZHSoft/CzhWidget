#include "ModbusRtuParser.h"
#include <QDateTime>
#include <QVariantList>

namespace Services {
namespace Protocol {

ParseResult ModbusRtuParser::parse(const QByteArray& data)
{
    ParseResult result;
    result.protocolType = getProtocolType();
    result.protocolName = getProtocolName();
    result.parseTime = QDateTime::currentDateTime();
    result.rawData = data;

    if (data.size() < 4) {
        result.success = false;
        result.errorMessage = "数据长度不足，至少需要4字节(地址+功能码+CRC)";
        result.formattedResult = QString("[解析失败] %1").arg(result.errorMessage);
        return result;
    }

    if (!validateCrc(data)) {
        result.success = false;
        result.errorMessage = "CRC校验失败";
        result.formattedResult = QString("[解析失败] %1").arg(result.errorMessage);
        return result;
    }

    uint8_t slaveAddress = static_cast<uint8_t>(data[0]);
    uint8_t functionCode = static_cast<uint8_t>(data[1]);

    if (functionCode >= 0x80) {
        uint8_t exceptionCode = static_cast<uint8_t>(data[2]);
        result.parsedData[ParseResult::KEY_SLAVE_ADDRESS] = slaveAddress;
        result.parsedData[ParseResult::KEY_FUNCTION_CODE] = functionCode;
        result.parsedData[ParseResult::KEY_FUNCTION_NAME] = QString("异常响应 (0x%1)").arg(functionCode - 0x80, 2, 16, QChar('0'));
        result.parsedData[ParseResult::KEY_ERROR_CODE] = exceptionCode;
        result.parsedData[ParseResult::KEY_ERROR_DESCRIPTION] = modbusErrorToString(exceptionCode);

        result.success = true;
        result.formattedResult = QString(
            "【Modbus RTU 异常响应】\n"
            "从站地址: %1 (0x%2)\n"
            "功能码: 0x%3 (异常)\n"
            "异常码: 0x%4\n"
            "异常描述: %5"
        ).arg(slaveAddress).arg(slaveAddress, 2, 16, QChar('0'))
         .arg(functionCode, 2, 16, QChar('0'))
         .arg(exceptionCode, 2, 16, QChar('0'))
         .arg(modbusErrorToString(exceptionCode));
        return result;
    }

    QByteArray pduData = data.mid(2, data.size() - 4);
    QString funcName = functionCodeToString(functionCode);

    result.parsedData[ParseResult::KEY_SLAVE_ADDRESS] = slaveAddress;
    result.parsedData[ParseResult::KEY_FUNCTION_CODE] = functionCode;
    result.parsedData[ParseResult::KEY_FUNCTION_NAME] = funcName;

    QString formatted = QString(
        "【Modbus RTU 帧解析】\n"
        "从站地址: %1 (0x%2)\n"
        "功能码: 0x%3 - %4\n"
    ).arg(slaveAddress).arg(slaveAddress, 2, 16, QChar('0'))
     .arg(functionCode, 2, 16, QChar('0'))
     .arg(funcName);

    switch (functionCode) {
    case 0x01:
    case 0x02: {
        int byteCount = static_cast<uint8_t>(pduData[0]);
        QVariantList values;
        QString bitsStr;
        for (int i = 0; i < byteCount; ++i) {
            uint8_t byteVal = static_cast<uint8_t>(pduData[i + 1]);
            for (int j = 7; j >= 0; --j) {
                bool bit = (byteVal >> j) & 0x01;
                values.append(bit);
                bitsStr += bit ? "1" : "0";
                if (j > 0) bitsStr += " ";
            }
            if (i < byteCount - 1) bitsStr += "\n";
        }
        result.parsedData[ParseResult::KEY_DATA_LENGTH] = byteCount * 8;
        result.parsedData[ParseResult::KEY_DATA_VALUES] = values;
        formatted += QString("\n字节计数: %1\n线圈/离散输入状态:\n%2").arg(byteCount).arg(bitsStr);
        break;
    }
    case 0x03:
    case 0x04: {
        int byteCount = static_cast<uint8_t>(pduData[0]);
        int registerCount = byteCount / 2;
        QVariantList values;
        QString regStr;
        for (int i = 0; i < registerCount; ++i) {
            uint16_t value = (static_cast<uint8_t>(pduData[1 + i * 2]) << 8) |
                              static_cast<uint8_t>(pduData[2 + i * 2]);
            values.append(value);
            regStr += QString("寄存器%1: %2 (0x%3)\n").arg(i + 1).arg(value).arg(value, 4, 16, QChar('0'));
        }
        result.parsedData[ParseResult::KEY_DATA_LENGTH] = registerCount;
        result.parsedData[ParseResult::KEY_DATA_VALUES] = values;
        formatted += QString("\n字节计数: %1\n保持寄存器/输入寄存器值:\n%2").arg(byteCount).arg(regStr);
        break;
    }
    case 0x05: {
        uint16_t outputAddress = (static_cast<uint8_t>(pduData[0]) << 8) |
                                  static_cast<uint8_t>(pduData[1]);
        uint16_t value = (static_cast<uint8_t>(pduData[2]) << 8) |
                          static_cast<uint8_t>(pduData[3]);
        QString state = (value == 0xFF00) ? "ON" : "OFF";
        result.parsedData[ParseResult::KEY_REGISTER_ADDRESS] = outputAddress;
        result.parsedData[ParseResult::KEY_DATA_VALUE] = state;
        formatted += QString("\n输出线圈地址: %1\n输出值: %2").arg(outputAddress).arg(state);
        break;
    }
    case 0x06: {
        uint16_t registerAddress = (static_cast<uint8_t>(pduData[0]) << 8) |
                                    static_cast<uint8_t>(pduData[1]);
        uint16_t value = (static_cast<uint8_t>(pduData[2]) << 8) |
                          static_cast<uint8_t>(pduData[3]);
        result.parsedData[ParseResult::KEY_REGISTER_ADDRESS] = registerAddress;
        result.parsedData[ParseResult::KEY_DATA_VALUE] = value;
        formatted += QString("\n寄存器地址: %1\n写入值: %2 (0x%3)").arg(registerAddress).arg(value).arg(value, 4, 16, QChar('0'));
        break;
    }
    case 0x07: {
        uint8_t status = static_cast<uint8_t>(pduData[0]);
        QString statusStr;
        if (status == 0x00) statusStr = "正常";
        else if (status == 0xFF) statusStr = "故障";
        else statusStr = QString("未知 (0x%1)").arg(status, 2, 16, QChar('0'));
        result.parsedData[ParseResult::KEY_DATA_VALUE] = statusStr;
        formatted += QString("\n诊断状态: %1").arg(statusStr);
        break;
    }
    case 0x0B: {
        uint16_t subfunction = (static_cast<uint8_t>(pduData[0]) << 8) |
                                static_cast<uint8_t>(pduData[1]);
        result.parsedData[ParseResult::KEY_DATA_VALUE] = subfunction;
        formatted += QString("\n获取事件计数器子功能: 0x%1").arg(subfunction, 4, 16, QChar('0'));
        break;
    }
    case 0x0F:
    case 0x10: {
        uint16_t startAddress = (static_cast<uint8_t>(pduData[0]) << 8) |
                                 static_cast<uint8_t>(pduData[1]);
        uint16_t quantity = (static_cast<uint8_t>(pduData[2]) << 8) |
                             static_cast<uint8_t>(pduData[3]);
        result.parsedData[ParseResult::KEY_REGISTER_ADDRESS] = startAddress;
        result.parsedData[ParseResult::KEY_DATA_LENGTH] = quantity;
        formatted += QString("\n起始地址: %1\n写入数量: %2").arg(startAddress).arg(quantity);
        break;
    }
    default:
        formatted += QString("\n数据字段: %1").arg(pduData.toHex());
    }

    result.success = true;
    result.formattedResult = formatted;
    return result;
}

bool ModbusRtuParser::canParse(const QByteArray& data) const
{
    return data.size() >= 4;
}

bool ModbusRtuParser::validateCrc(const QByteArray& data)
{
    uint16_t crc = 0xFFFF;
    int len = data.size() - 2;

    for (int i = 0; i < len; ++i) {
        crc ^= static_cast<uint8_t>(data[i]);
        for (int j = 0; j < 8; ++j) {
            crc = (crc >> 1) ^ ((crc & 0x01) ? 0xA001 : 0);
        }
    }

    uint16_t receivedCrc = static_cast<uint8_t>(data[len]) |
                            (static_cast<uint8_t>(data[len + 1]) << 8);
    return crc == receivedCrc;
}

QString ModbusRtuParser::functionCodeToString(uint8_t code)
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

QString ModbusRtuParser::formatDataValue(uint8_t, const QByteArray& data)
{
    return data.toHex();
}

QString ModbusRtuParser::modbusErrorToString(uint8_t exceptionCode)
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