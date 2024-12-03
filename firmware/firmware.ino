/*  --- Entête principale -- information sur le programme
 *   
 *  Programme:        (nom du programme)
 *  Date:             (date initiale de création)
 *  Auteur:           (nom, initiale de l'auteur de ce code)
 *  Pltform matériel: (plateforme matérielle et version - ESP32 DOIT KIT V1 - protoTPhys)
 *  Pltform develop : (nom et version de la plateforme de développement - Arduino 2.0.3/1.8.19)
 *  Description:      (brève description de ce qu'accompli ce programme/sketche (+lignes si requis)
 *  Fonctionnalités:  (fonctionnalités principales)
 *  Notes:
 *    
 *  -- Inspirations et credits: --
 *   (faire la liste des codes d'inspiration et ou des crédits de code)
 *
 *  Note: les éléments //*** sont à effacer lors de l'utilisation du modèle, de même que l'exemple de l'historique.
 */

/* --- Materiel et composants -------------------------------------------
 * //*** ex: - BMP280 : capteur de température
 * //*** (faire la liste...)
*/


/* --- HISTORIQUE de développement --------------------------------------
 * //*** ex: v0.1.x: version initiale de test 
 * //*** ex:    - détails sur ce qui a été fait
 * //*** ex:    - détails sur ce qui a été expérimenté
 * //*** ex:    - détails...
 * //*** ex: v1.0.x: version 1 de niveau production (détails)
 * //*** ex: v2.x: versions futures et développements
 * //*** Version et historique de developpement (plus bas)
 * //*** Consulter: https://semver.org/lang/fr/ 
 * //*** ex: #define  _VERSION "0.1.1"
 * //*** format x.y.z:
 * //***   x: MAJEUR: niveau principal (0=preuve de concept et essais, 1=1ere prod, ...=subsequente);
 * //***   y: INTERMÉDIAIRE: sous-version;
 * //***   z: CORRECTIF: incrément de la sous-version intermédiaire
 */
//-----------------------------------------------------------------------



//--- Déclaration des librairies (en ordre alpha) -----------------------

#include <Decodeur.h>  // https://github.com/TechnoPhysCAL/TGP_Decodeur  version 1.1.2

//-----------------------------------------------------------------------



//--- Definitions -------------------------------------------------------

using CommandFunctionPtr = bool (*)(Decodeur &, Context &);

//-----------------------------------------------------------------------



//--- Variables et Constantes --------------------------------------------------------

//-----------------------------------------------------------------------



//--- Objets --------------------------------------------

Decodeur monDecodeur(&Serial);

//-----------------------------------------------------------------------



//--- Section des routines specifiques ----------------------------------

namespace commands {



bool cmd_fan(Decodeur &cmdl) {
  if (cmdl.getArgCount() == 0) {
    // si pas de parametres, on toggle
    fan::enabled = !fan::enabled;
  } else {
    auto arg0 = cmdl.getArg(0);
    if (isnan(arg0)) {
      // le deuxieme parametre n'est pas un chiffre
      return false;
    } else {
      fan::speed = arg0;
    }
  }
  return true;
}

struct Command {
  char code;
  CommandFunctionPtr fn;
  String help_text;
}

static const Command commands = [
    { 'F', &cmd_fan, "F [n] [s]: change the state of the fan. [n=which fan to manipulate] [s=the state to put it to, 1(on) or 0(off)]" }, 
  ];

/* 
 * Nom: Run Commands
 * Fonction: execute les differentes commandes recues du port série. Entrée principale de l'interaction en série.
 * Argument(s) réception: (rien)
 * Argument(s) de retour: (rien)
 * Modifie/utilise (globale):
 * Notes:  (spécial, source, amélioration, etc)
 * 
 */
void run_commands(Decodeur &cmdl) {
  

  if (!cmdl.available()) {
    return;
  }


  const auto cmd = String(cmdl.getCommand()).toUpperCase()[0];

  auto ack = false;
  for (auto cmd_pair : commands) {
    if (cmd_pair.first == cmd) {
      ack = (*cmd_pair.second)(&cmdl);
    }
  }
  Serial.println(ack ? "*" : "?");
}

}  // end namespace commands

//-----------------------------------------------------------------------

//--- Setup et Loop -----------------------------------------------------
void setup() {
  Serial.begin(115200);

}  //Fin de setup()

void loop() {
  decodeur.refresh();
  run_commands(decodeur);
}  //Fin de loop()
   //-----------------------------------------------------------------------

/* Version de modèle: B - Yh H23 */