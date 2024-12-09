

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

#include <ADS1X15.h>
#include <Adafruit_INA219.h>
#include <DallasTemperature.h>
#include <Decodeur.h>  // https://github.com/TechnoPhysCAL/TGP_Decodeur  version 1.1.2
#include <OneWire.h>
#include <MCP_DAC.h>

#include "util.hpp"
#include "context.hpp"
#include "commands.hpp"
#include "sensors.hpp"

//-----------------------------------------------------------------------



//--- Definitions -------------------------------------------------------



//-----------------------------------------------------------------------



//--- Variables et Constantes --------------------------------------------------------

const uint8_t ONEWIRE_BUS_P = 0, // TODO: use correct pins
              CHIP_SELECT_P = 5, // IMPORTANT
              MAIN_SWITCH_P = 0,
              HOT_SIDE_FAN_P = 0,
              COLD_SIDE_FAN_P = 0,
              INA_I2C_ADDR = 0x40,
              ADS_I2C_ADDR = 0x48;

//-----------------------------------------------------------------------



//--- Objets --------------------------------------------

Decodeur cmdl(&Serial);
SPIClass spi = SPIClass(VSPI);

Context ctx{
  String("L'incroyable  et faramineuse plaque peltier de felix, V0.0.1"),
  {
    spi,
    ADS1115(0x48),
    MCP4921(&spi),
    DallasTemperature(&OneWire(ONEWIRE_BUS_P)),
    Adafruit_INA219(),
    // Les états enabled s'auto initializent à false.
  },
  WritePin(COLD_SIDE_FAN_P),
  WritePin(HOT_SIDE_FAN_P),
  WritePin(MAIN_SWITCH_P),
  // Le reste s'auto initialize à zéro et à false.
};
Timer timer(250);

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
void run_commands(Decodeur& cmdl) {

}


//-----------------------------------------------------------------------

//--- Setup et Loop -----------------------------------------------------
void setup() {
  Serial.begin(115200);
  sensors::init_sensors(ctx, Serial);
}  //Fin de setup()

void loop() {
  cmdl.refresh();
  commands::run_commands(cmdl, ctx, Serial);

  if (timer.is_done(millis())) {
    sensors::update_sensor_data(ctx);
  }
}  //Fin de loop()

//-----------------------------------------------------------------------