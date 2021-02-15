//----------------------------------------------------------------------------//
// Nom du projet 		:	Reglage Encodeur
// Nom du fichier 		:   configModuleI_O.c 
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

#include <xc.h>                 // appel de directive du compilateur C pour le Dspic
#include "configPWM.h"


//----------------------------------------------------------------------------//
//-- nom 				: configModulePWM
//-- entrée - sortie 	: - / - 
//-- description 		: 
//-- remarque 			: datasheet DSPIC 
//----------------------------------------------------------------------------//
inline void ConfigModulePWM(void)
{   
    P1TCONbits.PTMOD = 0;   // free Running mode
    P1TCONbits.PTCKPS = 0;  // prescelaire configurer à 1 : 1 / fcyc = tcyc 
    P1TCONbits.PTOPS = 0;   // postscaler 1 : 1 
    
    // registre de 16bits pour la mise à jour de la péridode du PWM 
    // période : 100kHz même chose que le timer 1 => 10us (T_PWM)
    // tcyc = 1 / Fcyc = 1 / 36.85 Mhz = 27ns 
    // N-Tic_PWM = T_PWM / Tcyc = 10us / 27ns = 370 tic 
    P1TPER = 370; 
    
    PWM1CON1 = 0;           // pour un clear du regitstre
    
    //-- activation PWM1 --// 
    PWM1CON1bits.PEN1H = 1; 
    PWM1CON1bits.PEN1L = 0; 
    PWM1CON1bits.PMOD1 = 1; 
  
    //-- activation PWM2 --// 
    PWM1CON1bits.PEN2H = 1; 
    PWM1CON1bits.PEN2L = 0; 
    PWM1CON1bits.PMOD2 = 1; 

    // MAJ du Timer Counter lié au PWM à zéro + activation mode PWM  
    P1TMR = 0;
    P1TCONbits.PTEN = 1;        // activer les PWM
}

//----------------------------------------------------------------------------//
//-- nom 				: ConfigTimerPWM
//-- entrée - sortie 	: - / - 
//-- description 		: configuration du Timer pour le PWM 
//-- remarque 			: datasheet DSPIC -> timer section 16 -> p213
//                        configuration de la période du PWM
//                        Fcy : 36.85 [MHz]
//                        nb_tic = Fcyc / Ft1 
//                        36.85 x 10^6 / 100 x 10^3 = 36.85
//-- modification       : 28.01.2021
//----------------------------------------------------------------------------//
void ConfigTimerPWM(float pourcentPWM) 
{
    //-- déclaration de varaible --// 
    float var1, var2; 
    
    //-- gestion limte valeur PWM --//
    if(pourcentPWM > 100)
        pourcentPWM = 100; 
    else if (pourcentPWM < -100)
        pourcentPWM = -100; 
    
    // PWM à 50% (la mise à jour ce fait à 2 x la valeur de la période)
    // ex : (2 x P1TPER + 2) * rapport cyclique (60/100)
    var1 = OFFSET + (pourcentPWM * PENTE) ; 
    P1DC1 = (2 * P1TPER + 2) * (var1/100.0); 
    
    // PWM à 50% (la mise à jour ce fait à 2 x la valeur de la période)
    // ex : (2 x P1TPER + 2) * rapport cyclique (60/100)
    var2 = OFFSET - (pourcentPWM * PENTE) ; 
    P1DC2 = (2 * P1TPER + 2) * (var2/100.0); 
}
