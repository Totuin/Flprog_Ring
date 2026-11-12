#pragma once
#include "flprogUtilites.h"

// Базовый тип переменной кольцевого обмена.
// index — идентификатор переменной; isNeedSend — наличие локального изменения.
class FLProgAbstractRingVariable
{
public:
  virtual ~FLProgAbstractRingVariable() = default;
  bool isNeedSend() { return _isNeedSend; }
  void isNeedSend(bool value) { _isNeedSend = value; }
  uint32_t index() { return _index; }

  // Преобразование между типизированным значением и строковым полем протокола.
  // Полученное значение не помечается к отправке повторно (защита от эха).
  virtual void setValueString(String value) { (void)value; }
  virtual String valueString() { return ""; }

protected:
  bool _isNeedSend = false;
  uint32_t _index = 0;
};

// Локальный setter value(...) помечает переменную на отправку.
// В отличие от него, setValueString(...) применяется при приёме с UART.
class FLProgBoolRingVariable : public FLProgAbstractRingVariable
{
public:
  explicit FLProgBoolRingVariable(uint32_t index);
  bool value() { return _value; }
  void value(bool value);
  void setValueString(String value) override;
  String valueString() override;
protected:
  bool _value = false;
};

class FLProgByteRingVariable : public FLProgAbstractRingVariable
{
public:
  explicit FLProgByteRingVariable(uint32_t index);
  uint8_t value() { return _value; }
  void value(uint8_t value);
  void setValueString(String value) override;
  String valueString() override;
protected:
  uint8_t _value = 0;
};

class FLProgIntegerRingVariable : public FLProgAbstractRingVariable
{
public:
  explicit FLProgIntegerRingVariable(uint32_t index);
  int16_t value() { return _value; }
  void value(int16_t value);
  void setValueString(String value) override;
  String valueString() override;
protected:
  int16_t _value = 0;
};

class FLProgFloatRingVariable : public FLProgAbstractRingVariable
{
public:
  explicit FLProgFloatRingVariable(uint32_t index);
  float value() { return _value; }
  void value(float value);
  void setValueString(String value) override;
  String valueString() override;
protected:
  float _value = 0.0f;
};

class FLProgLongRingVariable : public FLProgAbstractRingVariable
{
public:
  explicit FLProgLongRingVariable(uint32_t index);
  int32_t value() { return _value; }
  void value(int32_t value);
  void setValueString(String value) override;
  String valueString() override;
protected:
  int32_t _value = 0;
};

class FLProgUnsignedLonRingVariable : public FLProgAbstractRingVariable
{
public:
  explicit FLProgUnsignedLonRingVariable(uint32_t index);
  // Полный диапазон unsigned long: 0..4294967295.
  uint32_t value() { return _value; }
  void value(uint32_t value);
  void setValueString(String value) override;
  String valueString() override;
protected:
  uint32_t _value = 0;
};

class FLProgCharRingVariable : public FLProgAbstractRingVariable
{
public:
  explicit FLProgCharRingVariable(uint32_t index);
  char value() { return _value; }
  void value(char value);
  void setValueString(String value) override;
  String valueString() override;
protected:
  char _value = 0;
};

class FLProgStringRingVariable : public FLProgAbstractRingVariable
{
public:
  explicit FLProgStringRingVariable(uint32_t index);
  String value() { return _value; }
  void value(String value);
  void setValueString(String value) override;
  String valueString() override;
protected:
  String _value;
};
