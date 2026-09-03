 // ============================================================================
// IMPLÉMENTATION DES SOUS-MENUS
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
extern int grilleSelection;
extern int tempHeure;
extern int tempMinute;
extern int tempSeconde;
extern int editingHour;
extern int editingPlageHour;
extern int etalonSondeSelection;
extern int etalonEtat;
extern EtatMenu etatMenu;
extern int menuSelection;
extern int firstVisible;

// ============================================================================
// SSM_VANNE_1 - Réglage des Vannes
// ============================================================================
void gererSSM_Vannes(char key) {
  afficherGrille(vanne, "Vannes", vanneLabels, !grilleAffichee);
  if(!grilleAffichee) grilleAffichee = true;

   
  if(key == 'D' && grilleSelection > 0) {
    grilleSelection--;
  }
  if(key == 'C' && grilleSelection < 4) {
    grilleSelection++;
  }

   
  if(key == 'A' && grilleSelection < 4) {
    vanne[grilleSelection]++;
    if(vanne[grilleSelection] > 2) vanne[grilleSelection] = 0;
    onlyOneVanne(grilleSelection, vanne[grilleSelection]);
  }
  if(key == 'B' && grilleSelection < 4) {
    vanne[grilleSelection]--;
    if(vanne[grilleSelection] < 0) vanne[grilleSelection] = 2;
    onlyOneVanne(grilleSelection, vanne[grilleSelection]);
  }
  
  if(key == '*') {
    if(grilleSelection == 4) {
      etatMenu = MENU_PRINCIPAL;
      menuSelection = 0;
      firstVisible = 0;
      grilleSelection = 0;
      grilleAffichee = false;
    }
    delay(100);
  }
  
  if(key == '#') {
    etatMenu = MENU_PRINCIPAL;
    menuSelection = 0;
    grilleSelection = 0;
    grilleAffichee = false;
  }
  
  delay(100);
}

// ============================================================================
// SSM_HUMID_1 - Réglage Humidité
// ============================================================================
void gererSSM_Humidite(char key) {
  afficherGrilleValeurs(seuilHumidite, "Seuil H(%)", !grilleAffichee);
  if(!grilleAffichee) grilleAffichee = true;

   
  if(key == 'D' && grilleSelection > 0) {
    grilleSelection--;
  }
  if(key == 'C' && grilleSelection < 4) {
    grilleSelection++;
  }

   
  if(key == 'A' && grilleSelection < 4) {
    seuilHumidite[grilleSelection]++;
    if(seuilHumidite[grilleSelection] > 100) seuilHumidite[grilleSelection] = 0;
    sauvegarderParametres();
  }

  if(key == 'B' && grilleSelection < 4) {
    seuilHumidite[grilleSelection]--;
    if(seuilHumidite[grilleSelection] < 0) seuilHumidite[grilleSelection] = 100;
    sauvegarderParametres();
  }
  
  if(key == '*') {
    if(grilleSelection == 4) {
      etatMenu = MENU_PRINCIPAL;
      menuSelection = 1;
      firstVisible = 0;
      grilleSelection = 0;
      grilleAffichee = false;
    }
  }
  
  if(key == '#') {
    etatMenu = MENU_PRINCIPAL;
    menuSelection = 1;
    grilleSelection = 0;
    grilleAffichee = false;
  }
  
  delay(50);
}

// ============================================================================
// SSM_CALIB_1 - Calibration Débitmètre
// ============================================================================
void gererSSM_Calibration(char key) {
  if(!grilleAffichee) {
    lcd.clear();
  }

  lcd.setCursor(0, 0);
  lcd.print("Calibration");

  lcd.setCursor(0, 1);
  char marker1 = (grilleSelection == 0) ? '>' : ' ';
  lcd.print(marker1);
  lcd.print("Reset Volume");

  lcd.setCursor(0, 2);
  char marker2 = (grilleSelection == 1) ? '>' : ' ';
  lcd.print(marker2);
  lcd.print("Reset Pulses");

  lcd.setCursor(0, 3);
  char marker3 = (grilleSelection == 2) ? '>' : ' ';
  lcd.print(marker3);
  lcd.print("Retour");

  if(!grilleAffichee) grilleAffichee = true;

   
  if(key == 'D' && grilleSelection > 0) {
    grilleSelection--;
  }
  if(key == 'C' && grilleSelection < 2) {
    grilleSelection++;
  }
  
  if(key == '*') {
    if(grilleSelection == 0) {
      totalVolume = 0.0;
      lcd.setCursor(0, 1);
      lcd.print(">Volume reset OK    ");
      delay(1000);
    } else if(grilleSelection == 1) {
      noInterrupts();
      pulseCount = 0;
      interrupts();
      currentFlow = 0.0;
      lcd.setCursor(0, 2);
      lcd.print(">Pulses reset OK     ");
      delay(1000);
    } else if(grilleSelection == 2) {
      etatMenu = MENU_PRINCIPAL;
      menuSelection = 2;
      firstVisible = 0;
      grilleSelection = 0;
      grilleAffichee = false;
    }
  }
  
  if(key == '#') {
    etatMenu = MENU_PRINCIPAL;
    menuSelection = 2;
    grilleSelection = 0;
    grilleAffichee = false;
  }
  
  delay(100);
}

