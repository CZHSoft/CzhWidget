#ifndef CANPARSER_H
#define CANPARSER_H

#include "ProtocolParser.h"

namespace Services {
namespace Protocol {

class CanParser : public ProtocolParser
{
public:
    ParseResult parse(const QByteArray& data) override;
    QString getProtocolName() const override { return "CAN"; }
    ProtocolType getProtocolType() const override { return ProtocolType::Can; }

private:
    QString formatCanId(uint32_t canId, bool isExtended);
};

}
}

#endif // CANPARSER_H