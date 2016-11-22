#ifndef LED_HAND_H
#define LED_HAND_H
#include "timerController.h"
#include "Hand.h"
#include <Arduino.h>
#include <inttypes.h>
#define LED_HAND_DEV_ID 0x01

#define LED_ON_CMD_CODE 0x61
#define LED_OFF_CMD_CODE 0x62
#define LED_DIM_CMD_CODE 0x63


#define LED_LEFT_PIN A0
#define LED_RIGHT_PIN A1

int led_bri_l=0;
int led_bri_r=0;

void update_LED_R_callback(){
   PORTC &= 0x1d;    // turn off led2
   setTimer2OutPutComaperB(led_bri_r);//update brightness too
}
void update_LED_L_callback(){
   PORTC &= 0x1e;
   setTimer2OutPutComaperA(led_bri_l);//update the brightness of led1 for next cycles
}
void tmr_ovf_callback(){
    setTimer2OutPutComaperA(led_bri_l);//update brightness of led1 too. OCR2A holds the value of to compare with timmer
    setTimer2OutPutComaperB(led_bri_r);//update brightness too
    if(led_bri_l>10) {
      PORTC |= 0x01;//set the led1 on. Using OR operator cause don't touch to other pins
    }
    if(led_bri_r>10) {
      PORTC |= 0x02;//set the led1 on. Using OR operator cause don't touch to other pins
    }
}
class LED_hand: public Hand {
  private:  
  int fadeAmount;  
  int brightness;
  int dim_flag;
  int dim_interval;
  unsigned long previousMillis ;
  void led_off_callback(uint8_t *cmd_data);
  void led_on_callback(uint8_t *cmd_data);
  void led_dim_callback(uint8_t *cmd_data);
  void timerSetup();
  public:
  void init();
  void update();
  int led_pin;
  void triggerCMD(uint8_t code, uint8_t *data, uint8_t len);
  LED_hand(int dev_pos) :Hand(dev_pos){};
};

void LED_hand::update(){
  
  if(dim_flag==1){
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= dim_interval){
      brightness += fadeAmount;
      if (brightness <= 0 || brightness >= 255) {
        fadeAmount = -fadeAmount;
      }
      previousMillis = currentMillis;
      if(dev_pos==0)      led_bri_l = brightness; 
      if(dev_pos==1)      led_bri_r = brightness;
    }
  }
}

void LED_hand::timerSetup(){
    //set pins as outputs
    setTimer2Config(TIMER_WGM_FAST_PWM, PRESCAL_256);
    setTimer2Handlers(tmr_ovf_callback,update_LED_L_callback,update_LED_R_callback);
}
void LED_hand::led_dim_callback(uint8_t *cmd_data){
   dim_flag=1;
   dim_interval = *(cmd_data);
}

void LED_hand::init(){
    devid = LED_HAND_DEV_ID;    
    dim_flag = 0;
    dim_interval = 50;
    brightness = 0;
    fadeAmount = 5;
    previousMillis =0;
    
    led_pin = getHandPin(hand_analog);
    pinMode(led_pin, OUTPUT);
    timerSetup();
  }
  
  void LED_hand::led_off_callback(uint8_t *cmd_data){
    dim_flag = 0;
    
    if(dev_pos==0) {
      led_bri_l = 0;
      analogWrite(led_pin,led_bri_l) ;
    }
    else if(dev_pos==1) {
      led_bri_r = 0;
      analogWrite(led_pin,led_bri_r) ;
    }
  }
  void LED_hand::led_on_callback(uint8_t *cmd_data){
    dim_flag = 0;
    if(dev_pos==0) {
      led_bri_l = *(cmd_data);
      analogWrite(led_pin,led_bri_l) ;
    }
    else if(dev_pos==1) {
      led_bri_r = *(cmd_data);
      analogWrite(led_pin,led_bri_r) ;
    }
  }
  
  void LED_hand::triggerCMD(uint8_t code, uint8_t *data, uint8_t len){
    if(code == LED_ON_CMD_CODE)       led_on_callback(data);
    else if(code == LED_OFF_CMD_CODE)  led_off_callback(data);
    else if(code == LED_DIM_CMD_CODE)  led_dim_callback(data);
  }
#endif
