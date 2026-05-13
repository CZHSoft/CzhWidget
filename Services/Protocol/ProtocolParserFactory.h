#ifndef PROTOCOLPARSERFACTORY_H
#define PROTOCOLPARSERFACTORY_H

#include <memory>
#include <QList>
#include <QVariantMap>
#include "ProtocolType.h"

namespace Services {
namespace Protocol {

class ProtocolParser;

class ProtocolParserFactory
{
public:
    static std::unique_ptr<ProtocolParser> createParser(ProtocolType type);
    static std::unique_ptr<ProtocolParser> createCustomParser(const QVariantMap& config);
    static QList<ProtocolType> getSupportedProtocols();
    static QString getProtocolDisplayName(ProtocolType type);
};

}
}

#endif // PROTOCOLPARSERFACTORY_H