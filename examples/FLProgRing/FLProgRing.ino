#include "flprogRing.h"
#include "flprogUart.h"

// Пример для платы с аппаратным UART №0. Реальный порт и разводку RX/TX
// необходимо подобрать под свою плату. UART используется для обмена,
// поэтому Serial Monitor на этом же порту может мешать работе кольца.
FlprogUartExecutor uartExecutor;
FLProgRing ring(0, 8, 8, 12345678UL, &uartExecutor);

FLProgBoolRingVariable readBoolVar(1);
FLProgBoolRingVariable writeBoolVar(2);
FLProgByteRingVariable readByteVar(3);
FLProgByteRingVariable writeByteVar(4);
FLProgIntegerRingVariable readIntegerVar(5);
FLProgIntegerRingVariable writeIntegerVar(6);
FLProgFloatRingVariable readFloatVar(7);
FLProgFloatRingVariable writeFloatVar(8);
FLProgLongRingVariable readLongVar(9);
FLProgLongRingVariable writeLongVar(10);
FLProgUnsignedLonRingVariable readUnsignedLongVar(11);
FLProgUnsignedLonRingVariable writeUnsignedLonVar(12);
FLProgCharRingVariable readCharVar(13);
FLProgCharRingVariable writeCharVar(14);
FLProgStringRingVariable readStringVar(15);
FLProgStringRingVariable writeStringVar(16);

void setup()
{
    uartExecutor.beginUart(0);
    ring.addReadVariable(&readBoolVar);
    ring.addSendVariable(&writeBoolVar);
    ring.addReadVariable(&readByteVar);
    ring.addSendVariable(&writeByteVar);
    ring.addReadVariable(&readIntegerVar);
    ring.addSendVariable(&writeIntegerVar);
    ring.addReadVariable(&readFloatVar);
    ring.addSendVariable(&writeFloatVar);
    ring.addReadVariable(&readLongVar);
    ring.addSendVariable(&writeLongVar);
    ring.addReadVariable(&readUnsignedLongVar);
    ring.addSendVariable(&writeUnsignedLonVar);
    ring.addReadVariable(&readCharVar);
    ring.addSendVariable(&writeCharVar);
    ring.addReadVariable(&readStringVar);
    ring.addSendVariable(&writeStringVar);
}

void loop()
{
    ring.pool();
    // Пример записи локальной переменной:
    // writeBoolVar.value(true); // Теперь значение ожидает отправки по кольцу.
}
