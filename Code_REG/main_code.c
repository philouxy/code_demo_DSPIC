//----------------------------------------------------------------------------//
// Nom du projet 		:	Reglage Encodeur
// Nom du fichier 		:   main_code.c
// Date de création 	:   04.12.2019
// Date de modification : 	29.04.2021
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

//-- Macro de configuration selon le choix de l'échnatillonnage --// 
#if defined (CHOIX_TIME_1MS)
    long periodesaut = 5000;      // 5s => 5000ms / 1ms = 5000 
    #if defined (VITESSE)
        float saut = SAUT_VITESSE_1MS;
    #elif (defined (POSITION) || defined (POSITION_ANGULAIRE))
        float saut = SAUT_POSITION ;
    #else 
        float saut = 0; 
    #endif 
#elif defined (CHOIX_TIME_100US)
    long periodesaut = 50000;      // 5s => 5000ms / 0.1ms = 50 000
    #if defined (VITESSE)
        float saut = SAUT_VITESSE_100US;
    #elif (defined (POSITION) || defined (POSITION_ANGULAIRE))
        float saut = SAUT_POSITION ;
    #else 
        float saut = 0; 
    #endif   
#else 
   long periodesaut = 0;  
#endif

//-- variables globales utilisées dans plusieurs fichiers --// 
long temps_1ms = 0; 

long tempPID = 0, TempsautConsigne = 0, tempPosition = 0; 
long retenu = 0; 

//-- variable gloable position --//
long posk = 0, posk_1 = 0; 

//-- vitesse 
float vitessek, vitessek_1; 

//-- erreur 
float erreurk, erreurk_1; 

//-- consigne 
float consigneV, consigneP, consigne_P_k_1, consigneP_Ang, consigneP_Ang_k_1; 

//-- constante PID
float Kp = 0.02, Ki = 0, Kd = 0;  
float Up_k = 0, Ui_k = 0, Ui_k_1 = 0,  Ud_k = 0, Ud_k_1 = 0; 
float Uk; 

//-- prototype --// 
void Convertion_Cv_Cp(); 

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
            #if defined (VITESSE)
                CalculerVitesse();
                Convertion_Cv_Cp(); 
            #endif 
            CalculerErreur();
            CalculerPID(); 
            RaffraichirPWM(); 
            MAJVariables(); 
            tempPID = tempPID + PERIODE_ECHANTILLON_PID_MS; 
        }
        
        if(temps_1ms > TempsautConsigne)
        {
            CalculerConsigne(); 
            TempsautConsigne = TempsautConsigne + periodesaut; 
        }
        
        //-- représentation de la consigne et de la valeur regulée --// 
        //-- utilisation de macro pour faire ceci - ATTENTION NE PAS TOUCHER --//
        #if defined (VITESSE)
            //-- Représentation Consigne vitesse --//
            OC2RS = (((consigneV / (10484.0/10)) * 3685) + 1842); 
            //-- Représentation Vitesse régulée --//
            OC1RS = (((vitessek / (10484.0/10)) * 3685) + 1842); 
        #elif defined (POSITION)
            //-- Représentation Consigne position --//                          
            OC2RS = (((consigneP / 1048576.0) * 3685) + 1842);                  // // 4 * 65536 * 4 = 1048576  
            //-- Représentation Position régulée --//                           // 3685 = 100% Timer1
            OC1RS = (((posk / 1048576.0) * 3685) + 1842); 
        #elif defined (POSITION_ANGULAIRE)
            //-- Représentation Consigne position angulaire --//
            OC2RS = (((consigneP_Ang / (2*262144)) * 3685) + 1842);  
            //-- Représentation Position Angualire régulée --//
            OC1RS =  ((((posk * 3685) / (2*262144)) ) + 1842); 
        #endif         
    }

    return (EXIT_SUCCESS);
}


void CalculerVitesse(void)
{   
    vitessek = posk - posk_1; 
}



void CalculerErreur()
{
    //-- macro de sélection - NE PAS TOUCHER !!! --//
    
    #if (defined (VITESSE) || defined (POSITION))
        erreurk = consigneP - posk; 
    #elif defined (POSITION_ANGULAIRE)
        erreurk = consigneP_Ang - posk; 
    #else
        erreurk = consigneV - vitessek; 
    #endif
}

void CalculerPID(void) 
{
    //-- proportionnel --//
    Up_k = Kp * erreurk; 
    
    //-- intégrateur --// 
    Ui_k = Ki * erreurk + Ui_k_1; 
    
    //-- dérivateur --// 
    //Ud_k = Kd * erreurk + Ud_k_1; 
            
    //-- final --// 
    Uk = Up_k + Ui_k; // + Ud_k; 
} 


void RaffraichirPWM(void) 
{
    ConfigTimerPWM(Uk); //insérer valeur PWM 
} 


void MAJVariables(void) 
{
    posk_1 = posk; 
    //Ud_k_1 = Ud_k; 
    Ui_k_1 = Ui_k; 
    consigne_P_k_1 = consigneP; 
    
    //pour test ne pas toucher // 
    //tempPID = tempPID + PERIODE_ECHANTILLON_PID_MS; 
    //vitessek_1 = vitessek; 
}

void CalculerConsigne(void)
{
    //-- déclaration des variables --// 
    static int states = 0;
        
    states++; 
    
    if(states == 1)
    {
        #if defined (VITESSE)
            consigneV = 0; 
        #elif defined (POSITION)
             consigneP = 0; 
        #elif defined (POSITION_ANGULAIRE)
             consigneP_Ang = 0; 
        #endif 
    }
    else if(states == 2)
    {
        #if defined (VITESSE)
             consigneV = saut;
        #elif defined (POSITION)
             consigneP = saut; 
        #elif defined (POSITION_ANGULAIRE)
             consigneP_Ang = saut / 2;   
        #endif 
    }
    else if(states == 3)
    {
        #if defined (VITESSE)
             consigneV = -saut;
        #elif defined (POSITION)
             consigneP = -saut; 
        #elif defined (POSITION_ANGULAIRE)
             consigneP_Ang = -(saut / 2);   
        #endif 

        //-- remise à zéro de l'état states --//
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

void Convertion_Cv_Cp()
{
     consigneP = consigneV + consigne_P_k_1; 
}