// ============================================================================
// SSM_TIME_1 - Durée d'Arrosage
// ============================================================================
void gererSSM_Time(char key) {
  afficherGrilleValeurs(tempsCircuit, "Temps Fct.", !grilleAffichee);
  if(!grilleAffichee) grilleAffichee = true;

   
  if(key == 'D' && grilleSelection > 0) {
    grilleSelection--;
  }
  if(key == 'C' && grilleSelection < 4) {
    grilleSelection++;
  }

   
  if(key == 'A' && grilleSelection < 4) {
    tempsCircuit[grilleSelection]++;
    if(tempsCircuit[grilleSelection] > 120) tempsCircuit[grilleSelection] = 0;
    sauvegarderParametres();
  }

  if(key == 'B' && grilleSelection < 4) {
    tempsCircuit[grilleSelection]--;
    if(tempsCircuit[grilleSelection] < 0) tempsCircuit[grilleSelection] = 120;
    sauvegarderParametres();
  }
  
  if(key == '*') {
    if(grilleSelection == 4) {
      etatMenu = MENU_PRINCIPAL;
      menuSelection = 3;
      firstVisible = 0;
      grilleSelection = 0;
      grilleAffichee = false;
    }
  }
  
  if(key == '#') {
    etatMenu = MENU_PRINCIPAL;
    menuSelection = 3;
    grilleSelection = 0;
    grilleAffichee = false;
  }
  
  delay(50);
}

// ============================================================================
// SSM_TEMP_BOITIER - Température Boîtier + Heure RTC
// ============================================================================
void gererSSM_TempBoitier(char key) {
  if(!grilleAffichee) {
    lcd.clear();
    grilleAffichee = true;
  }
  
  if(editingHour >= 0 && editingHour <= 1) {
     
    lcd.setCursor(0, 0);
    char buf[20];
    snprintf(buf, 20, "Regl Heure          ");
    lcd.print(buf);
    
    lcd.setCursor(0, 1);
    char markerH = (editingHour == 0) ? '>' : ' ';
    char markerM = (editingHour == 1) ? '>' : ' ';
    snprintf(buf, 20, "%c%02d:%c%02d           ", markerH, tempHeure, markerM, tempMinute);
    lcd.print(buf);
    
    lcd.setCursor(0, 2);
    snprintf(buf, 20, "                    ");
    lcd.print(buf);
    
    lcd.setCursor(0, 3);
    snprintf(buf, 20, "*=OK  #=Retour      ");
    lcd.print(buf);
    
    if(key == 'A') {
      if(editingHour == 0) {
        tempHeure++;
        if(tempHeure > 23) tempHeure = 0;
      } else if(editingHour == 1) {
        tempMinute++;
        if(tempMinute > 59) tempMinute = 0;
      }
    }
    
    if(key == 'B') {
      if(editingHour == 0) {
        tempHeure--;
        if(tempHeure < 0) tempHeure = 23;
      } else if(editingHour == 1) {
        tempMinute--;
        if(tempMinute < 0) tempMinute = 59;
      }
    }
    
    if(key == '*') {
      if(editingHour < 1) {
        editingHour++;
      } else {
        rtc.adjust(DateTime(2024, 1, 1, tempHeure, tempMinute, 0));
        editingHour = -1;
        grilleSelection = 1;
        grilleAffichee = false;
      }
      delay(200);
    }
    
    if(key == '#') {
      editingHour = -1;
      grilleSelection = 0;
      grilleAffichee = false;
    }
    
    delay(100);
  }
  else {
     
    lcd.setCursor(0, 0);
    char buf[20];
    snprintf(buf, 20, "Temp. Boitier       ");
    lcd.print(buf);
    
    lcd.setCursor(0, 1);
    if(dhtErrorFlag) {
      snprintf(buf, 20, "Act: ERR C           ");
    } else {
      snprintf(buf, 20, "Act: %d C            ", (int)tempBoitier);
    }
    lcd.print(buf);
    
    lcd.setCursor(0, 2);
    char marker = (grilleSelection == 0) ? '>' : ' ';
    snprintf(buf, 20, "%cSeuil: %d C         ", marker, (int)seuilTempVentilateur);
    lcd.print(buf);
    
    lcd.setCursor(0, 3);
    if(grilleSelection == 1) {
      snprintf(buf, 20, ">Regl Heure         ");
    } else if(grilleSelection == 2) {
      snprintf(buf, 20, ">Retour             ");
    } else {
      snprintf(buf, 20, "  Retour            ");
    }
    lcd.print(buf);

     
    if(key == 'D' && grilleSelection > 0) {
      grilleSelection--;
    }
    if(key == 'C' && grilleSelection < 2) {
      grilleSelection++;
    }
    
    if(key == 'A' && grilleSelection == 0) {
      seuilTempVentilateur++;
      if(seuilTempVentilateur > 60) seuilTempVentilateur = 20;
      sauvegarderParametres();
    }
    
    if(key == 'B' && grilleSelection == 0) {
      seuilTempVentilateur--;
      if(seuilTempVentilateur < 20) seuilTempVentilateur = 60;
      sauvegarderParametres();
    }
    
    if(key == '*') {
      if(grilleSelection == 1) {
        DateTime now = rtc.now();
        tempHeure = now.hour();
        tempMinute = now.minute();
        tempSeconde = now.second();
        editingHour = 0;
        grilleAffichee = false;
      } else if(grilleSelection == 2) {
        etatMenu = MENU_PRINCIPAL;
        menuSelection = 4;
        firstVisible = 0;
        grilleSelection = 0;
        grilleAffichee = false;
      }
    }
    
    if(key == '#') {
      etatMenu = MENU_PRINCIPAL;
      menuSelection = 4;
      grilleSelection = 0;
      grilleAffichee = false;
    }
    
    delay(50);
  }
}

