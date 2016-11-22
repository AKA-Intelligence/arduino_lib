#ifndef TESTLOWER_H
#define TESTLOWER_H


#include "Lower.h"
#include "SerialPortManager.h"
#define TEST_LOWER_DEVID 0X0f
#define ASK_CMD 0X61
class testLower : public Lower{
  public :
  testLower():Lower(){serial = SerialPortManager::getInstance();}
  void triggerCMD(uint8_t code, uint8_t* data, uint8_t len);
  void update(); // every time in loop, check modules state
  void init();
  private :
  void responseA(uint8_t *data, uint8_t len);
  int input;
  void analog_input_check(){}
  SerialPortManager *serial;
};

void testLower::update(){
  analog_input_check();
}
void testLower::triggerCMD(uint8_t code, uint8_t* data, uint8_t len){
  
  if(code == ASK_CMD)    responseA(data, len);
}

void testLower::responseA(uint8_t *data, uint8_t len){
  
  int index = 8 ;
  for(int j =0;j<len-1;j++){
    if(*(data+j) != 0x05 ) return;
  }
    
    
  uint8_t * value = (uint8_t*)malloc(sizeof(uint8_t)*index);
  for(int i=0;i<index;i++){
    *(value+i) = analogRead(A5);
  }
  
  Message msg = { index+1, devid, dev_pos, ASK_CMD, value };
  serial->writeResponsePacket(&msg);
}
void testLower::init(){
  devid = TEST_LOWER_DEVID;
  input = getLowerPin(analog1);
  pinMode(input, INPUT);
}
#endif
