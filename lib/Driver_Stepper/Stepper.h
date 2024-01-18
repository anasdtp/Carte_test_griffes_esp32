#include <Arduino.h>

#define DEGREPARSTEP 1.8 //a confirmer
#define MOUVEMENT_ROTATION 2
#define MOUVEMENT_LIGNE_DROITE 3

class STEPPER {
protected:
    int p_STEP, p_DIR, p_MO;

public:
   STEPPER(const int STEPpin, const int DIRpin, const int MOpin);
   void STEP(bool etat = 0);
   void DIR(bool etat = 0);
   void MO(bool etat = 0);
   char m0 = 0; int currentPosDegree = 0;

   void stepper(int swpulse, int m0, int dir);
   void position(int degree);
};