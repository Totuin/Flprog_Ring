#include "flprogRing.h"

FLProgRing::FLProgRing(uint8_t port, uint16_t readVariablesCount, uint16_t sendVariablesCount, uint32_t index, FlprogAbstractUartExecutor *executor)
{
    _port = port;
    _index = index;
    _executor = executor;
    _readIitems = new FLProgAbstractRingVariable *[readVariablesCount];
    for (uint16_t i = 0; i < readVariablesCount; i++)
    {
        _readIitems[i] = 0;
    }
    _sendIitems = new FLProgAbstractRingVariable *[sendVariablesCount];
    for (uint16_t i = 0; i < sendVariablesCount; i++)
    {
        _sendIitems[i] = 0;
    }
    _readIitemsCount = readVariablesCount;
    _sendIitemsCount = sendVariablesCount;
}

void FLProgRing::addReadVariable(FLProgAbstractRingVariable *variable)
{
    int16_t index = -1;
    for (uint16_t i = 0; i < _readIitemsCount; i++)
    {
        if (_readIitems[i] == 0)
        {
            index = i;
            break;
        }
    }
    if (index < 0)
    {
        return;
    }
    _readIitems[index] = variable;
}

void FLProgRing::addSendVariable(FLProgAbstractRingVariable *variable)
{
    int16_t index = -1;
    for (uint16_t i = 0; i < _sendIitemsCount; i++)
    {
        if (_sendIitems[i] == 0)
        {
            index = i;
            break;
        }
    }
    if (index < 0)
    {
        return;
    }
    _sendIitems[index] = variable;
}

void FLProgRing::readNextVariable()
{
    while (_executor->availableUart(_port) > 0)
    {
        if (_workMode == FLPROG_RING_END_READ_VARIABLE_MODE)
        {
            break;
        }
        analizeNextReadingyte(_executor->readUart(_port));
    }
    if (_workMode == FLPROG_RING_END_READ_VARIABLE_MODE)
    {
        checkBuffer();
        if (!_isSingleMode)
        {
            if (_tempSender != _index)
            {
                sendBuffer();
            }
        }
    }
    sendNextVariable();
    _workMode = FLPROG_RING_WHITE_MODE;
}

void FLProgRing::sendBuffer()
{
    _executor->writeUart(1, _port);
    _executor->printUart(String(_tempSender), _port);
    _executor->writeUart(2, _port);
    _executor->printUart(String(_tempName), _port);
    _executor->writeUart(3, _port);
    _executor->printUart(_tempString, _port);
    _executor->writeUart(4, _port);
}

void FLProgRing::checkBuffer()
{
    for (uint16_t i = 0; i < _readIitemsCount; i++)
    {
        if (_readIitems[i] == 0)
        {
            return;
        }
        if ((_readIitems[i]->index()) == _tempName)
        {
            _readIitems[i]->setValueString(_tempString);
        }
    }
}

void FLProgRing::analizeNextReadingyte(uint8_t byte)
{
    if (_workMode == FLPROG_RING_START_READ_VARIABLE_MODE)
    {
        if (byte == 1)
        {
            _tempString = "";
            _workMode = FLPROG_RING_READ_SENDER_VARIABLE_MODE;
        }
        return;
    }
    if (_workMode == FLPROG_RING_READ_SENDER_VARIABLE_MODE)
    {
        if (byte == 2)
        {
            _tempSender = _tempString.toInt();
            _tempString = "";
            _workMode = FLPROG_RING_READ_NAME_VARIABLE_MODE;
            return;
        }
        _tempString += (char)byte;
        return;
    }
    if (_workMode == FLPROG_RING_READ_NAME_VARIABLE_MODE)
    {
        if (byte == 3)
        {
            _tempName = _tempString.toInt();
            _tempString = "";
            _workMode = FLPROG_RING_READ_VALUE_VARIABLE_MODE;
            return;
        }
        _tempString += (char)byte;
        return;
    }
    if (_workMode == FLPROG_RING_READ_VALUE_VARIABLE_MODE)
    {
        if (byte == 4)
        {
            _workMode = FLPROG_RING_END_READ_VARIABLE_MODE;
            return;
        }
        _tempString += (char)byte;
    }
}

void FLProgRing::sendNextVariable()
{
    int16_t index = nextSendVariableIndex();
    if (index < 0)
    {
        return;
    }
    _tempString = _sendIitems[index]->valueString();
    _tempSender = _index;
    _tempName = _sendIitems[index]->index();
    sendBuffer();
    _sendIitems[index]->isNeedSend(false);
}

int16_t FLProgRing::nextSendVariableIndex()
{
    for (uint16_t i = 0; i < _sendIitemsCount; i++)
    {
        if (_sendIitems[i] == 0)
        {
            return -1;
        }
        if (_sendIitems[i]->isNeedSend())
        {
            return (int16_t)i;
        }
    }
    return -1;
}

void FLProgRing::pool()
{
    if (_eventsCount < _skippingEvents)
    {
        _eventsCount++;
        return;
    }
    _eventsCount = 0;
    if (_executor->availableUart(_port))
    {
        _workMode = FLPROG_RING_START_READ_VARIABLE_MODE;
        _tempString = "";
        readNextVariable();
        return;
    }
    else
    {
        sendNextVariable();
        return;
    }
}
