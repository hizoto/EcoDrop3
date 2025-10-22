#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include "sensors.h"

Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();

#define SHUT_Pin 22

void tof_setup() {
  Serial.begin(115200);
  Wire.begin();

  pinMode(SHUT_Pin, OUTPUT);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  
  Serial.println("TOF_Sensor_Test");
  if (!lox1.begin()) {
    Serial.println(F("Keine Vrbindung Sensor vorne"));
    while(1);
  }
  if (!lox2.begin()) {
    Serial.println(F("Keine Verbindung Sensor hinten"));
    while(1);
  }
  //Ausschalten des Sensors vorne um Adressänderung des hinteren Sensors zu machen
  digitalWrite(SHUT_Pin, LOW);

  //Adressänderung des hinteren Sensors auf 0x30
  if(!lox1.begin(0x30)) {
    Serial.println(F("Failed to boot first VL53L0X"));
    while(1);
  }
  delay(100);

  //Einschalten des vorderen Sensors
  digitalWrite(SHUT_Pin, HIGH);

}


void tof_loop() {
 // VL53L0X_RangingMeasurementData_t measure;
    
//  Serial.print("Reading a measurement... ");
 // lox.rangingTest(&measure, true); // pass in 'true' to get debug data printout!

 // if (measure.RangeStatus != 4) {  // phase failures have incorrect data
 //   Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
 // } else {
 //   Serial.println(" out of range ");
 // }
    
 // delay(2000);
  
   byte error, address;
  int nDevices = 0;
  for(address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("Found I2C device at 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");
      nDevices++;
    } else if (error==4) {
      Serial.print("Unknown error at 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0) Serial.println("No I2C devices found");
  else Serial.println("done");
  delay(3000);
}

/*
#include "Adafruit_VL53L0X.h"

// address we will assign if dual sensor is present
#define LOX1_ADDRESS 0x29
#define LOX2_ADDRESS 0x31

// set the pins to shutdown
#define SHT_LOX1 22
#define SHT_LOX2 23

// objects for the vl53l0x
Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();

// this holds the measurement
VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;



void setID() {
  // all reset
  digitalWrite(SHT_LOX1, LOW);    
  digitalWrite(SHT_LOX2, LOW);
  delay(100);
  // all unreset
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, HIGH);
  delay(100);

  // activating LOX1 and resetting LOX2
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, LOW);

  // initing LOX1
  if(!lox1.begin(LOX1_ADDRESS)) {
    Serial.println(F("Failed to boot first VL53L0X"));
    while(1);
  }
  delay(100);
/*
  // activating LOX2
  digitalWrite(SHT_LOX2, HIGH);
  delay(100);

  //initing LOX2
  if(!lox2.begin(LOX2_ADDRESS)) {
    Serial.println(F("Failed to boot second VL53L0X"));
    while(1);
  }
}


void setup() {
  Serial.begin(115200);
  Wire.begin();

  // wait until serial port opens for native USB devices
  while (! Serial) { delay(1); }

  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);

  Serial.println(F("Shutdown pins inited..."));

  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);

  Serial.println(F("Both in reset mode...(pins are low)"));
  
  
  Serial.println(F("Starting..."));
  setID();
 
}

void loop() {
   
}

#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.begin(); // SDA=20, SCL=21 auf Mega
  delay(200);
  Serial.println("I2C Scanner start");
}

void loop() {
  byte error, address;
  int nDevices = 0;
  for(address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("Found I2C device at 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");
      nDevices++;
    } else if (error==4) {
      Serial.print("Unknown error at 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0) Serial.println("No I2C devices found");
  else Serial.println("done");
  delay(3000);
}
*/