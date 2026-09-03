// ============================================================================
// MODULE CLAVIER MATRICIEL 4X4
// ============================================================================
#ifndef CLAVIER_H
#define CLAVIER_H

#include <Keypad.h>
#include "config.h"

extern unsigned long lastActivityMillis;
extern unsigned long lastKeyTime;

void initialiserClavier();
char obtenirTouche();
char obtenirToucheAvecAcceleration();
bool toucheAppuyee(char touche);

#endif
