#include "flprogAbstractRingVariable.h"

FLProgBoolRingVariable::FLProgBoolRingVariable(uint32_t index)
{
    _index = index;
}

//----------------------------------------FLProgBoolRingVariable----------------------------------
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