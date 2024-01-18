#include "Hugo.h"

AX12A Pince;

STEPPER stepper(18,19,15);

const int pinFDCHoming = 33;

void Hugo_setup(){
  pinMode(pinFDCHoming,INPUT_PULLDOWN);
  Pince.begin(BaudRate, DirectionPin, &Serial2);
  delay(1000);
  FOURCHE();
  PINCE_AVANT();
  PELLE();

  FOURCHE(true);
  stepper_Homing();
  FOURCHE();
}

void PINCE_AVANT(bool lacher){
  if(!lacher){
    Pince.moveSpeed(ID_PINCE_AVANT_1, ANGLE_INITIAL_PINCE_AVANT_1, 500);
    delay(500);
    Pince.moveSpeed(ID_PINCE_AVANT_2, ANGLE_INITIAL_PINCE_AVANT_2, 500);
  }else{
    Pince.moveSpeed(ID_PINCE_AVANT_2, ANGLE_FINAL_PINCE_AVANT_2, 500);
    delay(500);
    Pince.moveSpeed(ID_PINCE_AVANT_1, ANGLE_FINAL_PINCE_AVANT_1, 500);
  }
  delay(500);
}

void FOURCHE(bool attraper){
  if(attraper){
    // for(int i = 4; i>0;i--){
    //   Pince.moveSpeed(ID_FOURCHE, ANGLE_FINAL_ID_FOURCHE - 40*i, 150);
    //   delay(250);
    //   Pince.moveSpeed(ID_FOURCHE, (ANGLE_FINAL_ID_FOURCHE - 40*i)-20, 150);
    //   delay(250);
    //   Pince.moveSpeed(ID_FOURCHE, ANGLE_FINAL_ID_FOURCHE - 40*i, 150);
    //   delay(250);
    //   Pince.moveSpeed(ID_FOURCHE, (ANGLE_FINAL_ID_FOURCHE - 40*i)-10, 150);
    //   delay(250);
    // }
    // Pince.moveSpeed(ID_FOURCHE, ANGLE_FINAL_ID_FOURCHE-40, 150);
    Pince.moveSpeed(ID_FOURCHE, ANGLE_FINAL_ID_FOURCHE, 150);
  }else{
    Pince.moveSpeed(ID_FOURCHE, ANGLE_INITIAL_ID_FOURCHE, 150);
  }
  delay(500);
}

void PELLE(bool relacher){
  if(relacher){
    Pince.moveSpeed(ID_PELLE, ANGLE_FINAL_PELLE, 500);
  }else{
    Pince.moveSpeed(ID_PELLE, ANGLE_INITIAL_PELLE, 500);
  }
  delay(500);
}

void stepper_Homing(){
  int debut_ms = millis(), timeout = 5*1000;

  int i = 0;
  while ((millis()-debut_ms)<timeout)
  {
    stepper.position(2*i--);
    if(digitalRead(pinFDCHoming)){
      stepper.currentPosDegree = 0;
      stepper.position(20 * 3600./80.);
      return;
    }
    delay(1);
  }
  
}