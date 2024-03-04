#pragma once
#include <Arduino.h>
#include "flprogUtilites.h"
#include "utilites/flprogAbstractRingVariable.h"

#define FLPROG_RING_WHITE_MODE 0
#define FLPROG_RING_START_READ_VARIABLE_MODE 1
#define FLPROG_RING_END_READ_VARIABLE_MODE 2
#define FLPROG_RING_READ_SENDER_VARIABLE_MODE 3
#define FLPROG_RING_READ_NAME_VARIABLE_MODE 4
#define FLPROG_RING_READ_VALUE_VARIABLE_MODE 5

class FLProgRing
{
public:
    FLProgRing(uint8_t port, uint16_t readVariablesCount, uint16_t sendVariablesCount, uint32_t index);

    void addReadVariable(FLProgAbstractRingVariable *variable);
    void addSendVariable(FLProgAbstractRingVariable *variable);

    uint16_t getSkippingEvents() { return _skippingEvents; };
    void setSkippingEvents(uint16_t value) { _skippingEvents = value; };

    uint32_t index() { return _index; };

    bool isSingleMode() { return _isSingleMode; };
    void isSingleMode(bool value) { _isSingleMode = value; };

    void pool();

protected:
    void readNextVariable();
    void analizeNextReadingyte(uint8_t byte);
    void sendNextVariable();
    void checkBuffer();
    void sendBuffer();
    int16_t nextSendVariableIndex();

    uint8_t _port = 0;
    uint32_t _index;
    bool _isSingleMode = false;
    uint8_t _workMode = FLPROG_RING_WHITE_MODE;
    uint32_t _timeout = 100;
    FLProgAbstractRingVariable **_readIitems = 0;
    FLProgAbstractRingVariable **_sendIitems = 0;
    uint16_t _readIitemsCount = 0;
    uint16_t _sendIitemsCount = 0;
    uint16_t _skippingEvents = 0;
    uint16_t _eventsCount = 0;

    String _tempString;
    uint32_t _tempSender;
    uint32_t _tempName;
};