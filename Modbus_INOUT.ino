/**
 *  Modbus slave example 2:
 *  The purpose of this example is to link the Arduino digital and analog
 *	pins to an external device.
 *
 *  Recommended Modbus Master: QModbus
 *  http://qmodbus.sourceforge.net/
 */
#include "fastio.h"
#include <ModbusRtu.h>
#define ID   1
#define OUT1 2
#define OUT2 3
#define OUT3 4
#define OUT4 5
#define IN1 6
#define IN2 7
#define IN3 8
#define IN4 9
#define IN5 10
#define IN6 11
#define IN7 12
#define IN8 13
Modbus slave(ID, 0, 0); 
//// this is slave ID and RS-232 or USB-FTDI
//boolean led;
int8_t state = 0;
//unsigned long tempus;
  byte diachi=0b00000000;
long _time = 0;
long debounceDelay = 100; 
uint16_t au16data[1];
long lastDebounceTime[4] = {0,0,0,0};
boolean _status[4]={0,0,0,0};
/**
 *  Setup procedure
 */
void setup() {
  io_setup(); // I/O settings
  slave.setID(diachi);
  slave.begin( 9600 );
 // tempus = millis();
}


void loop() {
  state = slave.poll( au16data, 1 );
  io_poll();
} 

/**
2,3,4,5 -> Chân IN 
6,7,8,9 Button DK ngõ OUT
10,11,12,13 Output 
 */
void io_setup() {
  // define i/o
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  SET_OUTPUT(OUT1);
  SET_OUTPUT(OUT2);
  SET_OUTPUT(OUT3);
  SET_OUTPUT(OUT4);
  SET_INPUT(IN1);
  SET_INPUT(IN2);
  SET_INPUT(IN3);
  SET_INPUT(IN4);
  SET_INPUT(IN5);
  SET_INPUT(IN6);
  SET_INPUT(IN7);
  SET_INPUT(IN8);
  digitalWrite( A3, 0);
  diachi |= ((digitalRead(A0)>0)?1:0)<<3 ; 
  diachi |= ((digitalRead(A1)>0)?1:0)<<2 ; 
  diachi |= ((digitalRead(A2)>0)?1:0)<<1 ; 
  diachi |= ((analogRead(A3)>800)?1:0)<<0 ; 
}

void get_button(byte vitri){
  _time=millis();
  if ( ( _time - lastDebounceTime[vitri]) > debounceDelay) {
    boolean pin_value=1;
    switch (vitri){      
      case 0: pin_value=READ( IN1 );break;
      case 1: pin_value=READ( IN2 );break;
      case 2: pin_value=READ( IN3 );break;
      case 3: pin_value=READ( IN4 );break;
    }           
      if (pin_value == 0) {
      if (_status[vitri]){
      _status[vitri]=false;
      boolean _bit = (bitRead( au16data[0], vitri+4 )==0)?1:0 ;
      bitWrite( au16data[0], vitri+4, _bit );
      lastDebounceTime[vitri] = millis(); 
      }
    }
    else if (_status[vitri]==false) {_status[vitri]=true;lastDebounceTime[vitri] = millis(); }
  }
}

void io_poll() {
  bitWrite( au16data[0], 0, READ( IN5 ));
  bitWrite( au16data[0], 1, READ( IN6 ));
  bitWrite( au16data[0], 2, READ( IN7 ));
  bitWrite( au16data[0], 3, READ( IN8 ));
  /////Get button for output control
  get_button(0);
  get_button(1);
  get_button(2);
  get_button(3);
 // Doc Coil Xuat ngo ra WRITE(LED_PIN,LOW);
  WRITE( OUT1, bitRead( au16data[0], 4 ));
  WRITE( OUT2, bitRead( au16data[0], 5 ));
  WRITE( OUT3, bitRead( au16data[0], 6 ));
  WRITE( OUT4, bitRead( au16data[0], 7 ));
}
