//----------------------------------------------------------------------------//
// Nom du projet 		:	Reglage Encodeur
// Nom du fichier 		:   configOscillateur.h 
// Date de cr�ation 	:   03.12.2020
// Date de modification : 	29.04.2021
// 
// Auteur 				: 	Philou (Ph. Bovey)
//                      :   Michel Bonzon 
//                      :   Kevin Bougnon 
//
// Description 			: 	Canevas pour le laboratoire de r�glage avec la carte
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
//-- librairie standart --// 
#include <stdint.h>

#ifndef CONFIGTIMER_H
#define	CONFIGTIMER_H

//-- ATTENTION A METTRE EN COMMENTAIRE LE TIMMING QUE L'ON VEUT PAS --//
//-- POUR LE CHOIX DE LA CONFIGURATION DU TIMER D'UTILISATION       --// 
#define CHOIX_TIME_1MS 100            // 1ms => 1ms / 10us = 100
//#define CHOIX_TIME_100US 10         // 100us => 100us / 10us = 10


//-- variables globales --// 
extern int8_t flagInterrupt;
extern long temps_1ms; 

//-- prototypes --// 
inline void Timer1Init(void); 
inline void Timer2Init(void); 
inline void InterruptTimer1Init(void); 
inline void InterruptTimer2Init(void); 

//-- prototypes -> interruption --// 
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt( void ); 
//void __attribute__((interrupt, no_auto_psv)) _T2Interrupt( void ); 

#endif	/* CONFIGTIMER_H */

