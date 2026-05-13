#ifndef COMMFACTORY_H
#define COMMFACTORY_H

#include <memory>
#include "CommProtocol.h"
#include "ICommInterface.h"

namespace Services {
namespace Comm {

class CommFactory
{
public:
    static std::unique_ptr<ICommInterface> createComm(CommProtocol protocol);
    static void destroyComm(ICommInterface* comm);
};

} // namespace Comm
} // namespace Services

#endif // COMMFACTORY_H
