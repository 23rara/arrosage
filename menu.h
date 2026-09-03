// ============================================================================
// MODULE GESTION MENUS
// ============================================================================
#ifndef MENU_H
#define MENU_H

#include "config.h"

enum EtatMenu {
  ACCUEIL, MENU_PRINCIPAL,
  SSM_VANNE_1, SSM_HUMID_1, SSM_CALIB_1, SSM_TIME_1,
  SSM_TEMP_BOITIER, SSM_PLAGE_HORAIRE, SSM_ETALONNAGE, SSM_SECURITE, SSM_DESACTIV_ALARME,
  SSM_CONFIG_RESERVOIR, SSM_POMPE_MANUEL
};

extern EtatMenu etatMenu;
extern int menuSelection;
extern int firstVisible;
extern int grilleSelection;
extern bool menuAffiche;
extern bool grilleAffichee;

void gererMenu();

#endif
