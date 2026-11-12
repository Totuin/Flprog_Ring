#include "flprogRing.h"
#include <cassert>
#include <cstdint>
#include <iostream>
#include <deque>
#include <vector>
#include <string>
#include <type_traits>

struct FakeUart : FlprogAbstractUartExecutor {
 std::deque<uint8_t> in; std::vector<uint8_t> out;
 void add(std::string s){for (unsigned char c:s)in.push_back(c);}
 void add(std::vector<uint8_t> b){for(auto c:b)in.push_back(c);}
 std::string output()const{return std::string(out.begin(),out.end());}
 void clear(){out.clear();}
 void beginUart(uint8_t) override {}
 void endUart(uint8_t) override {}
 int availableUart(uint8_t) override {return int(in.size());}
 uint8_t readUart(uint8_t) override {auto c=in.front();in.pop_front();return c;}
 void writeUart(uint8_t c,uint8_t) override {out.push_back(c);}
 void printUart(String s,uint8_t) override {for(unsigned char c:s.raw())out.push_back(c);}
 void writeUart(uint8_t *b,uint16_t n,uint8_t) override {for(int i=0;i<n;i++)out.push_back(b[i]);}
 uint32_t getSpeedUart(uint8_t) override {return 9600;}
 void setSpeedUart(uint32_t,uint8_t) override {}
 uint8_t getStatusUart(uint8_t) override {return 1;}
 uint8_t getDataBitUart(uint8_t) override {return 8;}
 void setDataBitUart(uint8_t,uint8_t) override {}
 uint8_t getStopBitUart(uint8_t) override {return 1;}
 void setStopBitUart(uint8_t,uint8_t) override {}
 uint8_t getParityUart(uint8_t) override {return 0;}
 void setParityUart(uint8_t,uint8_t) override {}
 uint8_t getPinRxUart(uint8_t) override {return 0;}
 void setPinRxUart(uint8_t,uint8_t) override {}
 uint8_t getPinTxUart(uint8_t) override {return 0;}
 void setPinTxUart(uint8_t,uint8_t) override {}
 void setPinsUart(uint8_t,uint8_t,uint8_t) override {}
};

std::string frame(const std::string& sender,const std::string& var,const std::string& val) {
 return std::string(1,'\1')+sender+'\2'+var+'\3'+val+'\4';
}
int main(){
 static_assert(std::is_same<decltype(std::declval<FLProgUnsignedLonRingVariable>().value()),uint32_t>::value,"Unsigned getter");
 {
  FakeUart uart;
  FLProgRing ring(1,2,2,1,&uart);
  FLProgUnsignedLonRingVariable u(4294967295U);
  FLProgUnsignedLonRingVariable out(2);
  assert(u.value()==0);
  FLProgBoolRingVariable b(3); assert(!b.value());
  FLProgIntegerRingVariable i(4);assert(i.value()==0);
  FLProgFloatRingVariable f(5);assert(f.value()==0);
  FLProgCharRingVariable c(6);assert(c.value()==0);
  ring.addReadVariable(&u);
  ring.addSendVariable(&out);
  out.value(4000000000U);
  ring.pool();
  assert(uart.output()==frame("1","2","4000000000"));
  assert(!out.isNeedSend());
  uart.clear();
  // Сегменты поступают между разными вызовами pool().
  std::string msg=frame("4294967294","4294967295","4294967295");
  for(char ch:msg){uart.add(std::string(1,ch)); fakeMillisState()+=1; ring.pool();}
  assert(u.value()==4294967295U);
  assert(uart.output()==msg); // чужой кадр транслируется дальше.
  uart.clear();
  // Собственный кадр читается, но НЕ пересылается обратно в кольцо.
  uart.add(frame("1","4294967295","4100000000"));ring.pool();
  assert(u.value()==4100000000U && uart.out.empty());
  // Строка со значением >UINT32_MAX не меняет переменную.
  u.setValueString("4294967296");assert(u.value()==4100000000U);
 }
 {
  FakeUart uart;
  FLProgRing ring(0,1,1,10,&uart);
  FLProgLongRingVariable read(50);ring.addReadVariable(&read);
  FLProgBoolRingVariable send(60);ring.addSendVariable(&send);
  ring.isSingleMode(true);
  uart.add(frame("12","50","-42"));ring.pool();
  assert(read.value()==-42 && uart.out.empty());
  uart.add(frame("not_a_number","50","111"));ring.pool();
  assert(read.value()==-42 && uart.out.empty());
  uart.add(frame("4294967296","50","222"));ring.pool();
  assert(read.value()==-42 && uart.out.empty());
  uart.add(frame("12","50","123"));ring.pool();assert(read.value()==123);
 }
 {
  FakeUart uart;
  FLProgRing ring(1,1,1,100,&uart);
  FLProgIntegerRingVariable read(80);ring.addReadVariable(&read);
  // Брошенный неполный кадр не должен портить следующий после тайм-аута.
  uart.add("\1" "101" "\2" "80" "\3" "777");ring.pool();
  fakeMillisState()+=101;
  // Новый кадр уже пришёл в UART до pool(): всё равно начинаем с нового старта.
  uart.add(frame("101","80","32"));ring.pool();
  assert(read.value()==32);
 }
 {
  FakeUart uart;
  FLProgRing ring(1,0,1,100,&uart);
  FLProgBoolRingVariable write(55);ring.addSendVariable(&write);
  ring.setSkippingEvents(2);
  write.value(true);
  ring.pool();ring.pool();assert(uart.out.empty());
  ring.pool();assert(uart.output()==frame("100","55","1"));
 }
 {
  // Вызовы вблизи переполнения millis() не должны сбрасывать кадр раньше времени.
  fakeMillisState()=UINT32_MAX-3;
  FakeUart uart;
  FLProgRing ring(2,1,0,1,&uart);
  FLProgIntegerRingVariable r(5);ring.addReadVariable(&r);
  uart.add("\1" "7" "\2" "5" "\3");ring.pool();
  fakeMillisState()=3;ring.pool();
  uart.add("19\4");ring.pool();assert(r.value()==19);
 }
 {
  FakeUart uart;
  FLProgRing ring(1,1,1,2,nullptr); ring.pool();
 }
 std::cout<<"PASS: fragmentation / forwarding / own sender / uint32 / malformed IDs / timeouts / millis wrap / skip / null executor\n";
}
