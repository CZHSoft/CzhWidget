#ifndef PARSERESULT_H
#define PARSERESULT_H

#include <QByteArray>
#include <QDateTime>
#include <QString>
#include <QVariantMap>
#include "ProtocolType.h"

namespace Services {
namespace Protocol {

struct ParseResult {
    bool success = false;
    QString errorMessage;
    ProtocolType protocolType;
    QString protocolName;
    QDateTime parseTime;
    QByteArray rawData;
    QVariantMap parsedData;
    QString formattedResult;

    static constexpr const char* KEY_SLAVE_ADDRESS = "slaveAddress";
    static constexpr const char* KEY_FUNCTION_CODE = "functionCode";
    static constexpr const char* KEY_FUNCTION_NAME = "functionName";
    static constexpr const char* KEY_REGISTER_ADDRESS = "registerAddress";
    static constexpr const char* KEY_DATA_LENGTH = "dataLength";
    static constexpr const char* KEY_DATA_VALUE = "dataValue";
    static constexpr const char* KEY_DATA_VALUES = "dataValues";
    static constexpr const char* KEY_CAN_ID = "canId";
    static constexpr const char* KEY_CAN_DLC = "canDlc";
    static constexpr const char* KEY_CAN_DATA = "canData";
    static constexpr const char* KEY_COB_ID = "cobId";
    static constexpr const char* KEY_SDO_INDEX = "sdoIndex";
    static constexpr const char* KEY_SDO_SUBINDEX = "sdoSubindex";
    static constexpr const char* KEY_TRANSACTION_ID = "transactionId";
    static constexpr const char* KEY_PROTOCOL_ID = "protocolId";
    static constexpr const char* KEY_PDU_LENGTH = "pduLength";
    static constexpr const char* KEY_NODE_ID = "nodeId";
    static constexpr const char* KEY_NMT_STATE = "nmtState";
    static constexpr const char* KEY_ERROR_CODE = "errorCode";
    static constexpr const char* KEY_ERROR_DESCRIPTION = "errorDescription";
};

}
}

#endif // PARSERESULT_H