#include <Arduino.h>
#include <Hugo.h>

#define SIZE_ACTION_MAX 32

typedef struct Instruction{
    char type;
    int16_t arg1;
} Instruction;

// /* [TYPE] [ARG1]*/
// Instruction tab_action_brut[SIZE_ACTION_MAX] = {
//     {'C', 1}, // Chasse neige Avant ouvrir = 1, fermer = 0
//     {'P', 1}, // Pince, relacher = 1, retracter = 0
//     {'F', 300}, // Fourche/Griffe position du dynamixel 
//     {'M', 020} // Stepper, arg1 => position, hauteur en mm
// };

Instruction *ptr_tab_action = NULL; // Initialiser le pointeur à NULL
int nouvelle_taille = 0; // Initialiser la taille du tableau à 0

char choix_utilisateur;

#define ACTION_PINCE_AVANT          1
#define ACTION_FOURCHE              2
#define ACTION_PELLE                3
#define ACTION_STEPPER_ASCENSEUR    4
#define ACTION_DELAY                5

typedef struct Etape
{
  /* data */
  int ACTION;
  int arg1;
}Etape;
Etape strategie_hackaton[SIZE_ACTION_MAX];
bool next_action = true;

void remplirTableauInstructions();
void Strategie_loop();
void remplirStructStrat();
void setup(){
    Serial.begin(115200);
    remplirTableauInstructions();


    Serial.println("setup() ...");


    Hugo_setup();
    remplirStructStrat();

    // analogReadResolution(10);

    Serial.println("Fin setup");
    delay(1000);
}

void loop(){
    Strategie_loop();
    delay(50);
}

void remplirTableauInstructions(){
    char instructionBuffer[20]; int nb_car = 0;
    Serial.println(".");
    Serial.println("Ecrire dans un fichier txt puis copier/coller dans le serial");
    Serial.println("C,1     : Chasse neige avant,  ouvrir = 1, fermer = 0");
    Serial.println("P,1     : Pince, relacher = 1, retracter = 0");
    Serial.println("F,300   : Fourche/Griffe, position du dynamixel avec trois chiffre entre [213;480], examples : 245");
    Serial.println("M,020   : Stepper, hauteur en mm, avec trois chiffre, examples : 020 pour 20 mm");
    Serial.println("D,3     : Delay, temps d'attente en seconde");
    Serial.println(".");
    Serial.println("Veuillez copier-coller les instructions et terminer en tapant deux fois sur entrer :");
    
    bool fin = false;
    while (!fin)
    {
        while (!Serial.available());
        char c = Serial.read(); Serial.print(c);
        // Terminer si une ligne vide est rencontrée
        if (c == '\n' && nb_car<=2) {
            fin = true;
        }
    // Si le caractère est un retour à la ligne
        if (c == '\n') {
             // Augmenter la taille du tableau
            ptr_tab_action = (Instruction*)realloc(ptr_tab_action, (nouvelle_taille + 1) * sizeof(Instruction));
            if (ptr_tab_action == NULL) {
                Serial.printf("Erreur d'allocation de mémoire.\n");
                return;
            }
            // Analyser l'instruction et réinitialiser le tampon
            (ptr_tab_action)[nouvelle_taille].type = instructionBuffer[0];
            if((ptr_tab_action)[nouvelle_taille].type == 'M' || (ptr_tab_action)[nouvelle_taille].type == 'F'){
                (ptr_tab_action)[nouvelle_taille].arg1 = (instructionBuffer[4]-'0') + 10 * (instructionBuffer[3]-'0') + 100 * (instructionBuffer[2]-'0');
            }else{
                (ptr_tab_action)[nouvelle_taille].arg1 = (instructionBuffer[2]-'0');
            }
            
            Serial.printf("     |   Type : %c , arg1 : %d, nb_car : %d\n", (ptr_tab_action)[nouvelle_taille].type, (ptr_tab_action)[nouvelle_taille].arg1, nb_car);
            (nouvelle_taille)++;
            nb_car = 0;
        } else {
            // Ajouter le caractère au tampon
            instructionBuffer[nb_car] = c;
            nb_car++;
        }

        
    }
    
    

    
    Serial.println(".");
    // Afficher le tableau
    for (int i = 0; i < nouvelle_taille; ++i) {
        Serial.printf("Instruction %d : Type = %c, Arg1 = %d\n", i + 1, ptr_tab_action[i].type, ptr_tab_action[i].arg1);
    }
    Serial.println(".");
    Serial.println(".");
    Serial.println(".");
}//*/

