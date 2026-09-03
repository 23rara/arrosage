 // ============================================================================
// IMPLÉMENTATION MODULE STOCKAGE EEPROM
// ============================================================================
#include <Arduino.h>
#include <EEPROM.h>
#include "config.h"
#include "stockage.h"
#include "vannes.h"
#include "ventilateur.h"
#include "capteurs.h"
#include "pompe.h"
#include "arrosage.h"

extern int seuilHumidite[4];
extern int tempsCircuit[4];
extern int heureDebut;
extern int heureFin;

 
void sauvegarderParametres() {
  for(int i=0;i<4;i++){
    EEPROM.update(EE_VANNE + i, (uint8_t)vanne[i]);
    EEPROM.update(EE_SEUIL_H + i, (uint8_t)seuilHumidite[i]);
    EEPROM.update(EE_TEMPS_C + i, (uint8_t)tempsCircuit[i]);
  }

  float curSeuilTemp; EEPROM.get(EE_SEUIL_TEMP, curSeuilTemp);
  if(curSeuilTemp != seuilTempVentilateur) EEPROM.put(EE_SEUIL_TEMP, seuilTempVentilateur);

  unsigned int curTimeout; EEPROM.get(EE_TIMEOUT_DEBIT, curTimeout);
  if(curTimeout != timeoutDebitParam) EEPROM.put(EE_TIMEOUT_DEBIT, timeoutDebitParam);

  float curSeuilDebit; EEPROM.get(EE_SEUIL_DEBIT, curSeuilDebit);
  if(curSeuilDebit != seuilDebitMinParam) EEPROM.put(EE_SEUIL_DEBIT, seuilDebitMinParam);

  EEPROM.update(EE_HEURE_DEBUT, (uint8_t)heureDebut);
  EEPROM.update(EE_HEURE_FIN, (uint8_t)heureFin);

  for(int i=0;i<4;i++){
     
    uint16_t curSec = 0;
    EEPROM.get(EE_SONDE_SEC + (i*2), curSec);
    if((int)curSec != sonde_sec[i]) {
      EEPROM.put(EE_SONDE_SEC + (i*2), (uint16_t)sonde_sec[i]);
    }

    uint16_t curM = 0;
    EEPROM.get(EE_SONDE_MOUILLE + (i*2), curM);
    if((int)curM != sonde_mouille[i]) {
      EEPROM.put(EE_SONDE_MOUILLE + (i*2), (uint16_t)sonde_mouille[i]);
    }
  }

  EEPROM.update(EE_ALARME_DESACTIV, alarmeDesactivee ? 1 : 0);

  int curHauteur=0; EEPROM.get(EE_HAUTEUR_RESERVOIR, curHauteur);
  if(curHauteur != hauteurReservoir) EEPROM.put(EE_HAUTEUR_RESERVOIR, hauteurReservoir);

  int curNBas=0; EEPROM.get(EE_NIVEAU_BAS, curNBas);
  if(curNBas != niveauBasCM) EEPROM.put(EE_NIVEAU_BAS, niveauBasCM);

  int curNCrit=0; EEPROM.get(EE_NIVEAU_CRITIQUE, curNCrit);
  if(curNCrit != niveauCritiqueCM) EEPROM.put(EE_NIVEAU_CRITIQUE, niveauCritiqueCM);
}

void chargerParametres() {
   
  for(int i=0;i<4;i++){
    vanne[i] = EEPROM.read(EE_VANNE + i);
    seuilHumidite[i] = EEPROM.read(EE_SEUIL_H + i);
    tempsCircuit[i] = EEPROM.read(EE_TEMPS_C + i);
  }

  EEPROM.get(EE_SEUIL_TEMP, seuilTempVentilateur);
  EEPROM.get(EE_TIMEOUT_DEBIT, timeoutDebitParam);
  EEPROM.get(EE_SEUIL_DEBIT, seuilDebitMinParam);

  heureDebut = EEPROM.read(EE_HEURE_DEBUT);
  heureFin = EEPROM.read(EE_HEURE_FIN);

  bool sondes_valid = true;
  for(int i=0;i<4;i++){
    uint16_t valSec = 0;
    uint16_t valMou = 0;
    EEPROM.get(EE_SONDE_SEC + (i*2), valSec);
    EEPROM.get(EE_SONDE_MOUILLE + (i*2), valMou);
    // EEPROM.get returns 0xFFFF if never written (erased flash) — détecter et marquer invalid
    if(valSec == 0xFFFF || valMou == 0xFFFF) {
      sondes_valid = false;
    } else {
       
      if(valSec <= 65535) sonde_sec[i] = (int)valSec;
      else sonde_sec[i] = 1023;
      if(valMou <= 65535) sonde_mouille[i] = (int)valMou;
      else sonde_mouille[i] = 300;
    }
  }

  if(!sondes_valid) {
    for(int i=0;i<4;i++){ sonde_sec[i] = 1023; sonde_mouille[i] = 300; }
  }

  byte alarmeStat = EEPROM.read(EE_ALARME_DESACTIV);
  alarmeDesactivee = (alarmeStat == 1);

  EEPROM.get(EE_HAUTEUR_RESERVOIR, hauteurReservoir);
  EEPROM.get(EE_NIVEAU_BAS, niveauBasCM);
  EEPROM.get(EE_NIVEAU_CRITIQUE, niveauCritiqueCM);

  
  if(timeoutDebitParam == 0 || timeoutDebitParam > 10000) timeoutDebitParam = TIMEOUT_DEBIT;
  if(seuilDebitMinParam == 0 || seuilDebitMinParam > 2.0) seuilDebitMinParam = SEUIL_DEBIT_MIN;
  if(heureDebut == 0xFF || heureDebut < 0 || heureDebut > 23) heureDebut = 6;
  if(heureFin == 0xFF || heureFin < 0 || heureFin > 23) heureFin = 18;

  if(hauteurReservoir < 5 || hauteurReservoir > 500) {
    hauteurReservoir = 100;
    niveauBasCM = 10;
    niveauCritiqueCM = 5;
  }
}