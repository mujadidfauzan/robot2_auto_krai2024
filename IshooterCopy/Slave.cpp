#include "Slave.h"

void slave(char address, int data) {
  Serial.println(address);
  Serial.println(data);
  // delay(15);
}

void slave(char address, int data, int data2) {
  Serial.println(address);
  Serial.println(data);
  Serial.println(data2);
  // delay(15);
}

void slave(char address, int data, int data2, int data3) {
  Serial.println(address);
  Serial.println(data);
  Serial.println(data2);
  Serial.println(data3);
  // delay(15);
}

void slave(char address, int data, int data2, int data3, int data4) {
  Serial.println(address);
  Serial.println(data);
  Serial.println(data2);
  Serial.println(data3);
  Serial.println(data4);

  // delay(15);
}