#include <Arduino.h>
#include <AX12A.h>
#include <Stepper.h>

#define DirectionPin  (10u)
#define BaudRate      (1000000ul)

#define ID_PINCE_AVANT_1     1
#define ID_PINCE_AVANT_2     2
#define ID_FOURCHE           3
#define ID_PELLE             4

//----------------------- STRATEGIE -------------------------

#define ANGLE_INITIAL_PINCE_AVANT_1 212
#define ANGLE_FINAL_PINCE_AVANT_1 646

#define ANGLE_INITIAL_PINCE_AVANT_2 780
#define ANGLE_FINAL_PINCE_AVANT_2 358



#define ANGLE_INITIAL_ID_FOURCHE 480
#define ANGLE_FINAL_ID_FOURCHE 213

#define ANGLE_FINAL_PELLE 580
#define ANGLE_INITIAL_PELLE 110

extern AX12A Pince;
extern STEPPER stepper;

void Hugo_setup();

void PINCE_AVANT(bool lacher   = false);
void FOURCHE    (bool attraper = false);
void PELLE      (bool relacher = false);
void stepper_Homing();