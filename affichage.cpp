// ============================================================================
// IMPLÉMENTATION MODULE AFFICHAGE LCD
// ============================================================================
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <avr/pgmspace.h>
#include "config.h"
#include "affichage.h"
#include "clavier.h"
#include "capteurs.h"
#include "vannes.h"
#include "pompe.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);
extern RTC_DS3231 rtc;

bool lcdEteint = false;
unsigned long lastActivityMillis = 0;

// Menu labels stored in PROGMEM to save SRAM
const char menu_0[] PROGMEM = "1.Regl Vannes";
const char menu_1[] PROGMEM = "2.Regl Humidite";
const char menu_2[] PROGMEM = "3.Duree Arrosage";
const char menu_3[] PROGMEM = "4.Regl.H+Temp.Boi";
const char menu_4[] PROGMEM = "5.Plage Horaire";
const char menu_5[] PROGMEM = "6.Suite";
const char menu_6[] PROGMEM = "7.Retour";
const char menu_7[] PROGMEM = "8.Calibration";
const char menu_8[] PROGMEM = "9.Etalonnage Sondes";
const char menu_9[] PROGMEM = "10.Regl Securite";
const char menu_10[] PROGMEM = "11.Desactiv Alarme";
const char menu_11[] PROGMEM = "12.Config Reservoir";
const char menu_12[] PROGMEM = "13.Demarrage Pompe";
const char menu_13[] PROGMEM = "14.Retour";
const char* const menuLabelsP[] PROGMEM = {
  menu_0, menu_1, menu_2, menu_3, menu_4, menu_5, menu_6, menu_7, menu_8, menu_9, menu_10, menu_11, menu_12, menu_13
};

const char* vanneLabels[3] = {"F", "O", "A"};

static char progBuf[21];

static void lcdPrintProgmemIndex(int idx) {
  if(idx < 0) return;
  const char* ptr = (const char*)pgm_read_word(&(menuLabelsP[idx]));
  strncpy_P(progBuf, ptr, sizeof(progBuf)-1);
  progBuf[sizeof(progBuf)-1] = '\0';
  lcd.print(progBuf);
}

void initialiserAffichage() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lastActivityMillis = millis();
}

void afficherAccueil() {
  DateTime now = rtc.now();
  char buf[21];

  if(dhtErrorFlag) {
    snprintf(buf, 21, "%02d:%02d:%02d T:ERR    ", now.hour(), now.minute(), now.second());
  } else {
    snprintf(buf, 21, "%02d:%02d:%02d T:%2dC   ", now.hour(), now.minute(), now.second(), (int)tempBoitier);
  }
  lcd.setCursor(0, 0); lcd.print(buf);

  lcd.setCursor(0, 1);
  for (int i = 0; i < 4; i++) {
    lcd.print("V"); lcd.print(i + 1); lcd.print(":");
    char st = (vanne[i] == 0) ? 'F' : (vanne[i] == 1) ? 'O' : 'A';
    lcd.print(st); if (i < 3) lcd.print(" ");
  }

  lcd.setCursor(0, 2);
  for (int i = 0; i < 4; i++) {
    snprintf(buf, 6, "%3d%%", humidite_val[i]);
    lcd.print(buf); if (i < 3) lcd.print(" ");
  }

  lcd.setCursor(0, 3);
  if(!pompeFonctionnelle) {
    snprintf(buf, 21, "POMPE ARRETEE!      ");
  } else {
    if(tauxRemplissage < 0) {
      snprintf(buf, 21, "RES:N/A Q:%.1fL/m   ", currentFlow);
    } else if(niveauCritique) {
      snprintf(buf, 21, "RES:VIDE!           ");
    } else if(niveauBas) {
      snprintf(buf, 21, "RES:BAS %d%%        ", (int)tauxRemplissage);
    } else {
      snprintf(buf, 21, "RES:%d%% Q:%.1fL/m", (int)tauxRemplissage, currentFlow);
    }
  }
  lcd.print(buf);
}

void afficherMenuPrincipal() {
  lcd.clear();

  extern int menuSelection;
  extern int firstVisible;

  for(int i = 0; i < 4; i++) {
    int optIdx = firstVisible + i;
    lcd.setCursor(0, i);
    if(optIdx < NB_OPTIONS) {
      lcd.print(menuSelection == optIdx ? ">" : " ");
      lcdPrintProgmemIndex(optIdx);
      int s = strlen(progBuf);
      for(int p = s + 2; p < 20; p++) lcd.print(" ");
    } else {
      lcd.print("                    ");
    }
  }
}

void afficherGrille(int vals[4], const char* titre, const char* valLabels[3], bool forceRefresh) {
  extern int grilleSelection;

  if(forceRefresh) {
    lcd.clear();
  }

  lcd.setCursor(0, 0);
  lcd.print(titre);

  lcd.setCursor(0, 1);
  char buf[20];
  char marker1 = (grilleSelection == 0) ? '>' : ' ';
  char marker2 = (grilleSelection == 1) ? '>' : ' ';
  char val1 = valLabels ? valLabels[vals[0]][0] : ('0' + vals[0] % 10);
  char val2 = valLabels ? valLabels[vals[1]][0] : ('0' + vals[1] % 10);
  snprintf(buf, 20, "%cV1:%c  %cV2:%c", marker1, val1, marker2, val2);
  lcd.print(buf);

  lcd.setCursor(0, 2);
  char marker3 = (grilleSelection == 2) ? '>' : ' ';
  char marker4 = (grilleSelection == 3) ? '>' : ' ';
  char val3 = valLabels ? valLabels[vals[2]][0] : ('0' + vals[2] % 10);
  char val4 = valLabels ? valLabels[vals[3]][0] : ('0' + vals[3] % 10);
  snprintf(buf, 20, "%cV3:%c  %cV4:%c", marker3, val3, marker4, val4);
  lcd.print(buf);

  lcd.setCursor(0, 3);
  if(grilleSelection == 4) lcd.print("> Retour            ");
  else lcd.print("  Retour            ");
}

void afficherGrilleValeurs(int vals[4], const char* titre, bool forceRefresh) {
  extern int grilleSelection;

  if(forceRefresh) {
    lcd.clear();
  }

  lcd.setCursor(0, 0);
  lcd.print(titre);

  lcd.setCursor(0, 1);
  char buf[20];
  char marker1 = (grilleSelection == 0) ? '>' : ' ';
  char marker2 = (grilleSelection == 1) ? '>' : ' ';
  snprintf(buf, 20, "%cV1:%3d  %cV2:%3d", marker1, vals[0], marker2, vals[1]);
  lcd.print(buf);

  lcd.setCursor(0, 2);
  char marker3 = (grilleSelection == 2) ? '>' : ' ';
  char marker4 = (grilleSelection == 3) ? '>' : ' ';
  snprintf(buf, 20, "%cV3:%3d  %cV4:%3d", marker3, vals[2], marker4, vals[3]);
  lcd.print(buf);

  lcd.setCursor(0, 3);
  if(grilleSelection == 4) lcd.print("> Retour            ");
  else lcd.print("  Retour            ");
}

void gestionVeilleLCD() {
  if (!lcdEteint && (millis() - lastActivityMillis > LCD_TIMEOUT_MS)) {
    lcd.noBacklight();
    lcdEteint = true;
  }
  if (lcdEteint && obtenirTouche() != '\0') {
    lcd.backlight();
    lcdEteint = false;
    lastActivityMillis = millis();
    lcd.clear();
    delay(500);
  }
}
