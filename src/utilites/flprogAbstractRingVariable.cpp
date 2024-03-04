#include "flprogAbstractRingVariable.h"

//----------------------------------------FLProgBoolRingVariable----------------------------------
void FLProgBoolRingVariable::value(bool value)
{
    if (value != _value)
    {
        _isNeedSend = true;
        +value = value;
    }
}