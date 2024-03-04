#pragma once
#include <Arduino.h>
#include "flprogUtilites.h"

class FLProgAbstractRingVariable
{
public:
    bool isNeedSend() { return _isNeedSend; };
    void isNeedSend(bool value) { _isNeedSend = value; };
    uint32_t index() { return _index; };
    virtual void setValueString(String value) { (void)value; };
    virtual String valueString() { return ""; };

protected:
    bool _isNeedSend = false;
    uint32_t _index;
};

class FLProgBoolRingVariable : public FLProgAbstractRingVariable
{
public:
    FLProgBoolRingVariable(uint32_t index);

    bool value() { return _value; };
    void value(bool value);
    virtual void setValueString(String value);
    virtual String valueString();

protected:
    bool _value;
};