#ifndef LOWER_H
#define LOWER_H

#include "Device.h"
#include "Musio_Common.h"


class Lower : public Device{
  public:
  Lower(uint8_t user_def_dev_id):Device(2){ devid = (0x10 || user_def_dev_id); } // user define device id is 0x1n
  Lower():Device(2){}  
  protected:
  int getLowerPin(int type);
};
int Lower::getLowerPin(int type){
  return getDevPin(type);
}
#endif
