#ifndef EM4205_H
#define EM4205_H

#include <Arduino.h>
#include <SoftwareSerial.h>
extern SoftwareSerial rfidSerial;

String getResponse();
bool initRFID();
bool checkTag();

#endif
