//----------------------------------------------------------------------------//
// Nom du projet 		:	Reglage Encodeur
// Nom du fichier 		:   reg_V.h
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
#ifndef REG_V_H
#define	REG_V_H

//-- défintion sans valeur pour choix asservissement POSITION ou VITESSE --//
//#define POSITION 
#define POSITION_ANGULAIRE
//#define VITESSE 

//-- déclaration de définition  --//
#define VAL_MAX_CMPT_QUADRA_H       65536   // lié à la taille du registre 
                                            // du compteur du module du quadrature

#define PERIODE_ECHANTILLON_PID_MS  10                  // voir consigne correspond à 10ms 
#define SAUT_VITESSE_1MS            2621                // représente 50% du PWM (100% => 5442.88 upas) 
#define SAUT_VITESSE_100US          262                 // 
#define SAUT_POSITION               (4.0 * 512.0 * 128.0)     // 128 représente le nombre de pas du réducteur  
                                                              // 512 représente le nombre de pas du l'encodeur 
                                                              // 4 représente le nombre de flanc que l'on désire entre A et B (encodeur)  
//-- déclaration de prototype --// 
void CalculerVitesse(void); 
void CalculerErreur(void);
void CalculerPID(void); 
void RaffraichirPWM(void); 
void MAJVariables(void); 
void CalculerConsigne(void); 
void LectureImagePosition(void); 


#endif	/* REG_V_H */