// ============================================================================
// SSM_PLAGE_HORAIRE - Plage d'Arrosage
// ============================================================================
void gererSSM_PlageHoraire(char key) {
  if(!grilleAffichee) {
    lcd.clear();
    grilleAffichee = true;
  }
  
  if(editingPlageHour >= 0 && editingPlageHour <= 1) {
    lcd.setCursor(0, 0);
    char buf[20];
    snprintf(buf, 20, "Plage Arrosage      ");
    lcd.print(buf);
    
    lcd.setCursor(0, 1);
    char markerD = (editingPlageHour == 0) ? '>' : ' ';
    char markerF = (editingPlageHour == 1) ? '>' : ' ';
    snprintf(buf, 20, "%cDebut:%02d %cFin:%02d", markerD, heureDebut, markerF, heureFin);
    lcd.print(buf);
    
    lcd.setCursor(0, 2);
    snprintf(buf, 20, "                    ");
    lcd.print(buf);
    
    lcd.setCursor(0, 3);
    snprintf(buf, 20, "*=OK    #=Retour    ");
    lcd.print(buf);
    
    if(key == 'A') {
      if(editingPlageHour == 0) {
        heureDebut++;
        if(heureDebut > 23) heureDebut = 0;
      } else if(editingPlageHour == 1) {
        heureFin++;
        if(heureFin > 23) heureFin = 0;
      }
    }
    
    if(key == 'B') {
      if(editingPlageHour == 0) {
        heureDebut--;
        if(heureDebut < 0) heureDebut = 23;
      } else if(editingPlageHour == 1) {
        heureFin--;
        if(heureFin < 0) heureFin = 23;
      }
    }
    
    if(key == '*') {
      if(editingPlageHour < 1) {
        editingPlageHour++;
      } else {
        sauvegarderParametres();
        editingPlageHour = -1;
        etatMenu = MENU_PRINCIPAL;
        menuSelection = 5;
        firstVisible = 0;
        grilleSelection = 0;
        grilleAffichee = false;
      }
      delay(200);
    }
    
    if(key == '#') {
      editingPlageHour = -1;
      etatMenu = MENU_PRINCIPAL;
      menuSelection = 5;
      grilleSelection = 0;
      grilleAffichee = false;
    }
    
    delay(100);
  }
}

