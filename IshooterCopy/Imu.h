#ifndef IMU_H
#define IMU_H
#include <Arduino.h>

int imuDegree(bool *isIMUOn);
int normalizeDegree(int errDegree);

#endif