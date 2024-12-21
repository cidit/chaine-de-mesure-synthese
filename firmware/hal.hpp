/*
 * Fichier : hal.hpp
 * Description : 
 *  Ce fichier contient les déclarations des fonctions et des classes utilisées pour 
 *  l'interface matérielle (Hardware Abstraction Layer - HAL) dans le projet. 
 */


/**
 * Ceci sont les temperatures que l'on considère achevables par nos thermistances. en Celcius.
 */
const float TEMP_MIN = -55, TEMP_MAX = 155;


/*
 * Nom: WritePin
 * Fonction: Gère l'état d'un pin numérique (haut ou bas) sur une carte Arduino.
 * Notes: 
 *  - La classe permet de contrôler un pin de manière simple, en utilisant la méthode `set()` pour changer son état, et `get()` pour récupérer l'état actuel.
 *  - Le pin est initialisé en mode `OUTPUT` lors de la création de l'objet. L'état initial est `LOW` par défaut.
 */
class WritePin {

  uint8_t _pin;
  bool _state = LOW;

public:
  /**
 * Nom: WritePin::WritePin
 * Fonction: Constructeur de la classe `WritePin`, initialise le pin en mode sortie.
 * Argument(s) réception:
 *  - uint8_t pin : le numéro du pin à contrôler.
 * Argument(s) de retour: 
 *  - (rien)
 * Modifie/utilise (globale):
 *  - Aucun impact global. Initialise le pin en mode `OUTPUT`.
 * Notes: 
 *  - Cette méthode est appelée pour initialiser le pin à contrôler.
 */
  WritePin(uint8_t pin)
    : _pin(pin) {
    pinMode(pin, OUTPUT);
  }

  /**
 * Nom: WritePin::set
 * Fonction: Change l'état du pin, le met à `HIGH` ou `LOW`.
 * Argument(s) réception:
 *  - bool high : définit l'état du pin. `true` pour `HIGH` et `false` pour `LOW`.
 * Argument(s) de retour: 
 *  - (rien)
 * Modifie/utilise (globale):
 *  - Utilise la fonction `digitalWrite()` pour modifier l'état du pin.
 * Notes: 
 *  - Cette fonction met à jour l'état du pin à chaque appel.
 */
  void set(bool high) {
    _state = high ? HIGH : LOW;
    digitalWrite(_pin, _state);
  }

  /**
 * Nom: WritePin::get
 * Fonction: Retourne l'état actuel du pin.
 * Argument(s) réception: 
 *  - (rien)
 * Argument(s) de retour:
 *  - bool : l'état actuel du pin (`HIGH` ou `LOW`).
 * Modifie/utilise (globale):
 *  - Aucun impact global. Utilise la variable `_state` pour retourner l'état.
 * Notes: 
 *  - Cette fonction permet de récupérer l'état du pin sans le modifier.
 */
  bool get() {
    return _state;
  }
};

/*
 * Nom: Thermistance
 * Fonction: Calcule la température à partir de la résistance d'une thermistance en utilisant la méthode de Steinhart-Hart.
 * Notes: 
 *  - Cette classe permet de calculer la température à partir des valeurs de tension et résistance d'une thermistance en utilisant la méthode de Steinhart-Hart.
 *  - Elle prend en compte des valeurs par défaut pour les coefficients de Steinhart-Hart mais permet leur personnalisation.
 */
class Thermistance {
  const float _a, _b, _c;
  float _ref_resistance;

public:
  /**
 * Nom: Thermistance::Thermistance
 * Fonction: Constructeur de la classe `Thermistance`, initialise les paramètres nécessaires pour le calcul de température.
 * Argument(s) réception:
 *  - float ref_resistance : résistance de référence de la thermistance.
 *  - float a, b, c : coefficients de la formule de Steinhart-Hart (par défaut, les valeurs spécifiques à la thermistance).
 * Argument(s) de retour: 
 *  - (rien)
 * Modifie/utilise (globale):
 *  - Aucune modification globale. Initialise les variables locales `_a`, `_b`, `_c` et `_ref_resistance`.
 * Notes: 
 *  - Cette méthode initialise les coefficients de Steinhart-Hart et la résistance de référence pour le calcul de la température.
 */

