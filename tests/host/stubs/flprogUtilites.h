#pragma once
#include <stdint.h>
#include <stddef.h>
#include <cstring>
#include <string>
#include <cstdlib>
#include <sstream>
#include <iomanip>

inline uint32_t& fakeMillisState() { static uint32_t clock=0; return clock; }
inline uint32_t millis() { return fakeMillisState(); }

class String {
public:
 String() = default;
 String(const char* v):s(v ? v : ""){}
 String(std::string v):s(v){}
 String(char v):s(1,v){}
 String(unsigned char v):s(std::to_string((unsigned)v)){}
 String(signed char v):s(std::to_string((int)v)){}
 String(short v):s(std::to_string((int)v)){}
 String(unsigned short v):s(std::to_string((unsigned)v)){}
 String(int v):s(std::to_string(v)){}
 String(unsigned int v):s(std::to_string(v)){}
 String(long v):s(std::to_string(v)){}
 String(unsigned long v):s(std::to_string(v)){}
 String(float v,int digits) { std::ostringstream out;out<<std::fixed<<std::setprecision(digits)<<v;s=out.str(); }
 String& operator=(const char* v){s=v ? v : ""; return *this;}
 String& operator+=(char ch){s+=ch;return *this;}
 String& operator+=(const String& rhs){s+=rhs.s;return *this;}
 bool operator==(const String& rhs)const{return s==rhs.s;}
 bool operator==(const char* rhs)const{return s==rhs;}
 size_t length()const{return s.length();}
 char charAt(size_t i)const{return s.at(i);}
 long toInt()const{return std::strtol(s.c_str(),nullptr,10);}
 float toFloat()const{return std::strtof(s.c_str(),nullptr);}
 const std::string& raw()const{return s;}
private: std::string s;
};
namespace flprog {
inline bool stringToBool(String s) { return s.raw()=="1" || s.raw()=="true"; }
inline char stringToChar(String s) { return s.length()?s.charAt(0):0; }
}
