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

#include "config_IO.h"



//----------------------------------------------------------------------------//
//-- nom 				: IOinit
//-- entrée - sortie 	: - / - 
//-- description 		: configuration des entrées / sorties  
//-- remarque 			: 
//                        
//-- modification       : 10.02.2021
//----------------------------------------------------------------------------//
inline void IOinit(void)
{
    //-- configuration des entrées --// 
    
    //-- remapping --//
    //-- configuration du module de quatrature --// 
    //-- configuration des entres A et B encodeur 
    RPINR14bits.QEA1R = 6;      // correspond à la pin RP6 => A
    RPINR14bits.QEB1R = 5;      // correspond à la pin RP5 => B 
    
    //-- configuration des sorties --//
    TRISBbits.TRISB8 = 0;               // pin8 sur port B en sortie 
    
    //-- activation du pont H au niveau hardware BTN8982TA --//
    //-- PIN RB8 en sortie à 1 --//
    LATBbits.LATB8 = 1; 
    
    //-- remapping --//
    RPOR7bits.RP15R = 0x12;             //configuration de la pin 15 en sortie OC1
    RPOR6bits.RP13R = 0x13;             // configuration de la pin 13 en sortie Oc2
}