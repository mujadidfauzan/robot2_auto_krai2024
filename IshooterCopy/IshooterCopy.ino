#include "Slave.h"
#include "GerakMotor.h"
#include "FungsiObject.h"
#include "Sensor.h"
#include "ColorSensor.h"
#include "Slave.h"
#include "lifting.h"
#include "Shooter.h"
#include "Imu.h"

// Pin Motor
#define R_PWM 5
#define L_PWM 6
// Pin Rs485
#define enPin 4
// Pin Sensor Warna s2 bisa diganti LOW
// #define s2 A0
#define s3 10
#define out 9
// Pin UltraSonic
#define trig A0
#define echo A1

char read;
int mode = 4;
bool isModeChanged = false;
// Object Detection
int detect_object;

#include <Arduino_FreeRTOS.h>

// ======== P I N =======
int baseSpeed = 60;
int topLift = 200;
int lift = 200;
int speedShooter = 80;
int m, n, s;
int color = 0;
int degree;
bool isIMUOn = false;
// ======================================

// define two tasks for Blink & AnalogRead
void TaskCom(void *pvParameters);
void TaskColor(void *pvParameters);
void TaskSensor(void *pvParameters);
void TaskIMU(void *pvParameters);

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  // pinMode(echo, INPUT);
  // pinMode(out, INPUT);
  // pinMode(s3, OUTPUT);
  pinMode(enPin, OUTPUT);
  pinMode(R_PWM, OUTPUT);
  pinMode(L_PWM, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, INPUT);

  digitalWrite(enPin, LOW);

  while (!Serial) {
    ;
  }

  xTaskCreate(
    TaskCom, "Comm"  // A name just for humans
    ,
    128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,
    NULL, 1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,
    NULL);

  xTaskCreate(
    TaskColor, "Color", 128  // Stack size
    ,
    NULL, 3  // Priority
    ,
    NULL);

  xTaskCreate(
    TaskSensor, "Sensor", 128  // Stack size
    ,
    NULL, 2  // Priority
    ,
    NULL);

  xTaskCreate(
    TaskIMU, "IMU", 128  // Stack size
    ,
    NULL, 1  // Priority
    ,
    NULL);

  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop() {
}

int sens;

void TaskSensor(void *pvParameters)  // This is a task.
{
  (void)pvParameters;

  for (;;) {

    if (Serial.available() > 0) {
      read = Serial.read();
      if (read == 'I') {
        s = Serial.parseInt();
        if (s == 10) {
          mode = 2;
          // isModeChanged = true;
        }
      }
    }
    if (mode == 2 || mode == 3 || mode == 4) {
      // Serial.print("MODE : ");
      // Serial.println(mode);
      digitalWrite(enPin, HIGH);
      sensor(&mode, color, topLift, lift, speedShooter, &isModeChanged);
    }

    vTaskDelay(1);  // one tick delay (15ms) in between reads for stability
  }
}

void TaskCom(void *pvParameters) {
  (void)pvParameters;

  for (;;) {
    int lastM;

    if (Serial.available() > 0) {
      if (read == 'M') {
        // Error Bola
        m = Serial.parseInt();
      } else if (read == 'N') {
        // Error Bola
        n = Serial.parseInt();
      }


      if (mode == 1) {
      } else {

        // Pas Ganti Mode stop dlu
        if (isModeChanged) {
          stop(mode);
          delay(2500);
          isModeChanged = false;
        } else {
          detect_object = detectObject(&mode, 0, 0);
          objectFol(&mode, detect_object, baseSpeed, degree);
          // Serial.print("TEST");
          // Serial.println(detect_object);
          // delay(15);
        }
      }

      lastM = m;
    }
    vTaskDelay(1);
  }
}

void TaskColor(void *pvParameters)  // This is a task.
{
  (void)pvParameters;

  for (;;) {

    color = colorSensor();
    // Serial.print(" Color : ");Serial.println(color);

    // CEK DATA

    vTaskDelay(1);  // one tick delay (15ms) in between reads for stability
  }
}

void TaskIMU(void *pvParameters)  // This is a task.
{
  (void)pvParameters;

  for (;;) {
    degree = imuDegree(&isIMUOn);
    // // Serial.print("error : ");
    // // Serial.println(degree);
    // // Serial.print("IMU : ");
    // // Serial.println(isIMUOn);
    // if (isIMUOn && mode == 3) {

    // }

    vTaskDelay(1);  // one tick delay (15ms) in between reads for stability
  }
}