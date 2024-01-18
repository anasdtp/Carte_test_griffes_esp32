#include "Stepper.h"

STEPPER::STEPPER(const int STEPpin, const int DIRpin, const int MOpin){
    p_STEP = STEPpin; p_DIR = DIRpin; p_MO = MOpin;

    pinMode(p_STEP, OUTPUT); pinMode(p_DIR, OUTPUT); pinMode(p_MO, OUTPUT);
}

void STEPPER::STEP(bool etat){
    digitalWrite(p_STEP, etat);
}

void STEPPER::DIR(bool etat){
    digitalWrite(p_DIR, etat);
}

void STEPPER::MO(bool etat){
    digitalWrite(p_MO, etat);
    if(etat){m0 = 1;}else{m0 = 0;}
}


void STEPPER::stepper(int swpulse, int m0, int dir){
  if (m0==0) {MO(LOW);} else {MO(HIGH);}
  if (dir==0) {DIR(LOW);} else {DIR(HIGH);}

  // STEP GENERATOR
  for ( int i = 0; i < swpulse; i++) {
      STEP(HIGH);
      delayMicroseconds(1000); 
      STEP(LOW);
      delayMicroseconds(1000); 


  }
  return;
}

// Function to move the motor to a specific position (in degrees)
void STEPPER::position(int degree) {//3600° c'est 10 tours
  if(degree == currentPosDegree){return;}
  float degreeParStep = DEGREPARSTEP; int dir = 0;
  //      m0 == 0 && m1 == 0 && m2 == 0
  if     (m0 == 1){degreeParStep /= 2.0;}

  int degreeAparcourir = degree - currentPosDegree;
  if(degreeAparcourir<0){dir = 1; degreeAparcourir *= -1;}

  // Calculate the number of steps needed to reach the target position
  float steps = degreeAparcourir / degreeParStep *1.0; 
  // Update the current position of the motor
  currentPosDegree = degree;
  // Call the "stepper" function to move the motor
  stepper((int)steps, m0, dir); // 1/2 microstep resolution (M0 = 1, M1 = 0, M2 = 0)
}