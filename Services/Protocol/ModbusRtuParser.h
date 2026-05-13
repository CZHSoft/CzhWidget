#ifndef MODBUSRTUPARSER_H
#define MODBUSRTUPARSER_H

#include "ProtocolParser.h"

namespace Services {
namespace Protocol {

class ModbusRtuParser : public ProtocolParser
{
public:
    ParseResult parse(const QByteArray& data) override;
    QString getProtocolName() const override { return "Modbus RTU"; }
    ProtocolType getProtocolType() const override { return ProtocolType::ModbusRtu; }
    bool canParse(const QByteArray& data) const override;

private:
    bool validateCrc(const QByteArray& data);
    QString functionCodeToString(uint8_t code);
    QString formatDataValue(uint8_t functionCode, const QByteArray& data);
    QString modbusErrorToString(uint8_t exceptionCode);
};

}
}

#endif // MODBUSRTUPARSER_H