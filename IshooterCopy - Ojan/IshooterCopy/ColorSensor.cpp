#include "Arduino.h"
#include "ColorSensor.h"
#define s3 10
#define out 9

int Red = 0, Blue = 0;
int warna = 0;

int colorSensor() {
  analogWrite(s3, 0);
  Red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH, 1000);
  Red = Red == 0 ? 1000 : Red;
  // delay(20);
  analogWrite(s3, 1023);
  Blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH, 1000);
  Blue = Blue == 0 ? 1000 : Blue;

  // delay(20);
  // Serial.print("Merah : ");
  // Serial.print(Red);
  // Serial.print("\t");
  // Serial.print("Biru : ");
  // Serial.println(Blue);

  // Jika Merah kurang dari 250 dan kurang dari nilai biru, maka Merah
  if (Red < 300 && Red < Blue){
    return 1;
  }
  // Serial.println("Merah");
  else if (Blue < 300 && Blue < Red)
    return 2;
  // Serial.println("Biru");
  else {
    return 0;
  }
  // Serial.println("Gatau");
}