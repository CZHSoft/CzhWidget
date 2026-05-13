#ifndef PROTOCOLPARSER_H
#define PROTOCOLPARSER_H

#include <memory>
#include <QByteArray>
#include "ParseResult.h"

namespace Services {
namespace Protocol {

class ProtocolParser
{
public:
    virtual ~ProtocolParser() = default;

    virtual ParseResult parse(const QByteArray& data) = 0;
    virtual QString getProtocolName() const = 0;
    virtual ProtocolType getProtocolType() const = 0;
    virtual bool canParse(const QByteArray& data) const { return true; }
};

}
}

#endif // PROTOCOLPARSER_H