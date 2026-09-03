// ============================================================================
// MODULE GESTION ARROSAGE AUTOMATIQUE
// ============================================================================
#ifndef ARROSAGE_H
#define ARROSAGE_H

#include "config.h"

extern int seuilHumidite[4];
extern int tempsCircuit[4];
extern int heureDebut;
extern int heureFin;

void gestionArrosageAutomatique();

#endif
