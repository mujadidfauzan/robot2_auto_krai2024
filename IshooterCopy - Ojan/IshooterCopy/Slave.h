#ifndef SLAVE_H
#define SLAVE_H
#include <Arduino.h>

void slave(char address, int data);
void slave(char address, int data, int data2);
void slave(char address, int data, int data2, int data3);
void slave(char address, int data, int data2, int data3, int data4);

#endif