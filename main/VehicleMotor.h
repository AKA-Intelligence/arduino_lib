#ifndef VEHICLEMOTOR_H
#define VEHICLEMOTOR_H
#include "Lower.h"

#define MOTOR_DEVICE_ID   0x02
#define FORWARD_CMD_CODE  0x61
#define BACKWARD_CMD_CODE   0x62
#define STOP_CMD_CODE     0x63

int mt1_sped = 0;
int mt2_sped = 0;

ISR(TIMER1_COMPB_vect){
    PORTC &= 0xfb;    
    OCR1BL = mt1_sped;
}
ISR(TIMER1_COMPA_vect){
    PORTC &= 0xf7;
    OCR1AL = mt2_sped;
}
ISR(TIMER1_OVF_vect){
    OCR1AL = mt2_sped;
    OCR1BL = mt1_sped;
    if(mt1_sped>10) PORTC |= 0x04;
    if(mt2_sped>10) PORTC |= 0x08;
}


class VehicleMotor: public Lower{
  public :
  VehicleMotor():Lower(){}  // for not user custom
  VehicleMotor(uint8_t user_def_dev_id):Lower(user_def_dev_id){}  // for user custom
  void triggerCMD(uint8_t code, uint8_t* data, uint8_t len);
  void update(); // every time in loop, check modules state
  void init();
  private :
  void foward_callback(uint8_t *cmd_data, uint8_t len);
  void backward_callback(uint8_t *cmd_data, uint8_t len);
  void stop_callback(uint8_t *cmd_data, uint8_t len);
  int motor1_dir_pin;
  int motor2_dir_pin;
  int motor1_pwm_pin;
  int motor2_pwm_pin;
};
void VehicleMotor::init(){
  devid = MOTOR_DEVICE_ID ;
  motor1_dir_pin = getLowerPin(analog4);
  pinMode(motor1_dir_pin , OUTPUT);
  motor2_dir_pin = getLowerPin(analog5);
  pinMode(motor2_dir_pin  , OUTPUT);
  motor1_pwm_pin = getLowerPin(analog2);
  pinMode(motor1_pwm_pin  , OUTPUT);
  motor2_pwm_pin = getLowerPin(analog3);
  pinMode(motor2_pwm_pin  , OUTPUT); 
  
  cli();
  TCCR1A = 0;    
  TCCR1B = 0;
  TCNT1L  = 0;
  
  OCR1AL = 2;
  OCR1BL = 2; 
  
  TCCR1A |= 0x01;
  TCCR1B |= 0x0b;
  TIMSK1 |= 0x07;
  sei();
}
void VehicleMotor::foward_callback(uint8_t *data, uint8_t len){
  if(len != 2) return ; // check data length is 2 byte for direction and speed
  int dir = *(data);
  int speed = *(data+1);
  if(dir == 0) {
    analogWrite(motor1_dir_pin, 256);
        mt1_sped = speed ;
  }
  else if(dir == 1) {
    analogWrite(motor2_dir_pin, 256);
        mt2_sped = speed ;
  }
}
void VehicleMotor::backward_callback(uint8_t *data, uint8_t len){
  if(len != 2) return ; // check data length is 2 byte for direction and speed
  int dir = *(data);
  int speed = *(data+1);
  if(dir == 0) {
    analogWrite(motor1_dir_pin, 0);
        mt1_sped = speed ;
  }
  else if(dir == 1) {
    analogWrite(motor2_dir_pin, 0);
        mt2_sped = speed ;
  }
}
void VehicleMotor::stop_callback(uint8_t *data, uint8_t len){
  if(len != 2) return ; // check data length is 2 byte for direction and speed
  int dir = *(data);
  //int speed = *(data+1);
  if(dir == 0) {
    analogWrite(motor1_dir_pin, 0);
        mt1_sped = 0 ;
  }
  else if(dir == 1) {
    analogWrite(motor2_dir_pin, 0);
        mt2_sped = 0 ;
  }
}
void VehicleMotor::triggerCMD(uint8_t code, uint8_t *data, uint8_t len){
    len--;  // except code length
    if(code == FORWARD_CMD_CODE )       foward_callback(data,len);
    else if(code == BACKWARD_CMD_CODE )  backward_callback(data,len);
    else if(code == STOP_CMD_CODE )  stop_callback(data,len);
}

void VehicleMotor::update(){
}





#endif
