#ifndef FUNGSIOBJECT_H
#define FUNGSIOBJECT_H
#include <Arduino.h>

int detectObject(int *mode, int errBall, int errSilo);
void objectFol(int *mode, int err, int baseSpeed, int degree);

#endif