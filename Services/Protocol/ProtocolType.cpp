#include "ProtocolType.h"

namespace Services {
namespace Protocol {

QString protocolTypeToString(ProtocolType type)
{
    switch (type) {
    case ProtocolType::None: return "关闭解析";
    case ProtocolType::ModbusRtu: return "Modbus-RTU";
    case ProtocolType::ModbusTcp: return "Modbus-TCP";
    case ProtocolType::Can: return "CAN";
    case ProtocolType::CanOpen: return "CANopen";
    case ProtocolType::Custom: return "自定义协议";
    default: return "未知协议";
    }
}

ProtocolType stringToProtocolType(const QString& str)
{
    if (str == "关闭解析") return ProtocolType::None;
    if (str == "Modbus-RTU") return ProtocolType::ModbusRtu;
    if (str == "Modbus-TCP") return ProtocolType::ModbusTcp;
    if (str == "CAN") return ProtocolType::Can;
    if (str == "CANopen") return ProtocolType::CanOpen;
    if (str == "自定义协议") return ProtocolType::Custom;
    return ProtocolType::None;
}

}
}