#include "Arduino.h"
#include "pins_arduino.h"
#include "HardwareSerial.h"
#include "Sensor.h"
#include "Shooter.h"
// #include <HCSR04.h>
#include "GerakMotor.h"
#include "Slave.h"
#include "ColorSensor.h"
#include "lifting.h"
#include "Shooter.h"
#include "FungsiObject.h"

// Pin UltraSonic
#define trig A0
#define echo A1

int duration, distance;
float ultraSensor;
int liftTrigger = 1;
int topLiftTrigger = 0;
bool isSendSignalToLift = true;

void sensor(int *mode, int color, int topLift, int lift, int speedShooter, bool *isModeChanged) {

  // Awal2 nyala Semua
  topLifting(liftTrigger, topLift);
  lifting(liftTrigger, lift);
  shooter(liftTrigger, speedShooter);

  // Serial.print(" lift trigger : "); Serial.println(liftTrigger);
  // Serial.print(" Mode : "); Serial.println(*mode);
  // Serial.print(" Color "); Serial.println(color);
  // Serial.print(" isSendLift : "); Serial.println(isSendSignalToLift);

  if (*mode == 2) {
    // Bola Warna Merah
    if (color == 1) {
      delay(250);
      // Mati Lift
      liftTrigger = 0;
      *mode = 3;
      // Selain Merah
    } else {
      liftTrigger = 1;
    }
  } else if (*mode == 4) {
    analogWrite(trig, 0);
    delayMicroseconds(2);
    analogWrite(trig, 1023);
    delayMicroseconds(10);
    analogWrite(trig, 0);

    duration = pulseIn(echo, HIGH, 5000);


    // Serial.print("duration: "); Serial.println(duration);
    duration = duration == 0 ? 5000 : duration;

    distance = duration * 0.034 / 2;
    // Serial.print("distance: "); Serial.println(distance);

    if (distance <= 20) {

      liftTrigger = 1;
      *isModeChanged = !*isModeChanged;
      *mode = 2;
    } else {
      liftTrigger = 0;
    }
  }
}
