#ifndef PROTOCOLTYPE_H
#define PROTOCOLTYPE_H

#include <QString>

namespace Services {
namespace Protocol {

enum class ProtocolType {
    None,
    ModbusRtu,
    ModbusTcp,
    Can,
    CanOpen,
    Custom
};

QString protocolTypeToString(ProtocolType type);
ProtocolType stringToProtocolType(const QString& str);

}
}

#endif // PROTOCOLTYPE_H