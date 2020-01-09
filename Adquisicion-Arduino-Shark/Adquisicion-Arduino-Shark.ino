#include <SoftwareSerial.h>
#include <ModbusRtu.h>


// data array for modbus network sharing
uint16_t au16data[20];
uint8_t u8state;

/**
 *  Modbus object declaration
 *  u8id : node id = 0 for master, = 1..247 for slave
 *  u8serno : serial port (use 0 for Serial)
 *  u8txenpin : 0 for RS-232 and USB-FTDI 
 *               or any pin number > 1 for RS-485
 */
Modbus master(0,1,2); // this is master and RS-232 or USB-FTDI
/**
 * This is an structe which contains a query to an slave device
 */
modbus_t telegram;
unsigned long u32wait;

#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11);

void setup() {
  mySerial.begin(9600);
  while (!Serial);
  master.begin( 9600 ); // baud-rate at 19200
  master.setTimeOut( 2000 ); // if there is no answer in 2000 ms, roll over
  u32wait = millis() + 1000;
  u8state = 0; 
}

void loop() {
  switch( u8state ) {
  case 0: 
    if (millis() > u32wait) u8state++; // wait state
    break;
  case 1: 
    telegram.u8id = 1; // slave address
    telegram.u8fct = 3; // function code (this one is registers read)
    telegram.u16RegAdd = 1005; // start address in slave
    telegram.u16CoilsNo = 20; // number of elements (coils or registers) to read
    telegram.au16reg = au16data; // pointer to a memory array in the Arduino

    master.query( telegram ); // send query (only once)
    u8state++;
    break;
  case 2:
    master.poll(); // check incoming messages
    if (master.getState() == COM_IDLE) {
      u8state = 0;
      u32wait = millis() + 100;
      //voltajes de linea
      uint32_t VolAB = au16data[0]*65536;
      float PVolAB = *(float*)&VolAB;
      String FVolAB=String(int(PVolAB));
      
      uint32_t VolBC = au16data[2]*65536;
      float PVolBC = *(float*)&VolBC;
      String FVolBC=String(int(PVolBC));
      
      uint32_t VolCA = au16data[4]*65536;
      float PVolCA = *(float*)&VolCA;
      String FVolCA=String(int(PVolCA));

      //corrientes
      uint32_t CuA = au16data[6]*65536;
      float PCuA = *(float*)&CuA;
      String FCuA=String(int(PCuA));
      
      uint32_t CuB = au16data[8]*65536;
      float PCuB = *(float*)&CuB;
      String FCuB=String(int(PCuB));
      
      uint32_t CuC = au16data[10]*65536;
      float PCuC = *(float*)&CuC;
      String FCuC=String(int(PCuC));

      // factor de potencia
      uint32_t fp = au16data[18]*65536;
      float Pfp = *(float*)&fp;
      String Ffp=String(int(Pfp));

      // frecuencia
      uint32_t fre = au16data[20]*65536;
      float Pfre = *(float*)&fre;
      String Ffre=String(int(Pfre));
            
      String data= FVolAB+"!"+FVolBC+"#"+FVolCA+"$"+Ffp+"%"+FCuA+"&"+FCuB+"/"+FCuC+"("+Ffre+")"+"0"+"*";
      Serial.println(data);
      
      String aStupidWasteOfResource = "Pissing away resources uselessly";
      char datos[50];
      data.toCharArray(datos, 50);
      Serial.println(datos);
      mySerial.write(datos);
      delay(10000);
      
      
    break;
  }
}
}