// ============================================================================
// SSM_ETALONNAGE - Étalonnage des Sondes
// ============================================================================
void gererSSM_Etalonnage(char key) {
  if(!grilleAffichee) {
    lcd.clear();
    grilleAffichee = true;
  }
  
  if(etalonEtat == 0) {
    // Sélection de la sonde
    lcd.setCursor(0, 0);
    char marker1 = (etalonSondeSelection == 0) ? '>' : ' ';
    char marker2 = (etalonSondeSelection == 1) ? '>' : ' ';
    char buf[20];
    snprintf(buf, 20, "%cSonde1  %cSonde2", marker1, marker2);
    lcd.print(buf);
    
    lcd.setCursor(0, 1);
    char marker3 = (etalonSondeSelection == 2) ? '>' : ' ';
    char marker4 = (etalonSondeSelection == 3) ? '>' : ' ';
    snprintf(buf, 20, "%cSonde3  %cSonde4", marker3, marker4);
    lcd.print(buf);
    
    lcd.setCursor(0, 2);
    lcd.print("                    ");
    
    lcd.setCursor(0, 3);
    lcd.print("*=Valider #=Retour  ");
    
    if(key == 'D' && etalonSondeSelection > 1) { 
      etalonSondeSelection -= 2; 
    }
    if(key == 'C' && etalonSondeSelection < 2) { 
      etalonSondeSelection += 2; 
    }
    if(key == 'A' && etalonSondeSelection < 3) { 
      etalonSondeSelection++; 
    }
    
    if(key == '*') {
      etalonEtat = 1;
      delay(200);
    }

    if(key == '#') {
      etatMenu = MENU_PRINCIPAL;
      menuSelection = 6;
      grilleSelection = 0;
      grilleAffichee = false;
    }
    
    delay(50);
  }
  else if(etalonEtat == 1) {
    // Mesure SEC
    lcd.setCursor(0, 0);
    char buf[20];
    snprintf(buf, 20, "Sonde %d - SEC      ", etalonSondeSelection + 1);
    lcd.print(buf);
    
    lcd.setCursor(0, 1);
    snprintf(buf, 20, "Valeur: %4d        ", sonde_brute[etalonSondeSelection]);
    lcd.print(buf);
    
    lcd.setCursor(0, 2);
    lcd.print("Mettez sonde SECHE  ");
    
    lcd.setCursor(0, 3);
    lcd.print("*=Valider           ");
    
    if(key == '*') {
      sonde_sec[etalonSondeSelection] = sonde_brute[etalonSondeSelection];
      etalonEtat = 2;
      delay(500);
    }
    
    delay(50);
  }
  else if(etalonEtat == 2) {
    // Mesure MOUILLE
    lcd.setCursor(0, 0);
    char buf[20];
    snprintf(buf, 20, "Sonde %d - MOUILLE  ", etalonSondeSelection + 1);
    lcd.print(buf);
    
    lcd.setCursor(0, 1);
    snprintf(buf, 20, "Valeur: %4d        ", sonde_brute[etalonSondeSelection]);
    lcd.print(buf);
    
    lcd.setCursor(0, 2);
    lcd.print("Mettez sonde MOUILLE");
    
    lcd.setCursor(0, 3);
    lcd.print("*=Valider           ");
    
    if(key == '*') {
      sonde_mouille[etalonSondeSelection] = sonde_brute[etalonSondeSelection];
      sauvegarderParametres();
      etalonEtat = 0;
      etalonSondeSelection = 0;
      etatMenu = MENU_PRINCIPAL;
      menuSelection = 6;
      firstVisible = 0;
      grilleAffichee = false;
      delay(500);
    }
    
    delay(50);
  }
}

