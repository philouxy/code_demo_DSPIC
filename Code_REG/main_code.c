//----------------------------------------------------------------------------//
// Nom du projet 		:	Reglage Encodeur
// Nom du fichier 		:   main_code.c
// Date de création 	:   04.12.2019
// Date de modification : 	10.02.2021
// 
// Auteur 				: 	Philou (Ph. Bovey) 
//                      :   Michel Bonzon 
//                      :   Kevin Bougnon 
//
// Description 			: 	Canevas pour le laboratoire de réglage avec la carte
//                          1601_HalfBridge 
//                         
// Remarques			: 
// 	    chemin pour trouver le headerfile 
//		C:\Program Files\Microchip\MPLAB C30\support\dsPIC33F\h
//
//      Hardware de la carte => ATTENTION PIC MONTER SUR LA CARTE DSPIC33FJ128MC802
//      K:\ES\PROJETS\SLO\1601x_HalfBridge3x\doc 
//
//	    doc pour le DSP : 
//		K:\ES\PROJETS\SLO\1601x_HalfBridge3x\doc\datasheets
//
//      doc pour la configuration du PWM : 
//      https://ww1.microchip.com/downloads/en/DeviceDoc/70187E.pdf
//
//      doc pour la configuration du module de quadrature 
//      https://ww1.microchip.com/downloads/en/DeviceDoc/70208C.pdf
//
//----------------------------------------------------------------------------//

//--- librairie à inclure ---// 
#if defined(__dsPIC33F__)
#include "p33Fxxxx.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <xc.h>                             // appel de directive du compilateur 
                                            // C pour le Dspic 
#include "math.h"

#include "configuration_bitsSettings.h"     // configuration des bits settings 
                                            // pour l'oscillateur 
#include "configOscillateur.h"              // configuration de l'oscillateur 
                                            // interne du Dspic
#include "configTimer.h"                    // configuration du module timers
#include "config_IO.h"                      // configuration des entrées - sorties
#include "configPWM.h"                      // configuration du module PWM
#include "configQuadrature.h"               // configuration du module en Quadrature
#include "configOC.h"                       // configuration du module OutputCapture
#include "reg_V.h"                          // librairie pour de la régulation de vitesse 

#define NBR_PAS_MAX_ECHANTILLONER 1024

//-- variables globales utilisées dans plusieurs fichiers --// 
int8_t flagInterrupt = 0;                   

long temps_1ms = 0; 

long periodesaut = 5000;                   // 5s 
long periodePID = 10;                       // 1ms 

long tempPID = 0, TempsautConsigne = 0; 

//-- position 
long retenu = 0;
long posk = 0, posk_1 = 0; 

//-- vitesse 
float vitessek, vitessek_1; 

//-- erreur 
float erreurk, erreurk_1; 

//-- consigne 
float consineV, consigneP; 

//-- constante PID
float Kp = 0.1, Ki, Kd;  
float Up_k, Ui_k, Ui_k_1,  Ud_k; 
float Uk; 

//-- consigne --//
float saut = 2621;                       // upas = 512*128*4 / 10ms   => 4 tous les flancs 


/*void CalculerVitesse(void); 
void CalculerErreur(void);
void CalculerPID(void); 
void RaffraichirPWM(void); 
void MAJVariables(void); 
void CalculerConsigne(void); 
void LectureImagePosition(void);*/ 



int main(int argc, char** argv) 
{
    //-- déclaration de variable --//
    
    //-- appel de fonction d'initialisation --//
    InitOscillateur();          // configuration de l'oscillateur 
    
    Timer1Init();               // initialisation du Timer 1 
    Timer2Init();               // initialisation du Timer 2 
    
    ConfigModulePWM();          // configuration des modules PWM1 et PWM2
    
    ConfigModuleQadrature();    // configuration du module en quatrature pour 
                                // les entrées de l'encodeur A et B    
    
    ConfigPWM_OC1();            // représentation temporelle de la vitesse angulaire  
    ConfigPWM_OC2();            // représentation temporelle de la consigne de 
                                // la vitesse angulaire 
    
    IOinit();                   // initialisation des entrées/sorties 
    
    T2CONbits.TON = 1;          // Enable Timer2 and start the counter
    
    //-- boucle sans fin --// 
    while(1)
    {
        if (temps_1ms > tempPID)
        {
            LectureImagePosition(); 
            CalculerVitesse(); 
            CalculerErreur();
            CalculerPID(); 
            RaffraichirPWM(); 
            MAJVariables(); 
            tempPID = tempPID + periodePID; 
        }
        
        if(temps_1ms > TempsautConsigne)
        {
            CalculerConsigne(); 
            TempsautConsigne = TempsautConsigne + periodesaut; 
        }
        
        // 3685 = 100% Timer1
        
        OC1RS = (((vitessek / 10484.0) * 3685) + 1842); 
        OC2RS = (((consineV / 10484.0) * 3685) + 1842);   
    }

    return (EXIT_SUCCESS);
}


void CalculerVitesse(void)
{   
    vitessek = posk - posk_1; 
}

void CalculerErreur()
{
    erreurk = consineV - vitessek; 
}

void CalculerPID(void) 
{
    
    //-- proportionnel 
    Up_k = Kp * erreurk; 
    Uk = Up_k; 
} 


void RaffraichirPWM(void) 
{
    ConfigTimerPWM(Uk); //insérer valeur PWM 
} 


void MAJVariables(void) 
{
    //tempPID = tempPID + periodePID; 
    vitessek_1 = vitessek; 
    posk_1 = posk; 
    
}

void CalculerConsigne(void)
{
    // variable 
    static int states = 0;
    
    states++; 
    
    if(states == 1)
        consineV = 0; 
    else if(states == 2)
    {
       consineV = saut;
       states = 0;
    }
    else if(states == 3)
    {
        consineV = -saut; 
        states = 0;
    }
}

void LectureImagePosition(void)
{
    long cptQadratureK; 
    static long cptQadratureK_1 = 0; 
    
    cptQadratureK = POS1CNT;         // lire compteur quadrature 
            
    if (cptQadratureK - cptQadratureK_1 >= 32768)
    { 
        retenu -= 1;  
    }
    else if(cptQadratureK - cptQadratureK_1 <= -32768)
    {
        retenu += 1; 
    }
    
    posk = retenu * pow(2, 16) + cptQadratureK; 
    cptQadratureK_1 = cptQadratureK; 
            
}