  Thermistance(
    float ref_resistance,
    float a = 1.12585 * pow(10, -3),
    float b = 2.34609 * pow(10, -4),
    float c = 8.61871 * pow(10, -8))
    : _ref_resistance(ref_resistance),
      _a(a),
      _b(b),
      _c(c) {
  }

  /**
 * Nom: Thermistance::calculate_resistance_thermistance
 * Fonction: Calcule la résistance de la thermistance à partir de la tension de référence et de la tension mesurée.
 * Argument(s) réception:
 *  - float vref : tension de référence.
 *  - float vres : tension mesurée sur la thermistance.
 * Argument(s) de retour: 
 *  - float : la résistance calculée de la thermistance, ou -1 en cas de valeur invalide.
 * Modifie/utilise (globale):
 *  - Aucune modification globale. Utilise les variables `_ref_resistance` et `vref` pour effectuer le calcul.
 * Notes: 
 *  - Cette fonction effectue un calcul basé sur un pont diviseur de tension pour obtenir la résistance de la thermistance.
 *  - Si la tension de référence est inférieure ou égale à la tension mesurée, la fonction retourne -1 pour indiquer une erreur.
 */
  float calculate_resistance_thermistance(float vref, float vres) {
    if (vref <= vres) {
      return -1;
    }
    return _ref_resistance * (vres / (vref - vres));
  }

  /**
 * Nom: Thermistance::steinharthart
 * Fonction: Calcule la température en degrés Celsius à partir de la résistance de la thermistance en utilisant la formule de Steinhart-Hart.
 * Argument(s) réception:
 *  - float r : résistance de la thermistance.
 * Argument(s) de retour: 
 *  - float : température en degrés Celsius calculée à partir de la résistance.
 * Modifie/utilise (globale):
 *  - Aucune modification globale. Utilise les coefficients `_a`, `_b`, `_c` pour effectuer le calcul.
 * Notes: 
 *  - Cette méthode applique la formule de Steinhart-Hart pour calculer la température à partir de la résistance de la thermistance.
 *  - Le résultat est converti en degrés Celsius (température absolue moins 273.15).
 */
  float steinharthart(float r) {
    return (1 / (_a + _b * log(r) + _c * pow(log(r), 3))) - 273.15;
  }

  /**
 * Nom: Thermistance::get_temperature
 * Fonction: Calcule la température en fonction des tensions de référence et mesurée sur la thermistance.
 * Argument(s) réception:
 *  - float vref : tension de référence.
 *  - float vres : tension mesurée sur la thermistance.
 * Argument(s) de retour: 
 *  - float : température en degrés Celsius ou -1 en cas d'erreur de calcul ou de valeurs hors limites.
 * Modifie/utilise (globale):
 *  - Aucune modification globale. Utilise la méthode `calculate_resistance_thermistance()` pour calculer la résistance et `steinharthart()` pour obtenir la température.
 * Notes: 
 *  - Cette fonction est utilisée pour obtenir la température réelle de la thermistance en fonction des tensions mesurées.
 *  - Elle vérifie également que la résistance et la température calculées sont dans des plages valides.
 *  - La température est renvoyée seulement si elle se situe dans une plage spécifiée par les constantes `TEMP_MIN` et `TEMP_MAX`.
 */
  float get_temperature(float vref, float vres) {
    float r_thermistance = calculate_resistance_thermistance(vref, vres);
    if (r_thermistance > 100000 || r_thermistance < 0) {
      return -1;
    }
    auto temperature = steinharthart(r_thermistance);
    if (temperature < TEMP_MIN || temperature > TEMP_MAX) {
      return -1;
    }
    return temperature;
  }
};