// ============================================================================
// SSM_SECURITE - Sécurité Pompe
// ============================================================================
void gererSSM_Securite(char key) {
  if(!grilleAffichee) {
    lcd.clear();
    grilleAffichee = true;
  }
  
  lcd.setCursor(0, 0);
  lcd.print("Securite Pompe      ");
  
  lcd.setCursor(0, 1);
  char marker1 = (grilleSelection == 0) ? '>' : ' ';
  char buf[20];
  snprintf(buf, 20, "%cTimeout: %d ms  ", marker1, timeoutDebitParam);
  lcd.print(buf);
  
  lcd.setCursor(0, 2);
  char marker2 = (grilleSelection == 1) ? '>' : ' ';
  snprintf(buf, 20, "%cSeuil Q: %.1f L/m", marker2, seuilDebitMinParam);
  lcd.print(buf);
  
  lcd.setCursor(0, 3);
  if(grilleSelection == 2) {
    lcd.print(">Retour             ");
  } else {
    lcd.print("  Retour            ");
  }

   
  if(key == 'D' && grilleSelection > 0) {
    grilleSelection--;
  }
  if(key == 'C' && grilleSelection < 2) {
    grilleSelection++;
  }

   
  if(key == 'A' && grilleSelection == 0) {
    timeoutDebitParam += 500;
    if(timeoutDebitParam > 10000) timeoutDebitParam = 1000;
    sauvegarderParametres();
  }

  if(key == 'B' && grilleSelection == 0) {
    if(timeoutDebitParam >= 1500) {
      timeoutDebitParam -= 500;
    } else {
      timeoutDebitParam = 10000;
    }
    sauvegarderParametres();
  }

  if(key == 'A' && grilleSelection == 1) {
    seuilDebitMinParam += 0.1;
    if(seuilDebitMinParam > 2.0) seuilDebitMinParam = 0.1;
    sauvegarderParametres();
  }

  if(key == 'B' && grilleSelection == 1) {
    if(seuilDebitMinParam >= 0.2) {
      seuilDebitMinParam -= 0.1;
    } else {
      seuilDebitMinParam = 2.0;
    }
    sauvegarderParametres();
  }
  
  if(key == '*') {
    if(grilleSelection == 2) {
      etatMenu = MENU_PRINCIPAL;
      menuSelection = 7;
      firstVisible = 0;
      grilleSelection = 0;
      grilleAffichee = false;
    }
  }
  
  if(key == '#') {
    etatMenu = MENU_PRINCIPAL;
    menuSelection = 7;
    grilleSelection = 0;
    grilleAffichee = false;
  }
  
  delay(50);
}

// ============================================================================
// SSM_DESACTIV_ALARME - Désactiver l'Alarme
// ============================================================================
void gererSSM_DesactivAlarme(char key) {
  if(!grilleAffichee) {
    lcd.clear();
    grilleAffichee = true;
  }
  
  lcd.setCursor(0, 0);
  lcd.print("Desactiver Alarme   ");
  
  lcd.setCursor(0, 1);
  char marker1 = (grilleSelection == 0) ? '>' : ' ';
  char buf[20];
  if(alarmeDesactivee) {
    snprintf(buf, 20, "%cAlarm: DESACTIV", marker1);
  } else {
    snprintf(buf, 20, "%cAlarm: ACTIV", marker1);
  }
  lcd.print(buf);
  
  lcd.setCursor(0, 2);
  char marker2 = (grilleSelection == 1) ? '>' : ' ';
  snprintf(buf, 20, "%cRetour          ", marker2);
  lcd.print(buf);
  
  lcd.setCursor(0, 3);
  lcd.print("                    ");

   
  if(key == 'D' && grilleSelection > 0) {
    grilleSelection--;
  }
  if(key == 'C' && grilleSelection < 1) {
    grilleSelection++;
  }
  
  if(key == '*') {
    if(grilleSelection == 0) {
      alarmeDesactivee = !alarmeDesactivee;
      if(alarmeDesactivee) {
        alarmeActive = false;
      }
      sauvegarderParametres();
      delay(300);
    } else if(grilleSelection == 1) {
      etatMenu = MENU_PRINCIPAL;
      menuSelection = 8;
      firstVisible = 5;
      grilleSelection = 0;
      grilleAffichee = false;
    }
  }
  
  if(key == '#') {
    etatMenu = MENU_PRINCIPAL;
    menuSelection = 8;
    grilleSelection = 0;
    grilleAffichee = false;
  }
  
  delay(100);
}

