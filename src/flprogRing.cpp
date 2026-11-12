#include "flprogRing.h"

namespace
{
// String::toInt() возвращает знаковый long и не годится для полного uint32_t.
// ID в протоколе всегда кодируются десятичными цифрами без знака.
bool parseUnsignedDecimal(const String &text, uint32_t &result)
{
    if (text.length() == 0)
    {
        return false;
    }
    uint32_t number = 0;
    for (size_t i = 0; i < text.length(); ++i)
    {
        char ch = text.charAt(i);
        if (ch < '0' || ch > '9')
        {
            return false;
        }
        const uint8_t digit = static_cast<uint8_t>(ch - '0');
        if (number > (UINT32_MAX - digit) / 10U)
        {
            return false;
        }
        number = number * 10U + digit;
    }
    result = number;
    return true;
}
}

FLProgRing::FLProgRing(uint8_t port, uint16_t readVariablesCount,
                       uint16_t sendVariablesCount, uint32_t index,
                       FlprogAbstractUartExecutor *executor)
    : _port(port), _executor(executor), _index(index)
{
    // Владеем только массивами, не экземплярами переменных и не executor.
    _readIitems = new FLProgAbstractRingVariable *[readVariablesCount]();
    _sendIitems = new FLProgAbstractRingVariable *[sendVariablesCount]();
    _readIitemsCount = readVariablesCount;
    _sendIitemsCount = sendVariablesCount;
}

FLProgRing::~FLProgRing()
{
    delete[] _readIitems;
    delete[] _sendIitems;
}

void FLProgRing::addReadVariable(FLProgAbstractRingVariable *variable)
{
    if (!variable)
    {
        return;
    }
    for (uint16_t i = 0; i < _readIitemsCount; i++)
    {
        if (_readIitems[i] == 0)
        {
            _readIitems[i] = variable;
            return;
        }
    }
}

void FLProgRing::addSendVariable(FLProgAbstractRingVariable *variable)
{
    if (!variable)
    {
        return;
    }
    for (uint16_t i = 0; i < _sendIitemsCount; i++)
    {
        if (_sendIitems[i] == 0)
        {
            _sendIitems[i] = variable;
            return;
        }
    }
}

void FLProgRing::readNextVariable()
{
    // В одном вызове принимаем не больше одного завершённого кадра.
    // Незавершённый кадр сохраняем между вызовами pool().
    while (_executor->availableUart(_port) > 0)
    {
        const uint8_t nextByte = _executor->readUart(_port);
        _lastRxByteMillis = millis();
        analizeNextReadingyte(nextByte);
        if (_workMode != FLPROG_RING_END_READ_VARIABLE_MODE)
        {
            continue;
        }

        if (_frameValid)
        {
            checkBuffer();
            // В режиме кольца пересылаем кадр дальше, кроме собственных.
            if (!_isSingleMode && _tempSender != _index)
            {
                sendBuffer();
            }
        }
        _workMode = FLPROG_RING_WHITE_MODE;
        _tempString = "";
        // Историческое поведение: по окончании кадра отправляем одну
        // изменённую локальную переменную, если она ожидает передачи.
        sendNextVariable();
        return;
    }
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
        // В отличие от первоначального варианта, незаполненные ячейки
        // списка не прерывают поиск зарегистрированных переменных.
        if (!_readIitems[i])
        {
            continue;
        }
        if (_readIitems[i]->index() == _tempName)
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
            _frameValid = true;
            _workMode = FLPROG_RING_READ_SENDER_VARIABLE_MODE;
        }
        return;
    }
    if (_workMode == FLPROG_RING_READ_SENDER_VARIABLE_MODE)
    {
        if (byte == 2)
        {
            if (!parseUnsignedDecimal(_tempString, _tempSender))
            {
                _frameValid = false;
            }
            _tempString = "";
            _workMode = FLPROG_RING_READ_NAME_VARIABLE_MODE;
            return;
        }
        // Числовые поля ограничены 10 ASCII-символами (uint32_t).
        if (_tempString.length() < 10)
        {
            _tempString += static_cast<char>(byte);
        }
        else
        {
            _frameValid = false;
        }
        return;
    }
    if (_workMode == FLPROG_RING_READ_NAME_VARIABLE_MODE)
    {
        if (byte == 3)
        {
            if (!parseUnsignedDecimal(_tempString, _tempName))
            {
                _frameValid = false;
            }
            _tempString = "";
            _workMode = FLPROG_RING_READ_VALUE_VARIABLE_MODE;
            return;
        }
        if (_tempString.length() < 10)
        {
            _tempString += static_cast<char>(byte);
        }
        else
        {
            _frameValid = false;
        }
        return;
    }
    if (_workMode == FLPROG_RING_READ_VALUE_VARIABLE_MODE)
    {
        if (byte == 4)
        {
            _workMode = FLPROG_RING_END_READ_VARIABLE_MODE;
            return;
        }
        _tempString += static_cast<char>(byte);
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
        if (_sendIitems[i] && _sendIitems[i]->isNeedSend())
        {
            return static_cast<int16_t>(i);
        }
    }
    return -1;
}

void FLProgRing::pool()
{
    if (!_executor)
    {
        return;
    }
    if (_eventsCount < _skippingEvents)
    {
        ++_eventsCount;
        return;
    }
    _eventsCount = 0;

    // Проверяем устаревший пакет ДО чтения новых байтов: даже если буфер UART
    // уже непуст, новый кадр не должен продолжать старый после долгой паузы.
    if (_workMode != FLPROG_RING_WHITE_MODE &&
        static_cast<uint32_t>(millis() - _lastRxByteMillis) >= _timeout)
    {
        // Безопасно при переполнении 32-битного millis().
        _workMode = FLPROG_RING_WHITE_MODE;
        _tempString = "";
    }
    if (_executor->availableUart(_port) > 0)
    {
        if (_workMode == FLPROG_RING_WHITE_MODE)
        {
            _workMode = FLPROG_RING_START_READ_VARIABLE_MODE;
            _tempString = "";
        }
        readNextVariable();
        return;
    }
    // Пока пакет принимается, не перемешиваем его с локальными отправками.
    if (_workMode != FLPROG_RING_WHITE_MODE)
    {
        return;
    }
    sendNextVariable();
}
