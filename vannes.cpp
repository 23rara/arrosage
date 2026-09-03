 
#include <Arduino.h>
#include <EEPROM.h>
#include "config.h"
#include "vannes.h"
#include "stockage.h"

const int VANNE_PINS[4] = {VANNE_PIN_1, VANNE_PIN_2, VANNE_PIN_3, VANNE_PIN_4};

int vanne[4] = {0,0,0,0};
bool arrosageEnCours[4] = {false,false,false,false};
unsigned long timeoutArrosage[4] = {0,0,0,0};

void initialiserVannes() {
  for(int i=0;i<4;i++){
    pinMode(VANNE_PINS[i], OUTPUT);
    digitalWrite(VANNE_PINS[i], HIGH);
  }
  chargerParametres();
}

void onlyOneVanne(int idx, int v) {
  if(v==1){
    for(int j=0;j<4;j++) if(j!=idx) vanne[j]=0;
  }
  for(int i=0;i<4;i++){
    if(vanne[i]==1) digitalWrite(VANNE_PINS[i], LOW);
    else digitalWrite(VANNE_PINS[i], HIGH);
  }
  sauvegarderParametres();
}

void fermerToutesVannes(){
  for(int i=0;i<4;i++){
    vanne[i]=0;
    digitalWrite(VANNE_PINS[i], HIGH);
  }
  sauvegarderParametres();
}

void ouvrirVanne(int idx){
  if(idx>=0 && idx<4) onlyOneVanne(idx,1);
}
