 // ============================================================================
// IMPLEMENTATION MODULE GESTION MENUS (non-bloquant)
// ============================================================================
#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include "config.h"
#include "menu.h"
#include "affichage.h"
#include "clavier.h"
#include "vannes.h"
#include "pompe.h"
#include "capteurs.h"
#include "arrosage.h"
#include "ventilateur.h"
#include "stockage.h"

extern RTC_DS3231 rtc;
extern LiquidCrystal_I2C lcd;

EtatMenu etatMenu = ACCUEIL;
int menuSelection = 0, firstVisible = 0;
int grilleSelection = 0;
bool menuAffiche = false;
bool grilleAffichee = false;

int tempHeure = 0;
int tempMinute = 0;
int tempSeconde = 0;
int editingHour = -1;
int editingPlageHour = -1;

int etalonSondeSelection = 0;
int etalonEtat = 0;

extern void gererSSM_Vannes(char key);
extern void gererSSM_Humidite(char key);
extern void gererSSM_Calibration(char key);
extern void gererSSM_Time(char key);
extern void gererSSM_TempBoitier(char key);
extern void gererSSM_PlageHoraire(char key);
extern void gererSSM_Etalonnage(char key);
extern void gererSSM_Securite(char key);
extern void gererSSM_DesactivAlarme(char key);
extern void gererSSM_ConfigReservoir(char key);
extern void gererSSM_PompeManuel(char key);

void gererMenuAccueil(char key) {
  if(alarmeActive) {
    static bool alarmeAffichee = false;
    if(!alarmeAffichee) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("!!! ALARME !!!      ");
      lcd.setCursor(0,1);
      lcd.print(messagesErreur);
      for(int i = strlen(messagesErreur); i < 20; i++) lcd.print(" ");
      lcd.setCursor(0,2);
      lcd.print("Pompe ARRETEE       ");
      lcd.setCursor(0,3);
      lcd.print("*=Continuer         ");
      alarmeAffichee = true;
    }
    if(key == '*') {
      alarmeActive = false;
      alarmeManuelle = false;  
      pompeFonctionnelle = true;
      timeoutDebit = millis();
      timeoutAlarmeDisabled = millis();
      tempsErreur = 0;
       
      alarmeAffichee = false;
    }
     
  } else {
    menuAffiche = false;
    afficherAccueil();
    if(key == '*') {
      etatMenu = MENU_PRINCIPAL;
      menuSelection = 0;
      firstVisible = 0;
      menuAffiche = false;
    }
     
  }
}

void gererMenuPrincipal(char key) {
  if(!menuAffiche) {
    afficherMenuPrincipal();
    menuAffiche = true;
  }

   
  if(key == 'D') {
     
    if(firstVisible < 7 && menuSelection > 0) {
      menuSelection--;
      if(menuSelection < firstVisible) firstVisible = menuSelection;
      menuAffiche = false;
    }
     
    else if(firstVisible >= 7 && menuSelection > 7) {
      menuSelection--;
      if(menuSelection < firstVisible) firstVisible = menuSelection;
      menuAffiche = false;
    }
  }

  if(key == 'C') {
     
    if(firstVisible < 7 && menuSelection < 6) {
      menuSelection++;
      if(menuSelection > firstVisible + 3) firstVisible = menuSelection - 3;
      menuAffiche = false;
    }
     
    else if(firstVisible >= 7 && menuSelection < 13) {
      menuSelection++;
      if(menuSelection > firstVisible + 3) firstVisible = menuSelection - 3;
      menuAffiche = false;
    }
  }

   
  if(key == 'A') {
    if(firstVisible < 7) {
      // Page 1 -> Page 2
      firstVisible = 7;
      menuSelection = 7;
      menuAffiche = false;
    }
  }

   
  if(key == 'B') {
    if(firstVisible >= 7) {
       
      firstVisible = 0;
      menuSelection = 0;
      menuAffiche = false;
    }
  }

  if(key >= '1' && key <= '9') {
    int menuIdx = key - '1';
    if(menuIdx < NB_OPTIONS - 1) {
      menuSelection = menuIdx;
       
      if(menuIdx <= 6) firstVisible = 0;
      else firstVisible = 7;
      key = '*';
    }
  } else if(key == '0') {
    menuSelection = 9;
    firstVisible = 7;
    key = '*';
  }

  if(key == '*') {
    menuAffiche = false;
    grilleAffichee = false;
    switch(menuSelection) {
      case 0: etatMenu = SSM_VANNE_1; grilleSelection = 0; break;
      case 1: etatMenu = SSM_HUMID_1; grilleSelection = 0; break;
      case 2: etatMenu = SSM_TIME_1; grilleSelection = 0; break;
      case 3: etatMenu = SSM_TEMP_BOITIER; grilleSelection = 0; break;
      case 4: etatMenu = SSM_PLAGE_HORAIRE; editingPlageHour = 0; break;
      case 5: firstVisible = 7; menuSelection = 7; menuAffiche = false; break; // Suite -> page 2
      case 6: etatMenu = ACCUEIL; break; // Retour page 1 -> accueil
      case 7: etatMenu = SSM_CALIB_1; grilleSelection = 0; break;
      case 8: etatMenu = SSM_ETALONNAGE; etalonSondeSelection = 0; etalonEtat = 0; break;
      case 9: etatMenu = SSM_SECURITE; grilleSelection = 0; break;
      case 10: etatMenu = SSM_DESACTIV_ALARME; grilleSelection = 0; break;
      case 11: etatMenu = SSM_CONFIG_RESERVOIR; grilleSelection = 0; break;
      case 12: etatMenu = SSM_POMPE_MANUEL; grilleSelection = 0; break;
      case 13: firstVisible = 0; menuSelection = 0; menuAffiche = false; break; // Retour page 2 -> page 1
    }
  }

  if(key == '#') {
    etatMenu = ACCUEIL;
  }
}

void gererMenu() {
  char key = obtenirToucheAvecAcceleration();

  switch(etatMenu) {
    case ACCUEIL: gererMenuAccueil(key); break;
    case MENU_PRINCIPAL: gererMenuPrincipal(key); break;
    case SSM_VANNE_1: gererSSM_Vannes(key); break;
    case SSM_HUMID_1: gererSSM_Humidite(key); break;
    case SSM_CALIB_1: gererSSM_Calibration(key); break;
    case SSM_TIME_1: gererSSM_Time(key); break;
    case SSM_TEMP_BOITIER: gererSSM_TempBoitier(key); break;
    case SSM_PLAGE_HORAIRE: gererSSM_PlageHoraire(key); break;
    case SSM_ETALONNAGE: gererSSM_Etalonnage(key); break;
    case SSM_SECURITE: gererSSM_Securite(key); break;
    case SSM_DESACTIV_ALARME: gererSSM_DesactivAlarme(key); break;
    case SSM_CONFIG_RESERVOIR: gererSSM_ConfigReservoir(key); break;
    case SSM_POMPE_MANUEL: gererSSM_PompeManuel(key); break;
    default: break;
  }
}