/*
void remplirTableauInstructions(){
    do {
        // Demander à l'utilisateur s'il veut ajouter une nouvelle instruction
        Serial.println(".");
        Serial.printf("Voulez-vous ajouter une nouvelle instruction ? (O/N): ");
        while (!Serial.available());
        choix_utilisateur = Serial.read();
        Serial.println(choix_utilisateur);

        if (choix_utilisateur == 'O' || choix_utilisateur == 'o') {
            // Augmenter la taille du tableau
            ptr_tab_action = (Instruction*)realloc(ptr_tab_action, (nouvelle_taille + 1) * sizeof(Instruction));

            if (ptr_tab_action == NULL) {
                Serial.printf("Erreur d'allocation de mémoire.\n");
                return;
            }

            // Demander les détails de la nouvelle instruction
            Serial.printf("Type de l'instruction %d : ", nouvelle_taille + 1);
             while (!Serial.available());
            (ptr_tab_action)[nouvelle_taille].type = Serial.read();
            Serial.println(ptr_tab_action[nouvelle_taille].type);

            Serial.printf("Arg1 de l'instruction %d : ", nouvelle_taille + 1);
            while (!Serial.available());
            (ptr_tab_action)[nouvelle_taille].arg1 = Serial.read() - '0';
            Serial.println(ptr_tab_action[nouvelle_taille].arg1);

            nouvelle_taille++;
        }
    } while (choix_utilisateur == 'O' || choix_utilisateur == 'o');

    Serial.println(".");
    // Afficher le tableau
    for (int i = 0; i < nouvelle_taille; ++i) {
        Serial.printf("Instruction %d : Type = %c, Arg1 = %d\n", i + 1, ptr_tab_action[i].type, ptr_tab_action[i].arg1);
    }
    Serial.println(".");
    Serial.println(".");
    Serial.println(".");
}
//*/

void Strategie_loop(){
   static int FIFO_lecture=0,FIFO_occupation=0,FIFO_max_occupation=0;

    FIFO_occupation = nouvelle_taille - FIFO_lecture;
    if(FIFO_occupation<0){FIFO_occupation=FIFO_occupation + nouvelle_taille;}
    if(FIFO_max_occupation<FIFO_occupation){FIFO_max_occupation=FIFO_occupation;}//Ajouter des conditions : attendre que l'action est été faite

    
    if(!FIFO_occupation){return;}
    if(!next_action){return;}
    next_action = false;
    Serial.print(FIFO_lecture);  Serial.print(" - Action : ");  Serial.println(strategie_hackaton[FIFO_lecture].ACTION); 

    
    switch (strategie_hackaton[FIFO_lecture].ACTION)
    {
    case ACTION_PINCE_AVANT:
        {
            bool lacher = strategie_hackaton[FIFO_lecture].arg1;
            PINCE_AVANT(lacher);
            next_action = true; delay(500);
        }
        break;
    case ACTION_FOURCHE:
        {
            // bool attraper = strategie_hackaton[FIFO_lecture].arg1;
            // FOURCHE(attraper);
            int position = strategie_hackaton[FIFO_lecture].arg1;
            if(position>ANGLE_INITIAL_ID_FOURCHE){position = ANGLE_INITIAL_ID_FOURCHE;}
            else if(position<ANGLE_FINAL_ID_FOURCHE){position = ANGLE_FINAL_ID_FOURCHE;}
            Pince.moveSpeed(ID_FOURCHE, position, 150);
            next_action = true; 
        }
        break;
    case ACTION_PELLE:
        {
            bool relacher = strategie_hackaton[FIFO_lecture].arg1;
            PELLE(relacher);
            next_action = true; delay(500);
        }
        break;
    case ACTION_STEPPER_ASCENSEUR:
        {
            int Hauteur = strategie_hackaton[FIFO_lecture].arg1;
            stepper.position(Hauteur*3600.0/80.0);//Conversion en dizieme de deg
            next_action = true; delay(500);
        }
        break;
    case ACTION_DELAY:
        {
            int time_ms = strategie_hackaton[FIFO_lecture].arg1 * 1000;
            delay(time_ms);
            next_action = true;
        }break;
    default:
        break;
    }

    FIFO_lecture ++; //Serial.printf("FIFO_lecture : %d\n",FIFO_lecture);//*/
}

void remplirStructStrat(){
    Serial.println ("remplirStructStrat... ***********************************");
    for(int act = 0; act< nouvelle_taille; act++){
        Serial.print(act);  Serial.print(" - Action : "); 
        switch (ptr_tab_action[act].type)
        {
        case 'C'://Pince avant
        {
            //mettre WaitForJack a true
            strategie_hackaton[act].ACTION = ACTION_PINCE_AVANT;
            strategie_hackaton[act].arg1 = ptr_tab_action[act].arg1;
            Serial.println ("ACTION_PINCE_AVANT");
        }
        break;
        case 'F'://Fourche
        {
            strategie_hackaton[act].ACTION = ACTION_FOURCHE;
            strategie_hackaton[act].arg1 = ptr_tab_action[act].arg1;
            Serial.println ("ACTION_FOURCHE");
        }
            break;
        case 'P'://PELLE
        {
            strategie_hackaton[act].ACTION = ACTION_PELLE;
            strategie_hackaton[act].arg1 = ptr_tab_action[act].arg1;
            Serial.println ("ACTION_PELLE");
        }
            break;
        case 'M'://Ascenseur
        {
            strategie_hackaton[act].ACTION = ACTION_STEPPER_ASCENSEUR;
            strategie_hackaton[act].arg1 = ptr_tab_action[act].arg1;
            Serial.println ("ACTION_STEPPER_ASCENSEUR");
        }
            break;
        case 'D'://Delay
        {
            strategie_hackaton[act].ACTION = ACTION_DELAY;
            strategie_hackaton[act].arg1 = ptr_tab_action[act].arg1;
            Serial.println ("ACTION_DELAY");
        }
        default:
            break;
        }
    }
    Serial.println ("*************************************************");
}

