#ifndef TIMER_CTR_H
#define TIMER_CTR_H

#define TIMER_WGM_NORMAL    0X1
#define TIMER_WGM_PWM       0X2
#define TIMER_WGM_FAST_PWM  0X3

#define NO_PRESCALING 0X1
#define PRESCAL_8     0X2
#define PRESCAL_32    0X3
#define PRESCAL_64    0X4
#define PRESCAL_128   0X5
#define PRESCAL_256   0X6
#define PRESCAL_1024  0X7
/*
 * hand device timer : timer 2
 * lower device timer: timer 1
*/
typedef void (*handler_callback)();
typedef enum { _timer1, _timer2, Nbr_timer_size } timer_index_t;
int timer_arr[2] = { 0, 0 };
// TIMER2 INTERRUPT HANDLER FUNCTION POINTER
void (*TIMER2_COMPB_handler)();
void (*TIMER2_COMPA_handler)();
void (*TIMER2_OVF_handler)();
/*
// TIMER1 INTERRUPT HANDLER FUNCTION POINTER
void (*TIMER1_CAPT_handler)();
void (*TIMER1_COMPB_handler)();
void (*TIMER1_COMPA_handler)();
void (*TIMER1_OVF_handler)();

ISR(TIMER1_CAPT_vect){//timer1-capture interrupt service routine 
    TIMER1_CAPT_handler();
}
ISR(TIMER1_COMPA_vect){//timer1-channel A interrupt service routine 
    TIMER1_COMPB_handler();
}
ISR(TIMER1_COMPB_vect){//timer1-channel A interrupt service routine 
    TIMER1_COMPA_handler();
}
ISR(TIMER1_OVF_vect){//overflow for timmer 1,
    TIMER1_OVF_handler();
}
void setTimer1Handlers(handler_callback ovf_cb,handler_callback A_cb,handler_callback B_cb){
  TIMER1_OVF_handler = ovf_cb;
  TIMER1_COMPA_handler = A_cb;
  TIMER1_COMPB_handler = B_cb;
}*/


ISR(TIMER2_COMPB_vect){//timer2-channel B interrupt service routine 
    TIMER2_COMPB_handler();
}
ISR(TIMER2_COMPA_vect){//timer2-channel A interrupt service routine 
    TIMER2_COMPA_handler();
}
ISR(TIMER2_OVF_vect){//overflow for timmer 2,
    TIMER2_OVF_handler();
}
/*
int getFreeTimer(){
  for(int i=0;i<Nbr_timer_size;i++)
    if(timer_arr[i] == 0) return i;
  return -1;
}
void attachTimer(int timer){
  timer_arr[timer]=1;
}
void setTCCR1A(uint8_t TCCRnA){
  if(timer == 0) TCCR1A = TCCRnA;
  else if(timer == 1) TCCR2A = TCCRnA;
}*/
void setTimer2OutPutComaperA(int value){
  OCR2A = value;
}
void setTimer2OutPutComaperB(uint8_t value){
  OCR2B = value;
}
void setTimer2Config(uint8_t WGM, uint8_t CS){
    TCCR2A = WGM; //fast pwm
    TCCR2B = CS; //256 prescaler
    TCNT2  = 0;//initialize counter value to 0 - this register is counting register, timmer count something and save to here
    OCR2A = 2;
    OCR2B = 2;
}
void setTimer2Handlers(handler_callback ovf_cb,handler_callback A_cb,handler_callback B_cb){
  cli();//stop interrupts
  TIMSK2 = 0;
  if(ovf_cb != NULL){
    TIMER2_OVF_handler = ovf_cb;
    TIMSK2 |= 0x1;
  }
  if(A_cb != NULL){
    TIMER2_COMPA_handler = A_cb;
    TIMSK2 |= 0x2;
  }
  if(B_cb != NULL){
    TIMER2_COMPB_handler = B_cb;
    TIMSK2 |= 0x4;
  }
  sei();//allow general interrupts in general control register
}


#endif
