#ifndef MUSIO_ARD_H
#define MUSIO_ARD_H
#include "SerialPortManager.h"
#include "Hand.h"
#include "Device.h"
#define MAX_DEV_NUM 5

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
  void sendDevinfo();
  void triggerCMD(Message* msg);
  void init_devices();
  void add_Device(Device* dev);
private:
  int use_case;
  Device* devList[MAX_DEV_NUM];
  int dev_num;
  SerialPortManager* serial;
  boolean invalid_pin_config = 0;
  void update_devices();
  void pin_test();
};

void Musio_Ard::init_devices(){
  //init devices configuration
  //add pin use list
  for(int i=0;i<dev_num;i++)   devList[i]->init(); 

  for(int i=0;i<_Nbr_pin;i++)   
    if(Pin_List[i].used > 1) invalid_pin_config = 1;
}
void Musio_Ard::update_devices(){
  for(int i=0;i<dev_num;i++)   devList[i]->update(); 
}

void Musio_Ard::add_Device(Device* dev){
  if(dev == NULL || dev_num >= MAX_DEV_NUM) return;
  devList[dev_num++] = dev;
}

void Musio_Ard::pin_test(){
  toggleAnalog(A0);
  toggleDigital(4);
  toggleAnalog(A1);
  toggleDigital(8);
  toggleAnalog(A2);
  toggleAnalog(A4);

  toggleDigital(2);
  toggleDigital(3);
  toggleDigital(7);
  toggleDigital(5);
  toggleAnalog(A3);
  toggleAnalog(A5);
  
  serial->pin_test_flag = 0;
  init_devices();
}
  void Musio_Ard::update(){
    int pos = -1;
    delay(10);  //wait for prepare to receive data from ap
    serial->readPacket();
    if(serial->send_devinfo_flag == 1) sendDevinfo();  //restart at device info
    if(serial->pin_test_flag == 1) pin_test();

    Message *msg = serial->fetchMSG();
    if(msg) {
      if(invalid_pin_config) {sendDevinfo();}
      else {
        triggerCMD(msg);
        free(msg->data);
      }
    } 
    update_devices();
  }
  
  Musio_Ard::Musio_Ard(){
    serial = SerialPortManager::getInstance();  
    dev_num = 0;
  }
  void Musio_Ard::sendDevinfo(){
    int len = dev_num * 2;
    uint8_t* devinfos = (uint8_t*)malloc( sizeof(uint8_t) * len);
    if(invalid_pin_config){
      serial->sendDevinfoPacket(devinfos, 0);
    }
    else{
      for(int i=0;i<dev_num;i++){
        devinfos[i*2] = devList[i]->getDevid();
        devinfos[(i*2)+1] = devList[i]->getDevPos();
      }    
      serial->sendDevinfoPacket(devinfos,dev_num);
    }
  }
  
  void Musio_Ard::triggerCMD(Message* msg){
    uint8_t len = msg->len_data;
    uint8_t devid = msg->devid;
    uint8_t dev_pos = msg->dev_pos;
    uint8_t code = msg->code;
    uint8_t* data = msg->data;
    
    for(int i=0;i<dev_num;i++){
      if((devList[i]->getDevid() == devid) && (devList[i]->getDevPos() == dev_pos))       devList[i]->triggerCMD(code, data, len);  
    }
  }
  
 

#endif
