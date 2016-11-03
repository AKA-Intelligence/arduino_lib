#ifndef HAND_H
#define HAND_H

#include <inttypes.h>

class Hand{ // musio fist module virtual class
protected:
  uint8_t LRvalue;
  uint8_t devid;
  int8_t use_case;
public:
  Hand(int LRvalue):LRvalue(LRvalue){}
  Hand(){}
  uint8_t getDevid(){return devid;}
  uint8_t getUseCase(){return use_case;}
  virtual void triggerCMD(uint8_t code, uint8_t* data){}
  virtual void update(){} // every time in loop, check modules state
  virtual void init(){}
};
#endif
