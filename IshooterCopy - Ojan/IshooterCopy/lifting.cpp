#include "lifting.h"
#include "Slave.h"

char LTopLift = 'F';
char RTopLift = 'E';
char LBLift = 'G';
char RBLift = 'H';

void lifting(int a, int speed){
  if(a == 1){
    // Serial.print("Speed: ");
    // Serial.println(speed);
    slave(RBLift, speed);
    slave(LBLift, speed);
  }else{
    slave(RBLift, 0);
    slave(LBLift, 0);
  }
}

void topLifting(int a, int speed){
  if(a == 1){
    slave(RTopLift, speed);
    slave(LTopLift, speed);
  }else{
    slave(RTopLift, 0);
    slave(LTopLift, 0);
  }
}