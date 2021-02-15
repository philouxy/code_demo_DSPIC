//----------------------------------------------------------------------------//
// Nom du projet 		:	Reglage Encodeur
// Nom du fichier 		:   configModuleQudrature.c 
// Date de cr�ation 	:   10.02.2021
// Date de modification : 	xx.xx.2021
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
//--- librairie � inclure ---// 
#if defined(__dsPIC33F__)
#include "p33Fxxxx.h"
#endif

#include <stdint.h>

#include <xc.h>                 // appel de directive du compilateur C pour le Dspic
#include "configQuadrature.h"

//----------------------------------------------------------------------------//
//-- nom 				: configModuleQadrature
//-- entr�e - sortie 	: - / - 
//-- description 		: 
//-- remarque 			: datasheet DSPIC 
//----------------------------------------------------------------------------//
inline void ConfigModuleQadrature()
{
    QEI1CONbits.QEIM = 7;           // Quadrature mode 4 
    QEI1CONbits.PCDOUT = 1; 
    QEI1CONbits.POSRES = 0; 
    
    DFLT1CONbits.QEOUT = 0;
    
    MAX1CNT = 65535;  
}