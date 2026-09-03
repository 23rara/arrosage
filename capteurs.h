// ============================================================================
// MODULE GESTION CAPTEURS (DHT, SONDES HUMIDITÉ, NIVEAU EAU)
// ============================================================================
#ifndef CAPTEURS_H
#define CAPTEURS_H

#include "config.h"

// DHT11
extern float tempBoitier;
extern bool dhtErrorFlag;
extern unsigned long lastDHTReadTime;

// Sondes humidité
extern int humidite_val[4];
extern int sonde_brute[4];
extern int sonde_sec[4];
extern int sonde_mouille[4];
extern unsigned long lastSondeReadTime;

// Niveau d'eau (N/A dans cette version)
extern int niveauEauCM;
extern float tauxRemplissage;
extern bool niveauBas;
extern bool niveauCritique;
extern int hauteurReservoir;
extern int niveauBasCM;
extern int niveauCritiqueCM;
extern unsigned long lastWaterLevelRead;

void initialiserCapteurs();
void lireDHT();
void lireSondes();
int mesureNiveauEau();
void lireNiveauEau();

#endif
