// ============================================================================
// MODULE GESTION POMPE
// ============================================================================
#ifndef POMPE_H
#define POMPE_H

#include "config.h"

extern bool pompeFonctionnelle;
extern bool alarmeActive;
extern bool alarmeDesactivee;
extern bool alarmeManuelle;
extern unsigned long timeoutDebit;
extern unsigned long timeoutAlarmeDisabled;
extern const char* messagesErreur;
extern unsigned long tempsErreur;
extern unsigned int timeoutDebitParam;
extern float seuilDebitMinParam;
extern float currentFlow;
extern float totalVolume;
extern volatile unsigned long pulseCount;

void initialiserPompe();
void arreterPompe();
void demarrerPompe();
void pulseCounter();
void calculerDebit();

#endif
