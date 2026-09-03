// ============================================================================
// MODULE GESTION VANNES
// ============================================================================
#ifndef VANNES_H
#define VANNES_H

#include "config.h"

extern int vanne[4];
extern bool arrosageEnCours[4];
extern unsigned long timeoutArrosage[4];

void initialiserVannes();
void onlyOneVanne(int idx, int v);
void fermerToutesVannes();
void ouvrirVanne(int idx);

#endif
