// ============================================================================
// IMPLÉMENTATION MODULE GESTION VENTILATEUR
// ============================================================================
#include <Arduino.h>
#include "config.h"
#include "ventilateur.h"
#include "capteurs.h"

float seuilTempVentilateur = 30.0;

void initialiserVentilateur() {
  pinMode(PIN_VENTILATEUR, OUTPUT);
  analogWrite(PIN_VENTILATEUR, 0);
}

void gestionVentilateur() {
  if(!dhtErrorFlag && tempBoitier >= seuilTempVentilateur) {
    analogWrite(PIN_VENTILATEUR, 255);
  } else {
    analogWrite(PIN_VENTILATEUR, 0);
  }
}
