#ifndef CANOPENPARSER_H
#define CANOPENPARSER_H

#include "ProtocolParser.h"

namespace Services {
namespace Protocol {

class CanOpenParser : public ProtocolParser
{
public:
    ParseResult parse(const QByteArray& data) override;
    QString getProtocolName() const override { return "CANopen"; }
    ProtocolType getProtocolType() const override { return ProtocolType::CanOpen; }

private:
    QString cobIdToString(uint32_t cobId);
    QString sdoCommandToString(uint8_t command);
    QString nmtCommandToString(uint8_t command);
};

}
}

#endif // CANOPENPARSER_H