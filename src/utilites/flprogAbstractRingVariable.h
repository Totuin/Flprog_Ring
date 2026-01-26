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

class FLProgByteRingVariable : public FLProgAbstractRingVariable
{
public:
    FLProgByteRingVariable(uint32_t index);

    uint8_t value() { return _value; };
    void value(uint8_t value);
    virtual void setValueString(String value);
    virtual String valueString();

protected:
    uint8_t _value;
};

class FLProgIntegerRingVariable : public FLProgAbstractRingVariable
{
public:
    FLProgIntegerRingVariable(uint32_t index);

    int16_t value() { return _value; };
    void value(int16_t value);
    virtual void setValueString(String value);
    virtual String valueString();

protected:
    int16_t _value;
};

class FLProgFloatRingVariable : public FLProgAbstractRingVariable
{
public:
    FLProgFloatRingVariable(uint32_t index);

    float value() { return _value; };
    void value(float value);
    virtual void setValueString(String value);
    virtual String valueString();

protected:
    float _value;
};

class FLProgLongRingVariable : public FLProgAbstractRingVariable
{
public:
    FLProgLongRingVariable(uint32_t index);

    int32_t value() { return _value; };
    void value(int32_t value);
    virtual void setValueString(String value);
    virtual String valueString();

protected:
    int32_t _value;
};

class FLProgUnsignedLonRingVariable : public FLProgAbstractRingVariable
{
public:
    FLProgUnsignedLonRingVariable(uint32_t index);

    int32_t value() { return _value; };
    void value(uint32_t value);
    virtual void setValueString(String value);
    virtual String valueString();

protected:
    uint32_t _value;
};

class FLProgCharRingVariable : public FLProgAbstractRingVariable
{
public:
    FLProgCharRingVariable(uint32_t index);

    char value() { return _value; };
    void value(char value);
    virtual void setValueString(String value);
    virtual String valueString();

protected:
    char _value;
};

class FLProgStringRingVariable : public FLProgAbstractRingVariable
{
public:
    FLProgStringRingVariable(uint32_t index);

    String value() { return _value; };
    void value(String value);
    virtual void setValueString(String value);
    virtual String valueString();

protected:
    String _value;
};