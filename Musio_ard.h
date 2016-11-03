#ifndef MUSIO_ARD_H
#define MUSIO_ARD_H
#include "SerialPortManager.h"
#include "Hand.h"

void toggleAnalog(int pin){
  pinMode(pin, OUTPUT);
  analogWrite(pin, 256);
  delay(100);
  analogWrite(pin, 0);
  delay(50);
}
void toggleDigital(int pin){
  pinMode(pin, OUTPUT);
  digitalWrite(pin,HIGH);
  delay(100);
  digitalWrite(pin, LOW);
  delay(50);
}

class Musio_Ard {
  public :
  void update();
  Musio_Ard();
  void set_Device(Hand* l, Hand* r);
  boolean connectMusio();
  boolean checkDevice();
  void triggerCMD(Message* msg);
private:
  SerialPortManager* serial;
  Hand* left;
  Hand* right;
  boolean is_connected ;
  boolean is_handshaked ;
  void pin_test();
};

void Musio_Ard::pin_test(){
  serial->writePacket("TEST START");
  toggleAnalog(A0);
  toggleDigital(4);
  toggleAnalog(A1);
  toggleDigital(8);
  toggleAnalog(A2);
  toggleAnalog(A4);

 // lower pin connector test
  toggleDigital(2);
  toggleDigital(3);
  toggleDigital(7);
  toggleDigital(5);
  toggleAnalog(A3);
  toggleAnalog(A5);
  serial->writePacket("TEST END");
  serial->pin_test_flag = 0;
  left->init();
  right->init();
}
  void Musio_Ard::update(){
    int pos = -1;
   // Serial.print('l');
    if(left != NULL) left->update();
    //Serial.print('r');
    if(right != NULL) right->update();

    delay(10);
    serial->readPacket();

    if(serial->restart_flag == 1) {
      is_handshaked = 0;
      checkDevice();  //restart at device info
    }
    if(serial->pin_test_flag == 1){
      pin_test();
    }
    Message *msg = serial->fetchMSG();
    if(msg) {
     // Serial.print("pop");
      triggerCMD(msg);
      free(msg->data);
    } 
  }
  Musio_Ard::Musio_Ard(){
    serial = SerialPortManager::getInstance();  
    is_connected = false;
    is_handshaked = false;
  }
  void Musio_Ard::set_Device(Hand* l, Hand* r){
    left = l;
    right = r;
  }
  boolean Musio_Ard::connectMusio(){    
    while(!serial->check_connection()){}
    is_connected = true;
    return is_connected;
  }
  boolean Musio_Ard::checkDevice(){
    int8_t left_dev_id = 0;
    int8_t right_dev_id = 0;
    int8_t use_case = 1;
    if(left != NULL)  {
      left_dev_id = left->getDevid();
      use_case &= left->getUseCase();
    }
    if(right != NULL) {
      right_dev_id = right->getDevid();
      use_case &= right->getUseCase();
    }
    while(!serial->handShake(left_dev_id,right_dev_id, use_case)){}
    serial->restart_flag = 0;
    is_handshaked =true;
    return is_handshaked;
  }
  void Musio_Ard::triggerCMD(Message* msg){
    uint8_t devid = msg->devid;
    uint8_t LRvalue = msg->LRvalue;
    uint8_t code = msg->code;
    uint8_t* data = msg->data;
   
    if(LRvalue == 0x00 && devid == left->getDevid()) {
      left->triggerCMD(code,data);
    }
    if(LRvalue == 0x01 && devid == right->getDevid()) {
      right->triggerCMD(code,data);
    }
  }
  
 

#endif
