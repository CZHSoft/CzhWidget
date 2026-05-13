#ifndef MODBUSTCPPARSER_H
#define MODBUSTCPPARSER_H

#include "ProtocolParser.h"

namespace Services {
namespace Protocol {

class ModbusTcpParser : public ProtocolParser
{
public:
    ParseResult parse(const QByteArray& data) override;
    QString getProtocolName() const override { return "Modbus TCP"; }
    ProtocolType getProtocolType() const override { return ProtocolType::ModbusTcp; }
    bool canParse(const QByteArray& data) const override;

private:
    QString functionCodeToString(uint8_t code);
    QString modbusErrorToString(uint8_t exceptionCode);
};

}
}

#endif // MODBUSTCPPARSER_H