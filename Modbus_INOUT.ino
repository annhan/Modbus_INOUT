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

Modbus slave(ID, 0, 0); // this is slave ID and RS-232 or USB-FTDI
boolean led;
int8_t state = 0;
unsigned long tempus;

// data array for modbus network sharing
uint16_t au16data[9];
boolean status1=0;
boolean status2=0;
boolean status3=0;
boolean status4=0;
/**
 *  Setup procedure
 */
void setup() {
  io_setup(); // I/O settings
  slave.begin( 115200 );
  tempus = millis();
}

/**
 *  Loop procedure
 */
void loop() {
  state = slave.poll( au16data, 9 );
  io_poll();
} 

/**
 * pin maping:
 * 2 - digital input
 * 3 - digital input
 * 4 - digital input
 * 5 - digital input
 * 6 - digital output
 * 7 - digital output
 * 8 - digital output
 * 9 - digital output
 * 10 - analog output
 * 11 - analog output
 * 14 - analog input
 * 15 - analog input
 *
 * pin 13 is reserved to show a successful query
 */
void io_setup() {
  // define i/o
  pinMode(2, INPUT_PULLUP );
  pinMode(3, INPUT_PULLUP );
  pinMode(4, INPUT_PULLUP );
  pinMode(5, INPUT_PULLUP );
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
}

/**
 *  Link between the Arduino pins and the Modbus array
 */
void io_poll() {
  // get digital inputs -> au16data[0]
  bitWrite( au16data[0], 0, digitalRead( 2 ));
  bitWrite( au16data[0], 1, digitalRead( 3 ));
  bitWrite( au16data[0], 2, digitalRead( 4 ));
  bitWrite( au16data[0], 3, digitalRead( 5 ));
  
  if (digitalRead( 6 )==0 ){
    if (status1){
    status1=false;
    boolean _bit = (bitRead( au16data[0], 4 )==0)?1:0 ;
    bitWrite( au16data[0], 4, _bit );
   // byte trave=bitRead( au16data[0], 4 );
    //if (trave>0)bitWrite( au16data[0], 4, 0);
    //else bitWrite( au16data[0], 4, 1);
    }
  }
  else if (status1==false) status1=true;
  
  if (digitalRead( 7 )==0 ){
    if (status2){
    status2=false;
    byte trave=bitRead( au16data[0], 5 );
    if (trave>0)bitWrite( au16data[0], 5, 0);
    else bitWrite( au16data[0], 5, 1);
    }
  }
  else if (status2==false) status2=true;
  
  if (digitalRead( 8 )==0 ){
    if (status3){
    status3=false;
    byte trave=bitRead( au16data[0], 6 );
    if (trave>0)bitWrite( au16data[0], 6, 0);
    else bitWrite( au16data[0], 6, 1);
    }
  }
  else if (status3==false) status3=true;
  
  if (digitalRead( 9 )==0 ){
    if (status4){
    status4=false;
   // byte trave=bitRead( au16data[0], 7 );
    //gui = (bitRead( au16data[0], 7 )==0)?1:0 ;
    boolean _bit = (bitRead( au16data[0], 7 )==0)?1:0 ;
    bitWrite( au16data[0], 7, _bit );
 // gui1 = (digitalRead(IN2)==0)?0:1 ;
  //gui2 = (digitalRead(IN3)==0)?0:1 ;  
   // if (trave>0)bitWrite( au16data[0], 7, 0);
   // else bitWrite( au16data[0], 7, 1);
    }
  }
  else if (status4==false) status4=true;
 // Doc Coil Xuat ngo ra
  digitalWrite( 13, bitRead( au16data[0], 4 ));
  digitalWrite( 11, bitRead( au16data[0], 5 ));
  digitalWrite( 12, bitRead( au16data[0], 6 ));
  digitalWrite( 10, bitRead( au16data[0], 7 ));
  
  au16data[7] = slave.getInCnt();
  au16data[8] = slave.getOutCnt();
  au16data[9] = slave.getErrCnt();
}
