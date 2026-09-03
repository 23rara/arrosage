// ============================================================================
// IMPLÉMENTATION MODULE GESTION CAPTEURS
// ============================================================================
#include <Arduino.h>
#include <DHT.h>
#include "config.h"
#include "capteurs.h"

DHT dht(DHTPIN, DHTTYPE);
float tempBoitier = 23.0;
bool dhtErrorFlag = false;
unsigned long lastDHTReadTime = 0;

int humidite_val[4] = {65, 57, 83, 47};
int sonde_brute[4] = {0, 0, 0, 0};
int sonde_sec[4] = {1023, 1023, 1023, 1023};
int sonde_mouille[4] = {300, 300, 300, 300};
unsigned long lastSondeReadTime = 0;
const int SONDE_PINS[4] = {PIN_SONDE_1, PIN_SONDE_2, PIN_SONDE_3, PIN_SONDE_4};

int niveauEauCM = -1;
float tauxRemplissage = -1.0;
bool niveauBas = false;
bool niveauCritique = false;
int hauteurReservoir = 100;
int niveauBasCM = 10;
int niveauCritiqueCM = 5;
unsigned long lastWaterLevelRead = 0;

void initialiserCapteurs() {
  dht.begin();
  for(int i = 0; i < 4; i++) {
    pinMode(SONDE_PINS[i], INPUT);
  }
  lastDHTReadTime = millis();
  lastSondeReadTime = millis();
  lastWaterLevelRead = millis();
}

void lireDHT() {
  if(millis() - lastDHTReadTime < DHT_READ_INTERVAL) return;
  lastDHTReadTime = millis();
  float temp = dht.readTemperature();
  if(isnan(temp)) {
    dhtErrorFlag = true;
  } else {
    tempBoitier = temp;
    dhtErrorFlag = false;
  }
}

void lireSondes() {
  for(int i = 0; i < 4; i++) {
    sonde_brute[i] = analogRead(SONDE_PINS[i]);
    int valeur_min = sonde_sec[i];
    int valeur_max = sonde_mouille[i];
    if(valeur_max < valeur_min) {
      int tmp = valeur_min; valeur_min = valeur_max; valeur_max = tmp;
    }
    if(sonde_brute[i] <= valeur_min) {
      humidite_val[i] = 100;
    } else if(sonde_brute[i] >= valeur_max) {
      humidite_val[i] = 0;
    } else {
      humidite_val[i] = 100 - map(sonde_brute[i], valeur_min, valeur_max, 0, 100);
    }
  }
}

int mesureNiveauEau() {
  return -1;
}

void lireNiveauEau() {
  if(millis() - lastWaterLevelRead < WATER_LEVEL_READ_INTERVAL) return;
  lastWaterLevelRead = millis();
  niveauEauCM = -1;
  tauxRemplissage = -1.0;
  niveauCritique = false;
  niveauBas = false;
}
