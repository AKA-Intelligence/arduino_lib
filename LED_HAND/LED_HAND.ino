
#include "LED_hand.h"
#include "Hand.h"
#include "musio_ard.h"
#include "testLower.h"
Musio_Ard musio;
LED_hand *left, *right;
testLower *low;
void setup() {
  left = new LED_hand(0);
  right = new LED_hand(1);
  low = new testLower();
  musio.add_Device(left);
  musio.add_Device(right);
  musio.add_Device(low);
  musio.init_devices();
}

void loop() {
  musio.update();
}
