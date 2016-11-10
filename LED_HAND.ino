
#include "LED_hand.h"
#include "Hand.h"
#include "musio_ard.h"
Musio_Ard musio;
LED_hand *left, *right;

void setup() {
  left = new LED_hand(0);
  right = new LED_hand(1);
  musio.set_Device(left, right);
}

void loop() {
  musio.update();
}
