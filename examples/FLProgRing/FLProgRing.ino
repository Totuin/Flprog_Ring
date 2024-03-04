#include "flprogRing.h"

FLProgRing ring(0, 7, 7, 12345678);

FLProgBoolRingVariable readBoolVar(1);
FLProgBoolRingVariable writeBoolVar(5);

void setup()
{
  ring.addReadVariable(&readBoolVar);
  ring.addSendVariable(&writeBoolVar);
}

void loop()
{
  ring.pool();
}