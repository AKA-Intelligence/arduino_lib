#ifndef MUSIO_COMMON_H
#define MUSIO_COMMON_H

#define SERIAL_RX 6
#define SERIAL_TX 9

#define LEFT_DIG1 2
#define LEFT_DIG2 4
#define LEFT_AIN A0
#define LEFT_PWM 3

#define RIGHT_DIG1 7
#define RIGHT_DIG2 8
#define RIGHT_AIN A1
#define RIGHT_PWM 5

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
//#define DEVINFO_PACKET 'a'
#define DEVINFO_PACKET 0x02
#define CMD_PACKET 0x03
//#define CMD_PACKET 0x20
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

#endif
