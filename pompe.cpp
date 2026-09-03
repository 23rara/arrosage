 // ============================================================================
// IMPLÉMENTATION MODULE GESTION POMPE (avec INPUT_PULLUP sur YFS201)
// ============================================================================
#include <Arduino.h>
#include <EEPROM.h>
#include "config.h"
#include "pompe.h"
#include "vannes.h"

bool pompeFonctionnelle = true;
bool alarmeActive = false;
bool alarmeDesactivee = false;
bool alarmeManuelle = false;  
unsigned long timeoutDebit = 0;
unsigned long timeoutAlarmeDisabled = 0;
const char* messagesErreur = "";
unsigned long tempsErreur = 0;
unsigned int timeoutDebitParam = TIMEOUT_DEBIT;
float seuilDebitMinParam = SEUIL_DEBIT_MIN;

volatile unsigned long pulseCount = 0;
volatile unsigned long lastPulseTime = 0;
float currentFlow = 0.0;
float totalVolume = 0.0;
unsigned long lastFlowCalc = 0;

void initialiserPompe() {
  pinMode(PIN_POMPE, OUTPUT);
  digitalWrite(PIN_POMPE, HIGH);
   
  pinMode(PIN_YFS201, INPUT_PULLUP);
  int irq = digitalPinToInterrupt(PIN_YFS201);
#ifdef DEBUG
  if(irq == NOT_AN_INTERRUPT) {
    Serial.print("ERROR: PIN_YFS201=");
    Serial.print(PIN_YFS201);
    Serial.println(" is not an interrupt pin on this board!");
  } else {
    Serial.print("Attaching interrupt on pin ");
    Serial.println(PIN_YFS201);
  }
#endif
  if(irq != NOT_AN_INTERRUPT) {
    attachInterrupt(irq, pulseCounter, RISING);
  }
  timeoutDebit = millis();
  lastFlowCalc = millis();
  alarmeManuelle = false;  
}

void arreterPompe() {
  digitalWrite(PIN_POMPE, HIGH);
  pompeFonctionnelle = false;
  fermerToutesVannes();
}


void demarrerPompe() {
   
  if (!pompeFonctionnelle || alarmeActive) {
    return; 
  }
  digitalWrite(PIN_POMPE, LOW);
   
}

void pulseCounter() {
  
  pulseCount++;
  lastPulseTime = micros();
}

void reinitialiserSecuritePompe() {
  alarmeActive = false;
  alarmeManuelle = false;  
  pompeFonctionnelle = true;
  messagesErreur = "";
  timeoutDebit = millis();  
}

void calculerDebit() {
  if(millis() - lastFlowCalc <= 1000) return;

  noInterrupts();
  unsigned long pulses = pulseCount;
  pulseCount = 0;
  interrupts();

   
  if(pulses > 0) {
    timeoutDebit = millis();
  }

  currentFlow = ((float)pulses) / K_FACTOR;
  totalVolume += ((float)pulses) / (K_FACTOR * 60.0);

  unsigned long elapsed = millis() - timeoutDebit;

  if(!alarmeDesactivee && !alarmeActive && !alarmeManuelle && (millis() - timeoutAlarmeDisabled > ALARM_IGNORE_DURATION)) {
    if(elapsed > timeoutDebitParam) {
      pompeFonctionnelle = false;
      alarmeActive = true;
      alarmeManuelle = true;  
      messagesErreur = "ERR: Pression nulle";
      tempsErreur = millis();
      arreterPompe();
    } else if(currentFlow < seuilDebitMinParam) {
      pompeFonctionnelle = false;
      alarmeActive = true;
      alarmeManuelle = true;  
      messagesErreur = "ERR: Debit faible";
      tempsErreur = millis();
      digitalWrite(PIN_POMPE, HIGH);
    } else {
      pompeFonctionnelle = true;
    }
  }

  lastFlowCalc = millis();
}
