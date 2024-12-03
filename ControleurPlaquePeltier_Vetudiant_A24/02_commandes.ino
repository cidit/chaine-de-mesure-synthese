// --- Ventilateurs ---

/*
// Compléter l'entête
int brocheF: Broche de contrôle du ventilateur froid
int brocheC: Broche de contrôle du ventilateur chaud
*/
void initialisationVentilo(int brocheF, int brocheC) {
  // À COMPLÉTER
}

/*
// Compléter l'entête
int ventilo: Ventilateur à contrôler. 0 -> ventilateur froid, 1 -> ventilateur chaud
int etat: État que le ventilateur doit prendre. 0 -> éteindre le ventilateur, 0 -> allumer le ventilateur
*/
void commandeVentilo(int ventilo, int etat) {
  // À COMPLÉTER
}

// --- Commutateur ---
// Compléter l'entête
// Initialise le commutateur (coupe-circuit).
void initialisationCommutateur(int brocheOut) {
  pinMode(brocheOut, OUTPUT);
  digitalWrite(brocheOut, etatCommutateur);
}

// Compléter l'entête
// Allume (etat=1) ou éteint (etat=0) le commutateur.
// Note: pensez à mettre tout à "OFF" lorsque le commutateur principal est mis à "OFF": ventilos, consigne
void commandeCommutateur(bool etat) {
  /// À bonifier (valider que la valeur est dans les limites acceptables)
  etatCommutateur = etat;
  digitalWrite(broche_Commutateur, etat);
}

// --- MCP4921 ---
// Compléter l'entête
bool initialisationMCP() {
  myspi->begin();
  MCP.begin(MCP_CS_PIN);    // 5 for VSPI and 15 for HSPI
  return false;  // À compléter
}

// Compléter l'entête
void commandeMCP(float pourcentage) {
  /// À bonifier (valider que la valeur est dans les limites acceptables)
  consigneCourant = pourcentage;
  MCP.setPercentage(pourcentage, 0);
}



// --- LabVIEW ---
bool lectureCmdLabview() {
  bool retValue = false;
  monDecodeur.refresh();
  if (monDecodeur.isAvailable()) {
    char commande = monDecodeur.getCommand();
    switch (commande) {
      case 'X':
        // Commande de lecture du courant [courant_en_mA]
        envoieCourant();
        break;
      case 'V':
        // Commande de lecture de la tension Vbus [tension_Vbus_en_Volts]
        envoieVoltageBus();
        break;
      case 'W':
        // Commande de lecture de la puissance [puissance_en_mW]
        envoiePuissance();
        break;
      case 'I': 
        // Commande de lecture de l’ensemble électrique :  courant, tension Vbus et puissance 
        // [shuntV; Vbus;courant; puissance;loadV] 
        envoieLectureINA219();
        break;
      case 'J': 
        //Non-implémentée à l'A24
        Serial.println(NACK);
        break;
      case 'T':
        // 0 = Commande de lecture de la température du 
        // côté froid 
        // 1 = Commande de lecture de la température du 
        // côté chaud 
        // 2 = Commande de lecture de la température du 
        // capteur ambient
        // [temperature_en_Celsius]
        envoieTemperature(int(monDecodeur.getArg(0)));
        break;
      case 'G':
        // 0 = Commande de lecture de l’état du ventilateur 
        // côté froid, réponse 0=éteint, 1=allumé. 
        // 1 = Commande de lecture de l’état du ventilateur 
        // côté chaud, réponse 0=éteint, 1=allumé. 
        // [0|1]
        envoieEtatVentilo(int(monDecodeur.getArg(0)));
        break;
      case 'D':
        // Commande de lecture de l’état du commutateur principal. Réponse 0=éteint, 1=allumé. 
        // [0|1]
        envoieEtatCommutateur();
        break;
      case 'L':
        // Commande de lecture de la consigne de courant [0-100]
        envoieConsigneCourant();
        break;
      case 'P':
        // Commande de lecture du nom du programme et sa version [nom_du_programme]
        envoieNomProgramme();
        break;
      case 'F':
        // Commande d’état du ventilateur n=0=côté froid, n=1=côté chaud Fan [n] [ON|OFF]
        controleVentilo(int(monDecodeur.getArg(0)), int(monDecodeur.getArg(1)));
        Serial.println(ACK);
        break;
      case 'M':
        // Commande pour afficher le menu des commandes (pour utilisation humaine) [menu des commandes]
        afficheMenu();
        break;
      case 'C':
        // Commande d’état du commutateur principal Buckboost [ON|OFF]
        commandeCommutateur(bool(monDecodeur.getArg(0)));
        Serial.println(ACK);
        break;
      case 'S':
        // Commande de consigne du courant, valeur en pourcentage, par saut de 1%. [0-100]
        controleMCP(monDecodeur.getArg(0));
        envoieConsigneCourant();
        break;
      case 'R':
        // Re-démarrage du ESP32 *
        ESP.restart();
        break;
      default:
        Serial.println(NACK);
        return false;
        break;
    }
    retValue = true;
  }
  return retValue;
}

void envoieEtatVentilo(int ventilo) {
  switch (ventilo) {
    case 0:
      Serial.println(etatVentiloFroid);
      break;
    case 1:
      Serial.println(etatVentiloChaud);
      break;
    default:
      Serial.println(NACK);
  }
}

void envoieEtatCommutateur() {
  Serial.println(etatCommutateur);
}

void envoieNomProgramme() {
  Serial.println("Insérer votre nom de programme ici.");  // À COMPLÉTER
}

void afficheMenu() {
  Serial.println("T [n] : lecture de la temperature 0=froid, 1=chaud, 2=ambiante");
  Serial.println("X");
  //Continuer avec la suite... pour toutes les commandes, même format que "T"
}