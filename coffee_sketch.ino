// this example is public domain. enjoy!
// www.ladyada.net/learn/sensors/thermocouple

#include <max6675.h>
#include <ModbusRtu.h>

// data array for modbus network sharing

word au16data[16] = {
  3, 1415, 9265, 4, 2, 7182, 28182, 8, 0, 0, 0, 0, 0, 0, 1, -1
};

/**
    Modbus object declaration
    u8id : node id = 0 for master, = 1..247 for slave
    u8serno : serial port (use 0 for Serial)
    u8txenpin : 0 for RS-232 and USB-FTDI
                or any pin number > 1 for RS-485
*/
Modbus slave(1, Serial, 0); // this is slave @1 and RS-232 or USB-FTDI

int thermoDO = 7;
int thermoCS = 8;
int thermoCLK = 9;

MAX6675 thermocouple(thermoDO, thermoCS, thermoCLK);

int Burner = 3;


void setup() {
  Serial.begin( 115200 ); // 115200 baud, 8-bits, none, 1-bit stop
  slave.start();
  // use Arduino pins
  pinMode(Burner, OUTPUT);
  au16data[2] = 0;
  delay(500);
}

void loop() {
  au16data[2] = (uint16_t) (((thermocouple.readCelsius() / 1.7448) + 4.8) * 100); // Read Temp.sensor, scale value and pass to Modbus register.
  slave.poll( au16data, 16 ); // Read modbus registers.

  for (int i = 1; i <= 99; i++) {
    if (i <= au16data[4]) {
      digitalWrite(Burner, HIGH);  // Burner on. With value x in au16data[4] burner will be on x % on and (100-x) % off each half second
     
    }
    else {
      digitalWrite(Burner, LOW); // Burner off
     
    }
    delay(2);
  }
}
