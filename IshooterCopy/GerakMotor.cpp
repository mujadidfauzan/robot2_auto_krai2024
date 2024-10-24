#include "HardwareSerial.h"
#include "GerakMotor.h"
#include "Slave.h"

// Address Slave
char A_RF = 'A';  // Roda kanan depan
char B_LF = 'B';  // Roda kiri depan
char C_RB = 'C';  // Roda kanan belakang
char D_LB = 'D';  // Roda kiri belakang

void driveMotor(int mode, int m1, int m2) {
  if (mode == 2) {
    slave(A_RF, m1);
    slave(B_LF, m2);
    slave(C_RB, m1);
    slave(D_LB, m2);
  } else if (mode == 3 || mode == 4) {
    slave(A_RF, -m1);
    slave(B_LF, -m2);
    slave(C_RB, -m1);
    slave(D_LB, -m2);
  }
};

void driveMotor(int mode, int m1, int m2, int m3, int m4) {
  if (mode == 2) {
    slave(A_RF, m1);
    slave(B_LF, m2);
    slave(C_RB, m3);
    slave(D_LB, m4);
  } else if (mode == 3 || mode == 4) {
    slave(A_RF, -m1);
    slave(B_LF, -m2);
    slave(C_RB, -m3);
    slave(D_LB, -m4);
  }
};

void geserKiri(int mode, int spd) {
  driveMotor(mode, spd, -spd, -spd, spd);
}

void geserKanan(int mode, int spd) {
  driveMotor(mode, -spd, spd, spd, -spd);
}

void belok_kanan(int mode, int spd) {
  driveMotor(mode, spd, (spd * (0.4)), spd, (spd * (0.4)));
};

void belok_kiri(int mode, int spd) {
  driveMotor(mode, (spd * (0.4)), spd, (spd * (0.4)), spd);
};

void rotasi_kiri(int mode, int spd) {
  driveMotor(mode, spd, -spd, spd, -spd);
};

void rotasi_kanan(int mode, int spd) {
  driveMotor(mode, -spd, spd, -spd, spd);
};

void diagonal_kiri(int mode, int spd) {
  driveMotor(mode, spd, 0, 0, spd);
};

void diagonal_kanan(int mode, int spd) {
  driveMotor(mode, 0, spd, spd, 0);
};

void stop(int mode) {
  driveMotor(mode, 0, 0, 0, 0);
};