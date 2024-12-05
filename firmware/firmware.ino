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



//-----------------------------------------------------------------------



//--- Variables et Constantes --------------------------------------------------------

//-----------------------------------------------------------------------



//--- Objets --------------------------------------------

Decodeur cmdl(&Serial);

//-----------------------------------------------------------------------



//--- Section des routines specifiques ----------------------------------



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
  
}


//-----------------------------------------------------------------------

//--- Setup et Loop -----------------------------------------------------
void setup() {
  Serial.begin(115200);

}  //Fin de setup()

void loop() {
  cmdl.refresh();
  run_commands(cmdl);
}  //Fin de loop()

//-----------------------------------------------------------------------