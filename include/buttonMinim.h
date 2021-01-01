#pragma once

#ifndef BUTTONMINIM_H
#define BUTTONMINIM_H

#include <Arduino.h>

#pragma pack(push,1)
typedef struct {
  bool holdedFlag: 1;
  bool btnFlag: 1;
  bool pressF: 1;
  bool clickF: 1;
  bool holdF: 1;
} buttonMinimFlags;
#pragma pack(pop)

class buttonMinim {
  public:
    buttonMinim(uint8_t pin, boolean type);   // type true - pullDOWN, false - input pullUP
    boolean pressed();
    boolean clicked();
    boolean holding();
    boolean holded();
  private:
    buttonMinimFlags flags;
    void tick();
    uint32_t _btnTimer;
    byte _pin;
    boolean _type;
};

#endif // BUTTONMINIM_H