// ============================================================================
// SSM_CONFIG_RESERVOIR - Configuration Réservoir
// ============================================================================
void gererSSM_ConfigReservoir(char key) {
  if(!grilleAffichee) {
    lcd.clear();
    grilleAffichee = true;
  }
  
  lcd.setCursor(0, 0);
  lcd.print("Config Reservoir    ");
  
   
  lcd.setCursor(0, 1);
  char marker1 = (grilleSelection == 0) ? '>' : ' ';
  char buf[20];
  snprintf(buf, 20, "%cHauteur: %d cm      ", marker1, hauteurReservoir);
  lcd.print(buf);
  
   
  lcd.setCursor(0, 2);
  char marker2 = (grilleSelection == 1) ? '>' : ' ';
  snprintf(buf, 20, "%cNiv Bas: %d cm      ", marker2, niveauBasCM);
  lcd.print(buf);
  
   
  lcd.setCursor(0, 3);
  if(grilleSelection == 2) {
    snprintf(buf, 20, ">Niv Crit: %d cm    ", niveauCritiqueCM);
  } else if (grilleSelection == 3) {
    snprintf(buf, 20, ">Retour             ");
  } else {
    snprintf(buf, 20, "  Retour            ");
  }
  lcd.print(buf);

   
  if(key == 'D' && grilleSelection > 0) {
    grilleSelection--;
  }
  if(key == 'C' && grilleSelection < 3) {
    grilleSelection++;
  }

   
  if(key == 'A' && grilleSelection == 0) {
    hauteurReservoir += 10;
    if(hauteurReservoir > 500) hauteurReservoir = 5;
    sauvegarderParametres();
  }

  if(key == 'B' && grilleSelection == 0) {
    hauteurReservoir -= 10;
    if(hauteurReservoir < 5) hauteurReservoir = 500;
    sauvegarderParametres();
  }

  if(key == 'A' && grilleSelection == 1) {
    niveauBasCM++;
    if(niveauBasCM > 100) niveauBasCM = 5;
    sauvegarderParametres();
  }

  if(key == 'B' && grilleSelection == 1) {
    niveauBasCM--;
    if(niveauBasCM < 5) niveauBasCM = 100;
    sauvegarderParametres();
  }

  if(key == 'A' && grilleSelection == 2) {
    niveauCritiqueCM++;
    if(niveauCritiqueCM > niveauBasCM) niveauCritiqueCM = 1;
    sauvegarderParametres();
  }

  if(key == 'B' && grilleSelection == 2) {
    niveauCritiqueCM--;
    if(niveauCritiqueCM < 1) niveauCritiqueCM = niveauBasCM;
    sauvegarderParametres();
  }
  
  if(key == '*') {
    if(grilleSelection == 3) {
      etatMenu = MENU_PRINCIPAL;
      menuSelection = 9;
      firstVisible = 5;
      grilleSelection = 0;
      grilleAffichee = false;
    }
  }
  
  if(key == '#') {
    etatMenu = MENU_PRINCIPAL;
    menuSelection = 9;
    grilleSelection = 0;
    grilleAffichee = false;
  }
  
  delay(50);
}

// ============================================================================
// SSM_POMPE_MANUEL - Démarrage Manuel Pompe (Mode Test)
// ============================================================================
void gererSSM_PompeManuel(char key) {
  if(!grilleAffichee) {
    lcd.clear();
    grilleAffichee = true;
  }
  
  lcd.setCursor(0, 0);
  lcd.print("TEST RELAIS POMPE   ");
  
  lcd.setCursor(0, 1);
  char marker1 = (grilleSelection == 0) ? '>' : ' ';
  char buf[20];
  if(digitalRead(PIN_POMPE) == LOW) {
    snprintf(buf, 20, "%cRelais: ON          ", marker1);
  } else {
    snprintf(buf, 20, "%cRelais: OFF         ", marker1);
  }
  lcd.print(buf);
  
  lcd.setCursor(0, 2);
  char marker2 = (grilleSelection == 1) ? '>' : ' ';
  snprintf(buf, 20, "%cBasculer Relais     ", marker2);
  lcd.print(buf);
  
  lcd.setCursor(0, 3);
  if(grilleSelection == 2) {
    lcd.print(">Retour             ");
  } else {
    lcd.print("  Retour            ");
  }

   
  if(key == 'D' && grilleSelection > 0) {
    grilleSelection--;
  }
  if(key == 'C' && grilleSelection < 2) {
    grilleSelection++;
  }
  
  if(key == '*') {
    if(grilleSelection == 1) {
       
      if(digitalRead(PIN_POMPE) == HIGH) {
        digitalWrite(PIN_POMPE, LOW);
      } else {
        digitalWrite(PIN_POMPE, HIGH);
      }
    } else if(grilleSelection == 2) {
       
      digitalWrite(PIN_POMPE, HIGH);
      etatMenu = MENU_PRINCIPAL;
      menuSelection = 10;
      firstVisible = 6;
      grilleSelection = 0;
      grilleAffichee = false;
    }
  }
  
  if(key == '#') {
     
    digitalWrite(PIN_POMPE, HIGH);
    etatMenu = MENU_PRINCIPAL;
    menuSelection = 10;
    grilleSelection = 0;
    grilleAffichee = false;
  }
  
  delay(100);
}