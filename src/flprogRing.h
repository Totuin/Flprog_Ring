#pragma once
#include <Arduino.h>
#include "flprogUtilites.h"
#include "utilites/flprogAbstractRingVariable.h'

#define FLPROG_RING_WHITE_MODE 0

class FLProgRing
{
public:
    FLProgRing(uint8_t port, uint16_t readVariablesCount, uint16_t sendVariablesCount);

    uint16_t getSkippingEvents() { return _skippingEvents; };
    void setSkippingEvents(uint16_t value) { _skippingEvents = value; };

    void pool();

protected:
    uint8_t _port = 0;
    uint8_t workMode = FLPROG_RING_WHITE_MODE;
    uint32_t _timeout = 100;
    FLProgAbstractRingVariable **_readIitems = 0;
    FLProgAbstractRingVariable **_sendIitems = 0;
    uint16_t _skippingEvents = 0;
    uint16_t _eventsCount = 0;
};