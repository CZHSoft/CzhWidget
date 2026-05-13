#include "CanParser.h"
#include <QDateTime>

namespace Services {
namespace Protocol {

ParseResult CanParser::parse(const QByteArray& data)
{
    ParseResult result;
    result.protocolType = getProtocolType();
    result.protocolName = getProtocolName();
    result.parseTime = QDateTime::currentDateTime();
    result.rawData = data;

    if (data.size() < 5) {
        result.success = false;
        result.errorMessage = "数据长度不足，CAN帧最小需要5字节(ID+DLC+至少1字节数据)";
        result.formattedResult = QString("[解析失败] %1").arg(result.errorMessage);
        return result;
    }

    uint32_t canId = 0;
    bool isExtended = false;

    if (data.size() >= 5) {
        canId = static_cast<uint8_t>(data[0]);
        if (data.size() >= 6) {
            canId |= (static_cast<uint8_t>(data[1]) << 8);
            if (data.size() >= 7) {
                canId |= (static_cast<uint8_t>(data[2]) << 16);
                if (data.size() >= 8) {
                    canId |= (static_cast<uint8_t>(data[3]) << 24);
                    isExtended = true;
                }
            }
        }
    }

    int idOffset = isExtended ? 4 : 3;
    if (idOffset + 1 > data.size()) {
        result.success = false;
        result.errorMessage = "数据格式错误，缺少DLC字段";
        result.formattedResult = QString("[解析失败] %1").arg(result.errorMessage);
        return result;
    }

    uint8_t dlc = static_cast<uint8_t>(data[idOffset]);
    if (dlc > 8) {
        dlc = 8;
    }

    int dataStart = idOffset + 1;
    QByteArray canData = data.mid(dataStart, dlc);

    result.parsedData[ParseResult::KEY_CAN_ID] = canId;
    result.parsedData[ParseResult::KEY_CAN_DLC] = dlc;
    result.parsedData[ParseResult::KEY_CAN_DATA] = canData;

    QString formatted = QString(
        "【CAN 帧解析】\n"
        "ID: %1 (0x%2)\n"
        "ID类型: %3\n"
        "DLC: %4 字节\n"
        "数据: %5"
    ).arg(canId).arg(canId, isExtended ? 8 : 3, 16, QChar('0'))
     .arg(isExtended ? "扩展帧(29位)" : "标准帧(11位)")
     .arg(dlc)
     .arg(canData.toHex(' '));

    if (!canData.isEmpty()) {
        formatted += QString("\n十进制值:");
        for (int i = 0; i < canData.size(); ++i) {
            formatted += QString(" %1").arg(static_cast<int8_t>(canData[i]));
        }
    }

    uint32_t arbitrationId = canId & (isExtended ? 0x1FFFFFFF : 0x7FF);
    bool isRemoteFrame = (canId & 0x40000000) != 0;
    bool isErrorFrame = (canId & 0x80000000) != 0;

    if (isRemoteFrame) {
        formatted += "\n帧类型: 远程请求帧(RTR)";
    } else if (isErrorFrame) {
        formatted += "\n帧类型: 错误帧";
    } else {
        formatted += "\n帧类型: 数据帧";
    }

    if ((arbitrationId & 0x80000000) != 0) {
        formatted += "\nFD帧标志: 是";
    }

    result.success = true;
    result.formattedResult = formatted;
    return result;
}

QString CanParser::formatCanId(uint32_t canId, bool isExtended)
{
    if (isExtended) {
        return QString("0x%1").arg(canId, 8, 16, QChar('0'));
    }
    return QString("0x%1").arg(canId, 3, 16, QChar('0'));
}

}
}