#pragma once
#include "flprogUtilites.h"
#include "flprogUartBase.h"
#include "utilites/flprogAbstractRingVariable.h"

// Состояния разбора кадра кольцевого UART-протокола FLProg.
// Формат кадра: 0x01 + sender + 0x02 + variableId + 0x03 + value + 0x04.
// Значения и идентификаторы передаются как ASCII; служебные байты не экранируются.
#define FLPROG_RING_WHITE_MODE 0
#define FLPROG_RING_START_READ_VARIABLE_MODE 1
#define FLPROG_RING_END_READ_VARIABLE_MODE 2
#define FLPROG_RING_READ_SENDER_VARIABLE_MODE 3
#define FLPROG_RING_READ_NAME_VARIABLE_MODE 4
#define FLPROG_RING_READ_VALUE_VARIABLE_MODE 5

// Обмен значениями переменных по кольцевому UART.
// Массивы указателей на переменные принадлежат объекту; сами переменные — вызывающему коду.
// executor должен оставаться действительным в течение всего срока жизни FLProgRing.
class FLProgRing
{
public:
    FLProgRing(uint8_t port, uint16_t readVariablesCount, uint16_t sendVariablesCount,
               uint32_t index, FlprogAbstractUartExecutor *executor);
    ~FLProgRing();

    // Добавить переменную в первую свободную ячейку соответствующего списка.
    // Если список заполнен, регистрация игнорируется.
    void addReadVariable(FLProgAbstractRingVariable *variable);
    void addSendVariable(FLProgAbstractRingVariable *variable);

    // Сколько вызовов pool() пропускать между обработками UART.
    uint16_t getSkippingEvents() { return _skippingEvents; }
    void setSkippingEvents(uint16_t value) { _skippingEvents = value; }

    uint32_t index() { return _index; } // ID локального участника кольца.

    bool isSingleMode() { return _isSingleMode; }
    void isSingleMode(bool value) { _isSingleMode = value; } // Без ретрансляции чужих кадров.

    // Неблокирующий шаг обработки входящего кадра или отправки одной переменной.
    // Важно вызывать регулярно: кадр может приходить по частям.
    void pool();

    uint8_t getPort() { return _port; }
    void setPort(uint8_t port) { _port = port; }

    // Объект держит динамические массивы указателей: копирование запрещено.
    FLProgRing(const FLProgRing &) = delete;
    FLProgRing &operator=(const FLProgRing &) = delete;

protected:
    void readNextVariable();
    void analizeNextReadingyte(uint8_t byte); // Сохранено историческое имя API.
    void sendNextVariable();
    void checkBuffer();
    void sendBuffer();
    int16_t nextSendVariableIndex();

    uint8_t _port = 0;
    FlprogAbstractUartExecutor *_executor = 0; // Не владеем объектом UART.
    uint32_t _index;
    bool _isSingleMode = false;
    uint8_t _workMode = FLPROG_RING_WHITE_MODE;
    uint32_t _timeout = 100; // Тайм-аут между байтами неполного кадра, мс.
    uint32_t _lastRxByteMillis = 0;
    bool _frameValid = false; // Сбрасывается при некорректном числовом ID.
    FLProgAbstractRingVariable **_readIitems = 0;
    FLProgAbstractRingVariable **_sendIitems = 0;
    uint16_t _readIitemsCount = 0;
    uint16_t _sendIitemsCount = 0;
    uint16_t _skippingEvents = 0;
    uint16_t _eventsCount = 0;

    String _tempString;
    uint32_t _tempSender = 0;
    uint32_t _tempName = 0;
};
