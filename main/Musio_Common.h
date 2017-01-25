#ifndef MUSIO_COMMON_H
#define MUSIO_COMMON_H

#include <Arduino.h>
#if 1
#define SERIAL_RX 6
#define SERIAL_TX 9
#else
#define SERIAL_RX 0
#define SERIAL_TX 1
#endif

#define RXBUF_SIZE 100
#define TXBUF_SIZE 200
#define HOST_STRING "HOST"
#define HOST_STR_LEN 4

#define ACK_STRING "ACK"
#define ACK_STR_LEN 3

#define PACKET_START_CODE 's'
#define PACKET_END_CODE 'e'
#define AKA_FIST_DEV 0x00

///define packet type code
//
#define ASK_DEVINFO_PACKET 0X01
#define DEVINFO_PACKET 0x02
#define CMD_PACKET 0x03
#define RESPONSE_PACKET 0x04

#define PIN_TEST_PACKET 0xFF
#define INVALID_DEV_ID 0x00

typedef struct message{
  uint8_t len_data;
  uint8_t devid;
  uint8_t dev_pos;
  uint8_t code;
  uint8_t* data;
}Message;
typedef enum { analog0, analog1, analog2 , analog3, analog4, analog5, digital1, digital2, digital3, digital4, pwm1, pwm2, _Nbr_pin } pin_t;
typedef struct pin_use{
  int pin;
  uint8_t used;
}Pin_Use;
Pin_Use Pin_List[12] = { {A0,0}, {A1, 0},{A2, 0},{A3, 0},{A4, 0},{A5, 0},{2, 0},{4, 0},{7, 0},{8, 0},{3, 0},{5, 0} } ;

void software_reset(){
  asm volatile ("  jmp 0");  
}
#endif
