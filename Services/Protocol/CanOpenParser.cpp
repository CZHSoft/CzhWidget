#include "CanOpenParser.h"
#include <QDateTime>

namespace Services {
namespace Protocol {

ParseResult CanOpenParser::parse(const QByteArray& data)
{
    ParseResult result;
    result.protocolType = getProtocolType();
    result.protocolName = getProtocolName();
    result.parseTime = QDateTime::currentDateTime();
    result.rawData = data;

    if (data.size() < 4) {
        result.success = false;
        result.errorMessage = "数据长度不足，CANopen帧最小需要4字节(COB-ID+数据)";
        result.formattedResult = QString("[解析失败] %1").arg(result.errorMessage);
        return result;
    }

    uint32_t cobId = static_cast<uint8_t>(data[0]);
    if (data.size() >= 5) {
        cobId |= (static_cast<uint8_t>(data[1]) << 8);
        if (data.size() >= 6) {
            cobId |= (static_cast<uint8_t>(data[2]) << 16);
        }
    }

    uint8_t functionCode = (cobId >> 7) & 0x1F;
    uint8_t nodeId = cobId & 0x7F;

    result.parsedData[ParseResult::KEY_COB_ID] = cobId;
    result.parsedData[ParseResult::KEY_NODE_ID] = nodeId;

    QString formatted = QString(
        "【CANopen 帧解析】\n"
        "COB-ID: 0x%1\n"
        "功能码: 0x%2 - %3\n"
        "节点ID: %4"
    ).arg(cobId, 6, 16, QChar('0'))
     .arg(functionCode, 2, 16, QChar('0'))
     .arg(cobIdToString(cobId))
     .arg(nodeId);

    QByteArray canData = data.mid(3);

    switch (functionCode) {
    case 0x00: {
        if (canData.size() >= 1) {
            uint8_t nmtCommand = static_cast<uint8_t>(canData[0]);
            QString commandStr = nmtCommandToString(nmtCommand);
            result.parsedData[ParseResult::KEY_NMT_STATE] = commandStr;
            formatted += QString("\nNMT命令: 0x%1 - %2").arg(nmtCommand, 2, 16, QChar('0')).arg(commandStr);
        }
        break;
    }
    case 0x01: {
        if (canData.size() >= 1) {
            uint8_t bootUpCode = static_cast<uint8_t>(canData[0]);
            formatted += QString("\nBoot-up: 0x%1").arg(bootUpCode, 2, 16, QChar('0'));
        }
        break;
    }
    case 0x02: {
        if (canData.size() >= 8) {
            uint16_t sdoIndex = (static_cast<uint8_t>(canData[1]) << 8) | static_cast<uint8_t>(canData[2]);
            uint8_t sdoSubindex = static_cast<uint8_t>(canData[3]);
            uint8_t sdoCommand = static_cast<uint8_t>(canData[0]);

            result.parsedData[ParseResult::KEY_SDO_INDEX] = sdoIndex;
            result.parsedData[ParseResult::KEY_SDO_SUBINDEX] = sdoSubindex;

            formatted += QString(
                "\nSDO上传请求:\n"
                "  命令: 0x%1 - %2\n"
                "  索引: 0x%3\n"
                "  子索引: 0x%4"
            ).arg(sdoCommand, 2, 16, QChar('0'))
             .arg(sdoCommandToString(sdoCommand))
             .arg(sdoIndex, 4, 16, QChar('0'))
             .arg(sdoSubindex, 2, 16, QChar('0'));
        }
        break;
    }
    case 0x03: {
        if (canData.size() >= 8) {
            uint8_t sdoCommand = static_cast<uint8_t>(canData[0]);
            uint16_t sdoIndex = (static_cast<uint8_t>(canData[1]) << 8) | static_cast<uint8_t>(canData[2]);
            uint8_t sdoSubindex = static_cast<uint8_t>(canData[3]);
            QByteArray sdoData = canData.mid(4);

            result.parsedData[ParseResult::KEY_SDO_INDEX] = sdoIndex;
            result.parsedData[ParseResult::KEY_SDO_SUBINDEX] = sdoSubindex;

            formatted += QString(
                "\nSDO上传响应:\n"
                "  命令: 0x%1 - %2\n"
                "  索引: 0x%3\n"
                "  子索引: 0x%4\n"
                "  数据: %5"
            ).arg(sdoCommand, 2, 16, QChar('0'))
             .arg(sdoCommandToString(sdoCommand))
             .arg(sdoIndex, 4, 16, QChar('0'))
             .arg(sdoSubindex, 2, 16, QChar('0'))
             .arg(sdoData.toHex(' '));
        }
        break;
    }
    case 0x04: {
        if (canData.size() >= 8) {
            uint8_t sdoCommand = static_cast<uint8_t>(canData[0]);
            uint16_t sdoIndex = (static_cast<uint8_t>(canData[1]) << 8) | static_cast<uint8_t>(canData[2]);
            uint8_t sdoSubindex = static_cast<uint8_t>(canData[3]);

            result.parsedData[ParseResult::KEY_SDO_INDEX] = sdoIndex;
            result.parsedData[ParseResult::KEY_SDO_SUBINDEX] = sdoSubindex;

            formatted += QString(
                "\nSDO下载请求:\n"
                "  命令: 0x%1 - %2\n"
                "  索引: 0x%3\n"
                "  子索引: 0x%4"
            ).arg(sdoCommand, 2, 16, QChar('0'))
             .arg(sdoCommandToString(sdoCommand))
             .arg(sdoIndex, 4, 16, QChar('0'))
             .arg(sdoSubindex, 2, 16, QChar('0'));

            if (canData.size() > 4) {
                QByteArray sdoData = canData.mid(4);
                formatted += QString("\n  数据: %1").arg(sdoData.toHex(' '));
            }
        }
        break;
    }
    case 0x05: {
        if (canData.size() >= 1) {
            uint8_t sdoCommand = static_cast<uint8_t>(canData[0]);
            formatted += QString("\nSDO下载响应: 0x%1 - %2").arg(sdoCommand, 2, 16, QChar('0')).arg(sdoCommandToString(sdoCommand));
        }
        break;
    }
    case 0x08: {
        if (canData.size() >= 1) {
            uint8_t emergencyCode = static_cast<uint8_t>(canData[0]);
            uint8_t registerNumber = canData.size() > 1 ? static_cast<uint8_t>(canData[1]) : 0;
            uint16_t errorCode = canData.size() > 3 ? 
                ((static_cast<uint8_t>(canData[2]) << 8) | static_cast<uint8_t>(canData[3])) : 0;

            formatted += QString(
                "\nEmergency:\n"
                "  紧急代码: 0x%1\n"
                "  寄存器号: 0x%2\n"
                "  错误代码: 0x%3"
            ).arg(emergencyCode, 2, 16, QChar('0'))
             .arg(registerNumber, 2, 16, QChar('0'))
             .arg(errorCode, 4, 16, QChar('0'));
        }
        break;
    }
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13: {
        QString pdoType = (functionCode == 0x10) ? "TPDO1" : 
                         (functionCode == 0x11) ? "TPDO2" :
                         (functionCode == 0x12) ? "TPDO3" : "TPDO4";
        formatted += QString("\n%1 (发送PDO):\n  数据: %2").arg(pdoType).arg(canData.toHex(' '));
        break;
    }
    case 0x20:
    case 0x21:
    case 0x22:
    case 0x23: {
        QString pdoType = (functionCode == 0x20) ? "RPDO1" : 
                         (functionCode == 0x21) ? "RPDO2" :
                         (functionCode == 0x22) ? "RPDO3" : "RPDO4";
        formatted += QString("\n%1 (接收PDO):\n  数据: %2").arg(pdoType).arg(canData.toHex(' '));
        break;
    }
    case 0x30: {
        if (canData.size() >= 1) {
            uint8_t heartbeatState = static_cast<uint8_t>(canData[0]);
            QString stateStr;
            switch (heartbeatState) {
            case 0x00: stateStr = "初始化"; break;
            case 0x01: stateStr = "预操作"; break;
            case 0x02: stateStr = "操作"; break;
            case 0x03: stateStr = "停止"; break;
            default: stateStr = QString("未知 (0x%1)").arg(heartbeatState, 2, 16, QChar('0'));
            }
            result.parsedData[ParseResult::KEY_NMT_STATE] = stateStr;
            formatted += QString("\nHeartbeat: 状态=0x%1 - %2").arg(heartbeatState, 2, 16, QChar('0')).arg(stateStr);
        }
        break;
    }
    case 0x31: {
        formatted += QString("\n同步(SYNC): 数据=%1").arg(canData.toHex(' '));
        break;
    }
    case 0x32: {
        formatted += QString("\n时间戳(EMCY): 数据=%1").arg(canData.toHex(' '));
        break;
    }
    case 0x3E: {
        if (canData.size() >= 2) {
            uint16_t lssCommand = (static_cast<uint8_t>(canData[0]) << 8) | static_cast<uint8_t>(canData[1]);
            formatted += QString("\nLSS命令: 0x%1").arg(lssCommand, 4, 16, QChar('0'));
        }
        break;
    }
    default: {
        if (!canData.isEmpty()) {
            formatted += QString("\n数据: %1").arg(canData.toHex(' '));
        }
    }
    }

    result.success = true;
    result.formattedResult = formatted;
    return result;
}

QString CanOpenParser::cobIdToString(uint32_t cobId)
{
    uint8_t functionCode = (cobId >> 7) & 0x1F;

    switch (functionCode) {
    case 0x00: return "NMT 命令";
    case 0x01: return "Boot-up";
    case 0x02: return "SDO 上传请求";
    case 0x03: return "SDO 上传响应";
    case 0x04: return "SDO 下载请求";
    case 0x05: return "SDO 下载响应";
    case 0x08: return "Emergency";
    case 0x10: return "TPDO1";
    case 0x11: return "TPDO2";
    case 0x12: return "TPDO3";
    case 0x13: return "TPDO4";
    case 0x20: return "RPDO1";
    case 0x21: return "RPDO2";
    case 0x22: return "RPDO3";
    case 0x23: return "RPDO4";
    case 0x30: return "Heartbeat";
    case 0x31: return "SYNC";
    case 0x32: return "时间戳";
    case 0x3E: return "LSS";
    default: return QString("未知 (0x%1)").arg(functionCode, 2, 16, QChar('0'));
    }
}

QString CanOpenParser::sdoCommandToString(uint8_t command)
{
    switch (command) {
    case 0x22: return "初始化上传";
    case 0x42: return "上传请求";
    case 0x60: return "上传/下载响应";
    case 0x23: return "初始化下载";
    case 0x2B: return "下载请求(分段)";
    case 0x80: return "SDO中止";
    default: return QString("未知命令");
    }
}

QString CanOpenParser::nmtCommandToString(uint8_t command)
{
    switch (command) {
    case 0x00: return "停止远程节点";
    case 0x01: return "启动远程节点";
    case 0x02: return "进入预操作状态";
    case 0x80: return "复位节点";
    case 0x81: return "复位通信";
    default: return QString("未知命令 (0x%1)").arg(command, 2, 16, QChar('0'));
    }
}

}
}