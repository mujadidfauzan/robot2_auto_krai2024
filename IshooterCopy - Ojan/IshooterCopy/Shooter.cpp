#include "Shooter.h"

#define R_PWM 5
#define L_PWM 6

void shooter(int a, int speed) {
  if (a == 1) {
    analogWrite(L_PWM, speed);
    analogWrite(R_PWM, 0);
    // Serial.println("test");
  } else {
    analogWrite(R_PWM, 0);
    analogWrite(L_PWM, 0);
  }
}