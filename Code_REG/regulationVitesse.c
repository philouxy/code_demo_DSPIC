//----------------------------------------------------------------------------//
// Nom du projet 		:	Reglage Encodeur
// Nom du fichier 		:   regulationVitesse.c
// Date de création 	:   10.02.2021
// Date de modification : 	xx.xx.2021
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
//----------------------------------------------------------------------------//
//--- librairie à inclure ---// 
#if defined(__dsPIC33F__)
#include "p33Fxxxx.h"
#endif

#include <stdint.h>
#include <math.h>

#include <xc.h>                 // appel de directive du compilateur C pour le Dspic 
//#include "reg_V.h"

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


//----------------------------------------------------------------------------//
//-- nom 				: LectureImagePosition
//-- entrée - sortie 	: - / - 
//-- description 		:  
//-- remarque 			: 
//
//-- modification       : 10.02.2021
//----------------------------------------------------------------------------//

void LectureImagePosition(void)
{
    //-- déclaration de variable --// 
    long cptQadratureK;                     // compteur de flanc actuelle 
    static long cptQadratureK_1 = 0;        // compteur de flanc passé 
    
    cptQadratureK = POS1CNT;                // lire compteur de flanc
    
    //-- gestion de la retenu pour savoir si il y a eu dépassement ou non --// 
    if (cptQadratureK - cptQadratureK_1 >= 32768)
    { 
        retenu -= 1;  
    }
    else if(cptQadratureK - cptQadratureK_1 <= -32768)
    {
        retenu += 1; 
    }
    
    //-- MAJ de la position 
    posk = retenu * pow(2, 16) + cptQadratureK; 
    
    //-- MAJ du compteur flanc après gestion de la retenu --// 
    cptQadratureK_1 = cptQadratureK; 
            
}
