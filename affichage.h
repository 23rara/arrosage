// ============================================================================
// MODULE AFFICHAGE LCD
// ============================================================================
#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include <LiquidCrystal_I2C.h>
#include "config.h"

extern bool lcdEteint;
extern unsigned long lastActivityMillis;
extern LiquidCrystal_I2C lcd;
extern const char* vanneLabels[3];

void initialiserAffichage();
void afficherAccueil();
void afficherMenuPrincipal();
void afficherGrille(int vals[4], const char* titre, const char* valLabels[3], bool forceRefresh);
void afficherGrilleValeurs(int vals[4], const char* titre, bool forceRefresh);
void gestionVeilleLCD();

#endif
