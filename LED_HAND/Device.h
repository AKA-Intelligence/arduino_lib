#ifndef DEVICE_H
#define DEVICE_H
#include "Musio_Common.h"
class Device{
protected :
  uint8_t dev_pos;
  uint8_t devid;
public:
  Device(int pos):dev_pos(pos){}
  uint8_t getDevid(){return devid;}
  uint8_t getDevPos(){return dev_pos;}
  virtual void triggerCMD(uint8_t code, uint8_t* data, uint8_t len)=0;
  virtual void update()=0; // every time in loop, check modules state
  virtual void init()=0;
  int getDevPin(pin_t type){
    Pin_List[type].used++; 
    return Pin_List[type].pin;
  }
};
#endif
