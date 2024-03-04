#include "flprogRing.h"

FLProgRing::FLProgRing(uint8_t port, uint16_t readVariablesCount, uint16_t sendVariablesCount)
{
    _port = port;
    _readIitems = new FLProgAbstractRingVariable *[readVariablesCount];
    _sendIitems = new FLProgAbstractRingVariable *[sendVariablesCount];
}

void FLProgRing::pool()
{
    if (_eventsCount < _skippingEvents)
    {
        _eventsCount++;
        return;
    }
    _eventsCount = 0;
}