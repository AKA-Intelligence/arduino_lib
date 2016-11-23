#ifndef HAND_H
#define HAND_H

#include "Device.h"
#include "Musio_Common.h"
typedef enum { LEFT, RIGHT, _Nbr_hand } hand_type;
typedef enum { hand_analog, hand_digital1, hand_digital2, hand_pwm, _Nbr_hand_pin } hand_pin_t;
pin_t left_hand_pins[_Nbr_hand_pin] = {  analog0, digital1, digital2, pwm1 };
pin_t right_hand_pins[_Nbr_hand_pin] = { analog1, digital3, digital4, pwm2 };
class Hand : public Device{ // musio fist module virtual class
protected:
  int getHandPin(hand_pin_t type);
public:
  Hand(int pos):Device(pos){}
};

int Hand::getHandPin(hand_pin_t type){
  int pin_type ;
  if (dev_pos == 0) pin_type = left_hand_pins[type];
  else if (dev_pos == 1) pin_type = right_hand_pins[type];
  return getDevPin(pin_type);
}
#endif
