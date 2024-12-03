// --- ADS1115 ---

// Initialise l'ADS1115
// Compléter l'entête
bool initialisationADS1115(int gain = 0) {
  if (!ADS.begin()) {
    return false;
  } 
  if (!ADS.isConnected()) {
    return false;
  } 
  ADS.setGain(gain);
  return true;
}

/*
int canal: Canal de l'ADS1115 sur lequel faire la lecture de voltage.
// Compléter l'entête
*/
float lecture1ChADS1115(int canal) {
  // À COMPLÉTER - en validant le paramètre
  return -1.0;  // À compléter
}


// --- Thermistances ---

/*
float V1: Tension V1. (détaillez)
float V2: Tension V2. (détaillez)
float Rref: Valeur de la résistance utilisée dans le diviseur de tension.
// Compléter l'entête
*/
float calculResistanceThermistance(float& V1, float& V2, float Rref) {
  // À COMPLÉTER
  return -1.0;  // À compléter
}

/* Nom: calculTemperature
 * Fonction: calcule la temperature en fonction de la Rth
 * Argument(s) réception: Rth
 * Argument(s) de retour: temperature en degres Celcius
 * Modifie/utilise: (compléter)
 */
float calculTemperature(float Rth) {
  // À COMPLÉTER
  return -1.0;  // À compléter
}

/*
int thermistance: Thermistance sur laquelle faire la lecture.  0 -> thermistance froide, 1 -> thermistance chaude
Compléter l'entête
*/
float lectureTemperatureThermistance(int thermistance) {
  // À COMPLÉTER
  return -1.0;  // À compléter
}

// --- DS18B20 ---
// Compléter l'entête
float lectureTemperatureDS18B20() {
  // À COMPLÉTER
  return -1.0;  // À compléter
}

// Compléter l'entête
void envoieTemperature(int capteur) {
  switch (capteur) {
    case 0:
      Serial.println(temperatureFroid);
      break;
    case 1:
      Serial.println(temperatureChaud);
      break;
    case 2:
      Serial.println(temperatureAmbiant);
      break;
    default:
      Serial.println(NACK);
      break;
  }
}