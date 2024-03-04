#pragma once
#include <Arduino.h>
#include "flprogUtilites.h"

class FLProgAbstractRingVariable
{
public:
    bool isNeedSend() { return _isNeedSend; };
    uint16_t index() { return _index; };

protected:
    bool _isNeedSend = false;
    uint16_t _index;
};

class FLProgBoolRingVariable : public FLProgAbstractRingVariable
{
public:
    FLProgBoolRingVariable(uint16_t index) { _index = index; };

    bool value() { return _value; };
    void value(bool value);

protected:
    bool _value;
};