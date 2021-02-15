//----------------------------------------------------------------------------//
// Nom du projet 		:	Reglage Encodeur
// Nom du fichier 		:   configModuleQudrature.c 
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
#include "configOC.h"

//----------------------------------------------------------------------------//
//-- nom 				: ConfigPWM_OC1
//-- entrée - sortie 	: - / - 
//-- description 		: configuration du modules output capture OC1  
//-- remarque 			: datasheet DSPIC -> OC section 15 -> p209
//
//-- modification       : 10.02.2021
//----------------------------------------------------------------------------//
inline void ConfigPWM_OC1()
{
    OC1CONbits.OCM = 0;         // désactivation  du module 
    OC1CONbits.OCTSEL = 0;      // OC configurer avec le timer2
    OC1CONbits.OCM = 6;         // Activation
    
    //-- valeur de recharge de registres de travail pour avant de compter --// 
    OC1R = 100;
    OC1RS = 200; 
}

//----------------------------------------------------------------------------//
//-- nom 				: ConfigPWM_OC2
//-- entrée - sortie 	: - / - 
//-- description 		: configuration du modules output capture OC2  
//-- remarque 			: datasheet DSPIC -> OC section 15 -> p209
//
//-- modification       : 10.02.2021
//----------------------------------------------------------------------------//
inline void ConfigPWM_OC2()
{
    OC2CONbits.OCM = 0;         // désactivation
    OC2CONbits.OCTSEL = 0;      // OC configurer avec le timer2
    OC2CONbits.OCM = 6;         // activation 
    
    //-- valeur de recharge de registres de travail pour avant de compter --// 
    OC2R = 100;
    OC2RS = 200;     
}