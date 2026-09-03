 // ============================================================================
// PROGRAMME PRINCIPAL - SKETCH ARDUINO (optimisé, v2) - non-bloquant
// ============================================================================
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <DHT.h>
#include <EEPROM.h>
#include <avr/wdt.h>
#include <Keypad.h>

#include "config.h"
#include "clavier.h"
#include "vannes.h"
#include "pompe.h"
#include "capteurs.h"
#include "ventilateur.h"
#include "affichage.h"
#include "arrosage.h"
#include "stockage.h"
#include "menu.h"

RTC_DS3231 rtc;

void resetWatchdog() {
  wdt_reset();
}

void setup() {
  Wire.begin();
#ifdef DEBUG
  Serial.begin(9600);
  Serial.println("Setup start");
#endif

  initialiserAffichage();
  rtc.begin();

  initialiserClavier();
  initialiserCapteurs();
  initialiserVannes();
  initialiserPompe();
  initialiserVentilateur();
  chargerParametres();

  wdt_enable(WDTO_8S);

  lastActivityMillis = millis();

#ifdef DEBUG
  Serial.println("Setup done");
#endif
}

void loop() {
  static unsigned long lastLoopMillis = 0;
  const unsigned long LOOP_INTERVAL_MS = 40;

  resetWatchdog();

   
  unsigned long now = millis();
  if(now - lastLoopMillis < LOOP_INTERVAL_MS) {
     
    return;
  }
  lastLoopMillis = now;

  lireDHT();
  lireSondes();
  lireNiveauEau();  
  gestionVentilateur();
  calculerDebit();
  gestionArrosageAutomatique();
  gestionVeilleLCD();

  if(lcdEteint) {
     
    return;
  }

  gererMenu();
}