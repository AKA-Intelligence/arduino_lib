#ifndef DEVICE_H
#define DEVICE_H

class Device{
protected :
  uint8_t dev_pos;
  uint8_t devid;
  int8_t use_case;
public:
  explicit Device(int pos):dev_pos(pos){}
  Device(){}
  uint8_t getDevid(){return devid;}
  uint8_t getUseCase(){return use_case;}
  uint8_t getDevPos(){return dev_pos;}
  virtual void triggerCMD(uint8_t code, uint8_t* data, uint8_t len){}
  virtual void update(){} // every time in loop, check modules state
  virtual void init(){}
};
#endif
