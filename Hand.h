#ifndef HAND_H
#define HAND_H

#include "Device.h"

typedef enum { hand_analog, hand_digital1, hand_digital2, hand_pwm, _Nbr_hand_pin } hand_pin_t;
int left_hand_pins[_Nbr_hand_pin] = { A0, 2, 4, 3 };
int right_hand_pins[_Nbr_hand_pin] = { A1, 7, 8, 5 };
class Hand : public Device{ // musio fist module virtual class
protected:
  int getHandPin(hand_pin_t type);
public:
  Hand(int pos):Device(pos){use_case = 0x00;}
  Hand(){}
};

int Hand::getHandPin(hand_pin_t type){
  int pin=-1;
  //Serial.print(dev_pos);
  if(dev_pos == 0)        pin = left_hand_pins[type];
  else if(dev_pos == 1)   pin = right_hand_pins[type];
  return pin;
}
#endif
