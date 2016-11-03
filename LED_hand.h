#ifndef LED_HAND_H
#define LED_HAND_H

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


ISR(TIMER2_COMPB_vect){//timer2-channel B interrupt service routine for led2
    PORTC &= 0x1d;    // turn off led2
    OCR2B = led_bri_r;//update the brightness of led2 for next cycles
}
ISR(TIMER2_COMPA_vect){//timer2-channel A interrupt service routine for led 1
    PORTC &= 0x1e;
   // PORTC = (PINC & 0xfe) | (0x01  ^ (PINC & 0x01));
    OCR2A = led_bri_l;//update the brightness of led1 for next cycles

}
ISR(TIMER2_OVF_vect){//overflow for timmer 2,
    OCR2A = led_bri_l;//update brightness of led1 too. OCR2A holds the value of to compare with timmer
    OCR2B = led_bri_r;//update brightness too
    if(led_bri_l>10) PORTC |= 0x01;//set the led1 off. Using OR operator cause don't touch to other pins
    if(led_bri_r>10) PORTC |= 0x02;//set the led1 off. Using OR operator cause don't touch to other pins
}

class LED_hand: public Hand {
  private:  
  int fadeAmount;  
  int brightness;
  int dim_flag;
  int dim_interval;
 // uint8_t dim_interval_l;
 // uint8_t dim_interval_r;
  unsigned long previousMillis ;
  int led_pin;
  void led_off_callback(uint8_t *cmd_data);
  void led_on_callback(uint8_t *cmd_data);
  void led_dim_callback(uint8_t *cmd_data);
  void interrupt_setup();
  public:
  void init(int LRvalue);
  void update();
  void triggerCMD(uint8_t code, uint8_t *data);
  LED_hand(int LRvalue);
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
      if(LRvalue==0)      led_bri_l = brightness; 
      if(LRvalue==1)      led_bri_r = brightness;
    }
  }
}

void LED_hand::interrupt_setup(){
    //set pins as outputs
    cli();//stop interrupts
    if(LRvalue == 0){
      TCCR2A = 0;// set entire TCCR2A register to 0 to prepare for modifying it
      TCNT2  = 0;//initialize counter value to 0 - this register is counting register, timmer count something and save to here
      OCR2A = 2;// initial value of compare value of channel A
      TCCR2A |= 0x03;// turn on fast pwm mode, checking datasheet to know what is fast pwm mode and when interrupt were triggered
      TIMSK2 |= 0x03;
    }
    else if(LRvalue == 1){
      TCCR2B = 0;// set entire TCCR2A register to 0 to prepare for modifying it
      TCNT2  = 0;//initialize counter value to 0 - this register is counting register, timmer count something and save to here
      OCR2B = 2;// initial value of compare value of channel B
      TCCR2B |= 0x04;// Set CS21 bit for 64 prescaler. the default pwm is using prescaler of 64. So just use it. lowwer value will dimming led not very smooth. Too high frequency make interrupt all time, mainloop cannot run any command
      TIMSK2 |= 0x06;
    }
    sei();//allow general interrupts in general control register
}
void LED_hand::led_dim_callback(uint8_t *cmd_data){
   dim_flag=1;
   dim_interval = *(cmd_data);
}

void LED_hand::init(int LR){
    LRvalue = LR;
    devid = LED_HAND_DEV_ID;
    use_case = 0;
    dim_flag = 0;
    dim_interval = 50;
    brightness = 0;
    fadeAmount = 5;
    previousMillis =0;
    if(LRvalue == 0){  //config pin mode setting
      // left pin A0 2 4 3
      led_pin = LED_LEFT_PIN;
    }
    else if(LRvalue == 1){
      // right pin A1 7 8 5
      led_pin = LED_RIGHT_PIN;
    }
    pinMode(led_pin, OUTPUT);
    interrupt_setup();
  }
  
  void LED_hand::led_off_callback(uint8_t *cmd_data){
    dim_flag = 0;
    
    if(LRvalue==0) {
      led_bri_l = 0;
      analogWrite(led_pin,led_bri_l) ;
    }
    else if(LRvalue==1) {
      led_bri_r = 0;
      analogWrite(led_pin,led_bri_r) ;
    }
  }
  void LED_hand::led_on_callback(uint8_t *cmd_data){
    dim_flag = 0;
    if(LRvalue==0) {
      led_bri_l = *(cmd_data);
      analogWrite(led_pin,led_bri_l) ;
    }
    else if(LRvalue==1) {
      led_bri_r = *(cmd_data);
      analogWrite(led_pin,led_bri_r) ;
    }
  }
  
  void LED_hand::triggerCMD(uint8_t code, uint8_t *data){
    //  Serial.print(code);
    if(LRvalue==0){
      led_pin = LED_LEFT_PIN;
    }
    else{
      led_pin = LED_RIGHT_PIN;
    }
    if(code == LED_ON_CMD_CODE)       led_on_callback(data);
    else if(code == LED_OFF_CMD_CODE)  led_off_callback(data);
    else if(code == LED_DIM_CMD_CODE)  led_dim_callback(data);
  }
  LED_hand::LED_hand(int LRvalue){
    init(LRvalue);
  }
#endif
