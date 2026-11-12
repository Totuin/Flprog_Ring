#pragma once
#include "flprogUtilites.h"
class FlprogAbstractUartExecutor {
public:
 virtual ~FlprogAbstractUartExecutor() = default;
 virtual void beginUart(uint8_t) = 0;
 virtual void endUart(uint8_t) = 0;
 virtual int availableUart(uint8_t) = 0;
 virtual uint8_t readUart(uint8_t) = 0;
 virtual void writeUart(uint8_t,uint8_t) = 0;
 virtual void printUart(String,uint8_t) = 0;
 virtual void writeUart(uint8_t*,uint16_t,uint8_t) = 0;
 virtual uint32_t getSpeedUart(uint8_t) = 0;
 virtual void setSpeedUart(uint32_t,uint8_t) = 0;
 virtual uint8_t getStatusUart(uint8_t) = 0;
 virtual uint8_t getDataBitUart(uint8_t) = 0;
 virtual void setDataBitUart(uint8_t,uint8_t) = 0;
 virtual uint8_t getStopBitUart(uint8_t) = 0;
 virtual void setStopBitUart(uint8_t,uint8_t) = 0;
 virtual uint8_t getParityUart(uint8_t) = 0;
 virtual void setParityUart(uint8_t,uint8_t) = 0;
 virtual uint8_t getPinRxUart(uint8_t) = 0;
 virtual void setPinRxUart(uint8_t,uint8_t) = 0;
 virtual uint8_t getPinTxUart(uint8_t) = 0;
 virtual void setPinTxUart(uint8_t,uint8_t) = 0;
 virtual void setPinsUart(uint8_t,uint8_t,uint8_t) = 0;
};
