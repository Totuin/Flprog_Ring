#include "flprogAbstractRingVariable.h"

//----------------------------------------FLProgBoolRingVariable----------------------------------

FLProgBoolRingVariable::FLProgBoolRingVariable(uint32_t index)
{
    _index = index;
}

void FLProgBoolRingVariable::value(bool value)
{
    _isNeedSend = true;
    _value = value;
}

void FLProgBoolRingVariable::setValueString(String value)
{
    _value = flprog::stringToBool(value);
}

String FLProgBoolRingVariable::valueString()
{
    if (_value)
    {
        return "1";
    }
    return "0";
};

//----------------------------------------FLProgByteRingVariable----------------------------------

FLProgByteRingVariable::FLProgByteRingVariable(uint32_t index)
{
    _index = index;
}

void FLProgByteRingVariable::value(uint8_t value)
{
    _isNeedSend = true;
    _value = value;
}

void FLProgByteRingVariable::setValueString(String value)
{
    _value = value.toInt();
}

String FLProgByteRingVariable::valueString()
{
    return String(_value);
};

//----------------------------------------FLProgIntegerRingVariable----------------------------------
FLProgIntegerRingVariable::FLProgIntegerRingVariable(uint32_t index)
{
    _index = index;
}

void FLProgIntegerRingVariable::value(int16_t value)
{
    _isNeedSend = true;
    _value = value;
}

void FLProgIntegerRingVariable::setValueString(String value)
{
    _value = value.toInt();
}

String FLProgIntegerRingVariable::valueString()
{
    return String(_value);
};

//----------------------------------------FLProgFloatRingVariable----------------------------------
FLProgFloatRingVariable::FLProgFloatRingVariable(uint32_t index)
{
    _index = index;
}

void FLProgFloatRingVariable::value(float value)
{
    _isNeedSend = true;
    _value = value;
}

void FLProgFloatRingVariable::setValueString(String value)
{
    _value = value.toFloat();
}

String FLProgFloatRingVariable::valueString()
{
    return String(_value, 2);
};

//----------------------------------------FLProgLongRingVariable----------------------------------
FLProgLongRingVariable::FLProgLongRingVariable(uint32_t index)
{
    _index = index;
}

void FLProgLongRingVariable::value(int32_t value)
{
    _isNeedSend = true;
    _value = value;
}

void FLProgLongRingVariable::setValueString(String value)
{
    _value = value.toInt();
}

String FLProgLongRingVariable::valueString()
{
    return String(_value);
};

//----------------------------------------FLProgUnsignedLonRingVariable----------------------------------
FLProgUnsignedLonRingVariable::FLProgUnsignedLonRingVariable(uint32_t index)
{
    _index = index;
}

void FLProgUnsignedLonRingVariable::value(uint32_t value)
{
    _isNeedSend = true;
    _value = value;
}

void FLProgUnsignedLonRingVariable::setValueString(String value)
{
    _value = (uint32_t)(value.toInt());
}

String FLProgUnsignedLonRingVariable::valueString()
{
    return String(_value);
};

//----------------------------------------FLProgCharRingVariable----------------------------------
FLProgCharRingVariable::FLProgCharRingVariable(uint32_t index)
{
    _index = index;
}

void FLProgCharRingVariable::value(char value)
{
    _isNeedSend = true;
    _value = value;
}

void FLProgCharRingVariable::setValueString(String value)
{
    _value = flprog::stringToChar(value);
}

String FLProgCharRingVariable::valueString()
{
    return String(_value);
};

//----------------------------------------FLProgStringRingVariable----------------------------------
FLProgStringRingVariable::FLProgStringRingVariable(uint32_t index)
{
    _index = index;
}

void FLProgStringRingVariable::value(String value)
{
    _isNeedSend = true;
    _value = value;
}

void FLProgStringRingVariable::setValueString(String value)
{
    _value = value;
}

String FLProgStringRingVariable::valueString()
{
    return _value;
};