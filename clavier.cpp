// ============================================================================
// IMPLÉMENTATION MODULE CLAVIER
// ============================================================================
#include <Arduino.h>
#include <Keypad.h>
#include "config.h"
#include "clavier.h"
#include "affichage.h"

char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {27,29,31,33};
byte colPins[COLS] = {35,37,39,41};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

unsigned long lastKeyTime = 0;
char lastKeyPressed = '\0';
unsigned long keyPressStartTime = 0;
bool keyRepeatMode = false;
bool keyAccelMode = false;
unsigned long lastRepeatTime = 0;

void initialiserClavier() { }

char obtenirTouche() {
  unsigned long now = millis();
  if(now - lastKeyTime < KEY_DEBOUNCE_MS) return '\0';
  char key = customKeypad.getKey();
  if(key) {
    lastKeyTime = now;
    lastActivityMillis = now;
    return key;
  }
  return '\0';
}

char obtenirToucheAvecAcceleration() {
  unsigned long now = millis();
  char key = customKeypad.getKey();
  if(key) {
    if(now - lastKeyTime < KEY_DEBOUNCE_MS) return '\0';
    lastKeyTime = now;
    lastActivityMillis = now;
    lastKeyPressed = key;
    keyPressStartTime = now;
    keyRepeatMode = false;
    keyAccelMode = false;
    lastRepeatTime = now;
    return key;
  }

  if(lastKeyPressed != '\0' && (lastKeyPressed == 'A' || lastKeyPressed == 'B')) {
    unsigned long pressDuration = now - keyPressStartTime;
    if(!keyRepeatMode && pressDuration >= KEY_REPEAT_DELAY_MS) {
      keyRepeatMode = true; lastRepeatTime = now; return lastKeyPressed;
    }
    if(keyRepeatMode && !keyAccelMode && pressDuration >= KEY_REPEAT_ACCEL_DELAY_MS) {
      keyAccelMode = true; lastRepeatTime = now; return lastKeyPressed;
    }
    if(keyRepeatMode) {
      unsigned long interval = keyAccelMode ? KEY_REPEAT_ACCEL_INTERVAL_MS : KEY_REPEAT_INTERVAL_MS;
      if(now - lastRepeatTime >= interval) { lastRepeatTime = now; return lastKeyPressed; }
    }
  }

  if(!customKeypad.getKey() && lastKeyPressed != '\0') {
    lastKeyPressed = '\0'; keyRepeatMode = false; keyAccelMode = false;
  }

  return '\0';
}

bool toucheAppuyee(char touche) {
  char key = obtenirTouche();
  return (key == touche);
}
