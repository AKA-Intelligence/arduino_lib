
#include "LED_hand.h"
#include "Musio_ard.h"
#include "VehicleMotor.h"
Musio_Ard musio;
LED_hand *left, *right;
VehicleMotor *motor;
void setup() {
  left = new LED_hand(0);
  right = new LED_hand(1);
  motor = new VehicleMotor();
  musio.add_Device(left);
  musio.add_Device(right);
  musio.add_Device(motor);
  musio.init_devices();
}

void loop() {
  musio.update();
}
