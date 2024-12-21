#pragma once

#include <Arduino.h>

/*
 * Nom : Timer
 * Fonction : La classe `Timer` permet de gérer une minuterie, permettant de vérifier si un certain temps a écoulé depuis la dernière
 *            activation du timer. Elle fournit une méthode pour déterminer si le temps défini a expiré, en utilisant un horodatage fourni.
 * Notes : La classe peut être utilisée pour contrôler des délais dans le programme, en vérifiant régulièrement si un événement 
 *         devrait se produire après un certain temps.
 */
class Timer {
  uint32_t _last = 0;
  uint32_t _length;

public:
  /*
  * Nom : Timer::Timer
  * Fonction : Constructeur de la classe `Timer`, qui initialise la durée du timer. Le temps de la dernière activation est initialisé à 0.
  * Argument(s) réception :
  * - long length : La durée du temporisateur en millisecondes.
  * Argument(s) de retour : (rien)
  * Modifie/utilise (globale) : 
  * - Cette méthode initialise les variables membres `_length` et `_last`.
  * Notes : Le temporisateur est prêt à être utilisé dès sa création avec la durée spécifiée.
  */
  Timer(long length)
    : _length(length) {}

  /*
  * Nom : Timer::is_done
  * Fonction : Vérifie si la durée du temporisateur est écoulée, en comparant l'horodatage actuel avec la dernière activation du timer.
  *            Si le délai est écoulé, le timer est réinitialisé et la fonction retourne `true`. Sinon, elle retourne `false`.
  * Argument(s) réception :
  * - long now : L'horodatage actuel (en millisecondes), généralement fourni par la fonction `millis()` d'Arduino.
  * Argument(s) de retour :
  * - bool : `true` si le délai est écoulé, `false` sinon.
  * Modifie/utilise (globale) : 
  * - Cette fonction modifie la variable membre `_last` chaque fois que le délai est écoulé.
  * Notes : Cette méthode permet de gérer le comportement de temporisation dans le programme, avec une réinitialisation automatique du timer après chaque expiration.
  */
  bool is_done(long now) {
    if (now - _last < _length) {
      return false;
    }
    _last = now;
    return true;
  }
};
