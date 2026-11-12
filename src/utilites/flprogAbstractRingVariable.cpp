#include "flprogAbstractRingVariable.h"

// Реализации типизированных значений кольцевого обмена.
// Локальный setter помечает значение к отправке, а входящий UART — нет.
// Строковый формат унаследован от версии 1.0.0.

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
    // Точность 2 знака — исторический формат обмена; не меняем её без
    // согласования протокола между всеми устройствами кольца.
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
    // String::toInt() использует знаковый long и обрезает часть uint32_t.
    // Читаем десятичную запись без знака, с проверкой переполнения.
    if (value.length() == 0)
    {
        return;
    }
    uint32_t parsed = 0;
    for (size_t i = 0; i < value.length(); ++i)
    {
        char ch = value.charAt(i);
        if (ch < '0' || ch > '9')
        {
            return;
        }
        uint8_t digit = static_cast<uint8_t>(ch - '0');
        if (parsed > (UINT32_MAX - digit) / 10U)
        {
            return;
        }
        parsed = parsed * 10U + digit;
    }
    _value = parsed;
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