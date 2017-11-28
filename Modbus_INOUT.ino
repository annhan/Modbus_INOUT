/**
 *  Modbus slave example 2:
 *  The purpose of this example is to link the Arduino digital and analog
 *	pins to an external device.
 *
 *  Recommended Modbus Master: QModbus
 *  http://qmodbus.sourceforge.net/
 */

#include <ModbusRtu.h>
#define ID   1
Modbus slave(ID, 0, 0); 
//// this is slave ID and RS-232 or USB-FTDI
//boolean led;
int8_t state = 0;
//unsigned long tempus;
  byte diachi=0b00000000;
long _time = 0;
long debounceDelay = 100; 
uint16_t au16data[10];
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
  state = slave.poll( au16data, 10 );
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
  
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, INPUT_PULLUP); // Button ->OUT PIN10
  pinMode(7, INPUT_PULLUP); // Button ->OUT PIN11
  pinMode(8, INPUT_PULLUP); // Button ->OUT PIN12
  pinMode(9, INPUT_PULLUP); // Button ->OUT PIN13
  pinMode(10, INPUT_PULLUP );
  pinMode(11, INPUT_PULLUP );
  pinMode(12, INPUT_PULLUP );
  pinMode(13, INPUT );

  diachi |= ((digitalRead(A0)>0)?1:0)<<3 ; 
  diachi |= ((digitalRead(A1)>0)?1:0)<<2 ; 
  diachi |= ((digitalRead(A2)>0)?1:0)<<1 ; 
  diachi |= ((digitalRead(A3)>0)?1:0)<<0 ; 
}

void get_button(byte pin, byte vitri){
  _time=millis();
  if ( ( _time - lastDebounceTime[vitri]) > debounceDelay) {
    if (digitalRead(pin) == LOW) {
      if (_status[vitri]){
      _status[vitri]=false;
      // 4,5,6,7 là bit nhan button
      boolean _bit = (bitRead( au16data[0], vitri+4 )==0)?1:0 ;
      bitWrite( au16data[0], vitri+4, _bit );
      lastDebounceTime[vitri] = millis(); 
      }
    }
    else if (_status[vitri]==false) {_status[vitri]=true;lastDebounceTime[vitri] = millis(); }
  }
}

/**
 *  Link between the Arduino pins and the Modbus array
 */

void io_poll() {
  // get digital inputs -> au16data[0]
  bitWrite( au16data[0], 0, digitalRead( 10 ));
  bitWrite( au16data[0], 1, digitalRead( 11 ));
  bitWrite( au16data[0], 2, digitalRead( 12 ));
  bitWrite( au16data[0], 3, digitalRead( 13 ));
  /////Get button for output control
  get_button(6,0);
  get_button(7,1);
  get_button(8,2);
  get_button(9,3);
 // Doc Coil Xuat ngo ra
  digitalWrite( 2, bitRead( au16data[0], 4 ));
  digitalWrite( 3, bitRead( au16data[0], 5 ));
  digitalWrite( 4, bitRead( au16data[0], 6 ));
  digitalWrite( 5, bitRead( au16data[0], 7 ));
  ///////////////////////////////////
  au16data[7] = slave.getInCnt();
  au16data[8] = slave.getOutCnt();
  au16data[9] = slave.getErrCnt();
}
