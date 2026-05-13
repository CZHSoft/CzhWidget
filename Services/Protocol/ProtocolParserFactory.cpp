#include "ProtocolParserFactory.h"
#include "ProtocolParser.h"
#include "ModbusRtuParser.h"
#include "ModbusTcpParser.h"
#include "CanParser.h"
#include "CanOpenParser.h"

namespace Services {
namespace Protocol {

std::unique_ptr<ProtocolParser> ProtocolParserFactory::createParser(ProtocolType type)
{
    switch (type) {
    case ProtocolType::ModbusRtu:
        return std::make_unique<ModbusRtuParser>();
    case ProtocolType::ModbusTcp:
        return std::make_unique<ModbusTcpParser>();
    case ProtocolType::Can:
        return std::make_unique<CanParser>();
    case ProtocolType::CanOpen:
        return std::make_unique<CanOpenParser>();
    default:
        return nullptr;
    }
}

std::unique_ptr<ProtocolParser> ProtocolParserFactory::createCustomParser(const QVariantMap&)
{
    return nullptr;
}

QList<ProtocolType> ProtocolParserFactory::getSupportedProtocols()
{
    return {
        ProtocolType::None,
        ProtocolType::ModbusRtu,
        ProtocolType::ModbusTcp,
        ProtocolType::Can,
        ProtocolType::CanOpen,
        ProtocolType::Custom
    };
}

QString ProtocolParserFactory::getProtocolDisplayName(ProtocolType type)
{
    return protocolTypeToString(type);
}

}
}