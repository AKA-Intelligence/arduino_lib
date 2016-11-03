#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H
#include <SoftwareSerial.h>
#include <Arduino.h>
#include "Musio_Common.h"
#include "QueueArray.h"
#include <inttypes.h>


SoftwareSerial musio_serial(SERIAL_RX, SERIAL_TX);
class SerialPortManager{
  public :
  Message* fetchMSG();
  void writePacket(char* str);
  void readPacket();
  static SerialPortManager* getInstance();
  boolean check_connection();
  boolean handShake(int8_t devid_L, int8_t devid_R, int8_t use_case);
  int restart_flag=0;
  int pin_test_flag =0;
  private :
  void parsePacket(int packet_end, int index);
  boolean waitSerialString(char* str, int len); 
  void flushBuffer();
  char rxbuf[RXBUF_SIZE]={};
  int rxbuf_index=0;
  QueueArray<Message> msgQueue;
  SerialPortManager(){
    musio_serial.begin(9600);
  }
};
void SerialPortManager::writePacket(char* str){
  musio_serial.print(str);
}


 Message* SerialPortManager::fetchMSG(){
    Message* msg = msgQueue.pop();
    return msg;
  }
  
  
void SerialPortManager::readPacket(){
    int i=0;
    while(musio_serial.available() > 0 && (i < RXBUF_SIZE)){            
      rxbuf[i++] = musio_serial.read();
      delay(2);
    }
    flushBuffer();
    if(i > 0) 
      parsePacket(i,0);
}




void SerialPortManager::parsePacket(int packet_end, int index){
    uint8_t len = 0;
    uint8_t packet_type ;   
    //Serial.write(rxbuf[index]);
      if(rxbuf[index++]==PACKET_START_CODE){
        // TYPE
        // HANDSHAKE : 0X01, CMD : 0X02, DATA : 0X03, RESTART : 0x04
        packet_type = rxbuf[index++];
        
        if(packet_type == RESTART_PACKET && rxbuf[index] == PACKET_END_CODE) { //give up remain packets
          restart_flag = 1;
          msgQueue.clear();
          return ;
        }
        else if(packet_type == PIN_TEST_PACKET && rxbuf[index] == PACKET_END_CODE) { //give up remain packets
          pin_test_flag = 1;
          return ;
        }
        else{     
          if(packet_type != CMD_PACKET)  {
            parsePacket(packet_end, index);  // search again
            return;
          }
          // LEN : CMD + DATA LENGTH >= 2, <= 8 BYTES
          
          len = rxbuf[index++];
          Message msg = {};
          msg.LRvalue = rxbuf[index++];
          msg.devid = rxbuf[index++];  
          msg.code = rxbuf[index++];
          len--;
          msg.data = malloc(sizeof(char)*(len));
          for(int l=0;l<len;l++)
            msg.data[l] = rxbuf[index++];
          
          if(rxbuf[index++] == PACKET_END_CODE) {
            //Serial.print("push");
            msgQueue.push(msg);  
          }
          if(index + 2 < packet_end) parsePacket(packet_end, index);
        }
      }else{
        parsePacket(packet_end, index);     
      }
    
    return;
}
  

static SerialPortManager* SerialPortManager::getInstance(){
    static SerialPortManager serialPort;
    return &serialPort;
}

boolean SerialPortManager::check_connection(){
    boolean result = false;   
    int i=0;   
    //musio_serial.write("###");
    delay(200); 
    musio_serial.print("###");
    
    //while(musio_serial.available()){                
    return waitSerialString(HOST_STRING,HOST_STR_LEN);
}
  
boolean SerialPortManager::handShake(int8_t devid_L, int8_t devid_R, int8_t use_case){
    boolean result = true;
    char hs_packet[6] = {PACKET_START_CODE,DEVINFO_PACKET,0,0,0,PACKET_END_CODE};
    hs_packet[2]= use_case;
    hs_packet[3]= devid_L;
    hs_packet[4]= devid_R;
 
    musio_serial.write(hs_packet,6);
    delay(200);              
    return waitSerialString(ACK_STRING,ACK_STR_LEN);
}

void SerialPortManager::flushBuffer(){
    while(musio_serial.available()){
      musio_serial.read();
    }
}

boolean SerialPortManager::waitSerialString(char* str, int len){
    boolean result = false;
    int i=0;
    while(musio_serial.available() && i<len){            
      char c = musio_serial.read();
      if(c > 0) rxbuf[i++] = c;
    }
    flushBuffer();
    
    for(i=0;i<len;i++) {
      if(rxbuf[i] != str[i]){
        result = false;
        break;
      }
    }
   if(i == len) result = true; 
   return result;
}
  
#endif
