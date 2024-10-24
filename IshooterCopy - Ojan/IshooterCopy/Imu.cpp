#include "HardwareSerial.h"
#include "Arduino.h"
#include "Imu.h"
#include <Wire.h>
#define addr 0x0D

bool first = true;
int setPointDegree = 75;
int x, y;
int imuDegree(bool *isIMUOn) {
  if (first) {
    Wire.begin();
    Wire.beginTransmission(addr);  //start talking
    Wire.write(0x0B);              // Tell the HMC5883 to Continuously Measure
    Wire.write(0x01);              // Set the Register
    Wire.endTransmission();
    Wire.beginTransmission(addr);  //start talking
    Wire.write(0x09);              // Tell the HMC5883 to Continuously Measure
    Wire.write(0x1D);              // Set the Register
    Wire.endTransmission();

    Wire.beginTransmission(addr);
    Wire.write(0x00);  //start with register 3.
    Wire.endTransmission();

    // Wire.requestFrom(addr, 6);
    // if (6 <= Wire.available()) {
    //   x = Wire.read();        //MSB  x
    //   x |= Wire.read() << 8;  //LSB  x
    //   y = Wire.read();        //MSB y
    //   y |= Wire.read() << 8;  //LSB y
    // }


    // float degree = atan2(y, x) * 180 / PI;
    // if (degree < 0) {
    //   degree += 360;
    // }

    // setPointDegree = degree;
    first = false;
  }

  Wire.beginTransmission(addr);
  Wire.write(0x00);  //start with register 3. 
  Wire.endTransmission();

  Wire.requestFrom(addr, 6);
  if (6 <= Wire.available()) {
    x = Wire.read();        //MSB  x
    x |= Wire.read() << 8;  //LSB  x
    y = Wire.read();        //MSB y
    y |= Wire.read() << 8;  //LSB y
  }


  float degree = atan2(y, x) * 180 / PI;
  if (degree < 0) {
    degree += 360;
  }

  int error = normalizeDegree(setPointDegree - degree);
  if (error > 180) {
    error -= 360;
  }

  if (error < -2 || error > 2) {
    *isIMUOn = true;
  } else {
    *isIMUOn = false;
  }

  // Serial.print("SetPoint : ");
  // Serial.println(setPointDegree);
  // Serial.print("sudut : ");
  // Serial.println(degree);

  return error;
}

int normalizeDegree(int errDegree) {
  if (errDegree < 0) {
    errDegree += 360;
  }
  if (errDegree >= 360) {
    errDegree -= 360;
  }
  return errDegree;
}