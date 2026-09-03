// ============================================================================
// IMPLÉMENTATION MODULE GESTION ARROSAGE
// ============================================================================
#include <Arduino.h>
#include <RTClib.h>
#include "config.h"
#include "arrosage.h"
#include "vannes.h"
#include "pompe.h"
#include "capteurs.h"

extern RTC_DS3231 rtc;

int seuilHumidite[4] = {40,45,50,55};
int tempsCircuit[4] = {30,30,30,30};
int heureDebut = 6;
int heureFin = 18;

 void gestionArrosageAutomatique() {
   
  if (!pompeFonctionnelle || alarmeActive) {
    return;  
  }

  DateTime now = rtc.now();
  int heure_actuelle = now.hour();
  bool plageHoraireActive = (heure_actuelle >= heureDebut && heure_actuelle < heureFin);
  if(!plageHoraireActive) return;

  for(int i=0; i<4; i++){
    if(humidite_val[i] < seuilHumidite[i] && !arrosageEnCours[i]) {
      vanne[i] = 1;
      onlyOneVanne(i, 1);
      arrosageEnCours[i] = true;
      timeoutArrosage[i] = millis();
      demarrerPompe();
    }
     
    
    
    
    
    if(arrosageEnCours[i]) {
      unsigned long tempsEcoule = millis() - timeoutArrosage[i];
      if(tempsEcoule > (unsigned long)tempsCircuit[i] * 1000) {
        vanne[i] = 0;
        onlyOneVanne(i,0);
        arrosageEnCours[i] = false;
        bool pompeNecessaire = false;
        for(int j=0;j<4;j++) if(arrosageEnCours[j]) pompeNecessaire = true;
        if(!pompeNecessaire) digitalWrite(PIN_POMPE, HIGH);
      }
    }
  }
}
