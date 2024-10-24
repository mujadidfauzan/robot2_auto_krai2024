#include "HardwareSerial.h"
#include "FungsiObject.h"
#include "GerakMotor.h"

int detectObject(int *mode, int errBall, int errSilo) {
  if (*mode == 2) {
    return map(errBall, -330, 330, -10, 10);
  } else if (*mode == 4) {
    return map(errSilo, -330, 330, -10, 10);
  }
}

void objectFol(int *mode, int err, int baseSpeed, int degree) {
  // bool imuOn = (degree > 3 || degree < -3);
  if (*mode == 2) {
    if (err >= -10 && err <= -8) {
      belok_kiri(*mode, baseSpeed);
    } else if (err >= -7 && err <= -5) {
      driveMotor(*mode, baseSpeed * 0.5, baseSpeed);
    } else if (err >= -4 && err <= -2) {
      driveMotor(*mode, baseSpeed * 0.7, baseSpeed);
    } else if (err == -1) {
      driveMotor(*mode, baseSpeed * 0.85, baseSpeed);
    } else if (err == 0) {
      driveMotor(*mode, baseSpeed, baseSpeed);
    } else if (err == 1) {
      driveMotor(*mode, baseSpeed, baseSpeed * 0.85);
    } else if (err <= 4 && err >= 2) {
      driveMotor(*mode, baseSpeed, baseSpeed * 0.7);
    } else if (err <= 7 && err >= 5) {
      driveMotor(*mode, baseSpeed, baseSpeed * 0.5);
    } else if (err <= 10 && err >= 8) {
      belok_kanan(*mode, baseSpeed);
    } else {
      driveMotor(*mode, baseSpeed - 20, baseSpeed - 20);
    }
  }

  if (*mode == 4) {
    // if (abs(degree) > 30) {
    //   *mode = 3;
    // }
    // Serial.print("Err : ");
    // Serial.println(err);
    // Serial.println("Mode 4");
    // baseSpeed = 90;
    if (err >= -10 && err <= -8) {
      diagonal_kiri(*mode, baseSpeed + 20);
    } else if (err >= -7 && err <= -5) {
      diagonal_kiri(*mode, baseSpeed + 10);
    } else if (err >= -4 && err <= -2) {
      diagonal_kiri(*mode, baseSpeed);
    } else if (err == -1) {
      driveMotor(*mode, baseSpeed, baseSpeed * 0.9);
    } else if (err == 0) {
      driveMotor(*mode, baseSpeed, baseSpeed);
    } else if (err == 1) {
      driveMotor(*mode, baseSpeed * 0.9, baseSpeed);
    } else if (err <= 4 && err >= 2) {
      diagonal_kanan(*mode, baseSpeed);
    } else if (err <= 7 && err >= 5) {
      diagonal_kanan(*mode, baseSpeed + 10);
    } else if (err <= 10 && err >= 8) {
      diagonal_kanan(*mode, baseSpeed + 20);
    } else {
      stop(*mode);
    }
  }


  if (*mode == 3) {
    // Serial.print("Sudut : ");
    // Serial.println(degree);
    if (abs(degree) <= 3) {
      *mode = 4;
    }

    if (degree < -10) {
      // Serial.println("Kiri");
      rotasi_kiri(*mode, 50);
    } else if (degree < -3) {
      // Serial.println("Kiri");
      rotasi_kiri(*mode, 30);
    } else if (degree > 10) {
      // Serial.println("Kanan");
      rotasi_kanan(*mode, 50);
    } else if (degree > 3) {
      // Serial.println("Kanan");
      rotasi_kanan(*mode, 30);
    }
  }
}