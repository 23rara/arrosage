// ============================================================================
// CONFIGURATION GLOBALE - PINS ET CONSTANTES
// ============================================================================

#ifndef CONFIG_H
#define CONFIG_H

 

// CLAVIER MATRICIEL 4X4
#define ROWS 4
#define COLS 4
#define KEY_DEBOUNCE_MS 50
#define KEY_REPEAT_DELAY_MS 500
#define KEY_REPEAT_ACCEL_DELAY_MS 1000
#define KEY_REPEAT_INTERVAL_MS 150
#define KEY_REPEAT_ACCEL_INTERVAL_MS 50

// PINS PRINCIPAUX
#define DHTPIN 17
#define DHTTYPE DHT11
 
#define PIN_YFS201 2
#define PIN_POMPE 9
 
#define PIN_VENTILATEUR 44

// VANNES
#define VANNE_PIN_1 50
#define VANNE_PIN_2 51
#define VANNE_PIN_3 52
#define VANNE_PIN_4 53

// SONDES HUMIDITÉ
#define PIN_SONDE_1 A0
#define PIN_SONDE_2 A1
#define PIN_SONDE_3 A2
#define PIN_SONDE_4 A3

 
// EEPROM ADRESSES
#define EE_VANNE 0
#define EE_SEUIL_H 4
#define EE_TEMPS_C 8
#define EE_SEUIL_TEMP 12
#define EE_TIMEOUT_DEBIT 16
#define EE_SEUIL_DEBIT 20
#define EE_HEURE_DEBUT 24
#define EE_HEURE_FIN 26
#define EE_SONDE_SEC 28
#define EE_SONDE_MOUILLE 36
#define EE_ALARME_DESACTIV 44
#define EE_HAUTEUR_RESERVOIR 46
#define EE_NIVEAU_BAS 50
#define EE_NIVEAU_CRITIQUE 54

// CONSTANTES
#define WDT_TIMEOUT 8
#define TIMEOUT_DEBIT 5000
#define SEUIL_DEBIT_MIN 0.1
#define ALARM_IGNORE_DURATION 3000
#define DHT_READ_INTERVAL 3000
#define WATER_LEVEL_READ_INTERVAL 5000
#define PULSE_DEBOUNCE_MS 10
#define K_FACTOR 7.5
#define LCD_TIMEOUT_MS (2UL * 60UL * 1000UL)

// MENU
#define NB_OPTIONS 14

#endif
