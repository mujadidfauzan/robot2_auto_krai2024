#ifndef GERAKMOTOR_H
#define GERAKMOTOR_H
#include <Arduino.h>

void driveMotor(int  mode ,int m1, int m2);
void driveMotor(int mode ,int m1, int m2, int m3, int m4);
void geserKanan(int mode, int spd);
void geserKiri(int mode, int spd);
void belok_kanan(int  mode ,int spd);
void belok_kiri(int  mode ,int spd);
void rotasi_kiri(int  mode ,int spd);
void rotasi_kanan(int  mode ,int spd);
void diagonal_kanan(int  mode ,int spd);
void diagonal_kiri(int  mode ,int spd);
void stop(int  mode );

#endif