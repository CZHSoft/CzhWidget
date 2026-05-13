#include "SerialComm.h"

namespace Services {
namespace Comm {

SerialComm::SerialComm(QObject* parent)
    : QObject(parent), _serialPort(new QSerialPort(this)), _lastError("")
{
    QObject::connect(_serialPort, &QSerialPort::readyRead, this, &SerialComm::onReadyRead);
    QObject::connect(_serialPort, &QSerialPort::errorOccurred, this, &SerialComm::onErrorOccurred);
}

SerialComm::~SerialComm()
{
    disconnect();
}

bool SerialComm::connect()
{
    if (_serialPort->isOpen()) {
        _serialPort->close();
    }

    _serialPort->setPortName(_config.serialPort);
    _serialPort->setBaudRate(_config.baudRate);

    QSerialPort::DataBits dataBits;
    switch (_config.dataBits) {
    case 5: dataBits = QSerialPort::Data5; break;
    case 6: dataBits = QSerialPort::Data6; break;
    case 7: dataBits = QSerialPort::Data7; break;
    default: dataBits = QSerialPort::Data8;
    }
    _serialPort->setDataBits(dataBits);

    QSerialPort::Parity parity;
    if (_config.parity == "Even") parity = QSerialPort::EvenParity;
    else if (_config.parity == "Odd") parity = QSerialPort::OddParity;
    else if (_config.parity == "Space") parity = QSerialPort::SpaceParity;
    else if (_config.parity == "Mark") parity = QSerialPort::MarkParity;
    else parity = QSerialPort::NoParity;
    _serialPort->setParity(parity);

    QSerialPort::StopBits stopBits = QSerialPort::OneStop;
    // Skip stopBits compatibility check for now
    _serialPort->setStopBits(stopBits);

    _serialPort->setFlowControl(QSerialPort::NoFlowControl);

    bool success = _serialPort->open(QIODevice::ReadWrite);
    if (success) {
        emit connected();
    } else {
        _lastError = _serialPort->errorString();
        emit errorOccurred(_lastError);
    }

    return success;
}

bool SerialComm::disconnect()
{
    if (_serialPort->isOpen()) {
        _serialPort->close();
        emit disconnected();
        return true;
    }
    return false;
}

bool SerialComm::isConnected() const
{
    return _serialPort->isOpen();
}

qint64 SerialComm::sendData(const QByteArray& data)
{
    if (!_serialPort->isOpen()) {
        _lastError = "Serial port not open";
        return -1;
    }

    QByteArray sendData = data;
    if (_config.appendNewLine) {
        sendData += _config.newLineType.toUtf8();
    }

    return _serialPort->write(sendData);
}

QByteArray SerialComm::receiveData()
{
    if (!_serialPort->isOpen()) {
        return QByteArray();
    }
    return _serialPort->readAll();
}

QString SerialComm::getErrorString() const
{
    return _lastError;
}

QString SerialComm::getConnectionInfo() const
{
    if (!_serialPort->isOpen()) {
        return "Not connected";
    }
    return QString("%1 @ %2 bps").arg(_config.serialPort).arg(_config.baudRate);
}

void SerialComm::setConfig(const CommConfig& config)
{
    _config = config;
}

CommConfig SerialComm::getConfig() const
{
    return _config;
}

void SerialComm::onReadyRead()
{
    QByteArray data = _serialPort->readAll();
    if (!data.isEmpty()) {
        emit dataReceived(data);
    }
}

void SerialComm::onErrorOccurred(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::SerialPortError::NoError) {
        return;
    }
    _lastError = _serialPort->errorString();
    emit errorOccurred(_lastError);
}

} // namespace Comm
} // namespace Services
