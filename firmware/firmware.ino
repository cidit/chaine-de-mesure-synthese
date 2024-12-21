

/*  --- Entête principale -- information sur le programme
 *
 *  Programme:        "L'incroyable et faramineuse plaque peltier de felix, V0.0.1"
 *  Date:             avant le 20 décembre 2024...
 *  Auteur:           Félix St-Gelais FSTG 
 *  Pltform matériel: ESP32 DOIT KIT V1 - protoTPhys
 *  Pltform develop:  Arduino 2.0.3
 *  Description:      Controlleur d'un banc de test des performance d'un module de cellule Peltier.
 *  Fonctionnalités:  manipulation de la consigne de courant de la cellule, mesures de température, protocole de communication en série.
 *  Notes:
 *
 *  -- Inspirations et credits: --
 *   les laboratoires effectués précédament dans le cadre du cours de chaine de mesure et leurs inspirations.
 *
 */

/* --- Materiel et composants -------------------------------------------
  * la liste de matériel ainsi que la procédure complète du laboratoire de ce code se situe sur github: 
  *   https://github.com/cidit/chaine-de-mesure-synthese/blob/main/LaboCertif-cellulePeltier_A24.pdf 
 */


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
#include "hardware_ctx.hpp"

//-----------------------------------------------------------------------



//--- Variables et Constantes --------------------------------------------------------

const uint8_t DS18B20_ONEWIRE_BUS_P = 15,
              CHIP_SELECT_P = 5,
              MAIN_SWITCH_P = 32,
              HOT_SIDE_FAN_P = 25,
              COLD_SIDE_FAN_P = 26,
              INA_I2C_ADDR = 0x40,
              ADS_I2C_ADDR = 0x48;
const float COLD_REF_RESISTANCE = 9890,
            HOT_REF_RESISTANCE = 9852;
const int ADC_GAIN = 0;

//-----------------------------------------------------------------------



//--- Objets --------------------------------------------

Decodeur cmdl(&Serial);
SPIClass spi = SPIClass(VSPI);
OneWire onewire(DS18B20_ONEWIRE_BUS_P);

Context ctx{
  String("L'incroyable et faramineuse plaque peltier de felix, V0.0.1"),
  {
    &spi,
    ADS1115(ADS_I2C_ADDR),
    MCP4921(&spi),
    DallasTemperature(&onewire),
    Adafruit_INA219(INA_I2C_ADDR),
    { // Les états d'activations s'auto-initializent à false.
      0 },
    WritePin(COLD_SIDE_FAN_P),
    WritePin(HOT_SIDE_FAN_P),
    WritePin(MAIN_SWITCH_P),
    Thermistance(COLD_REF_RESISTANCE),
    Thermistance(HOT_REF_RESISTANCE),
  },
  { // Les valeurs de capteurs s'auto-initializent à zéro.
    0 },
};
Timer timer(1000); // en ms

//-----------------------------------------------------------------------


//--- Setup et Loop -----------------------------------------------------

void setup() {
  Serial.begin(115200);
  init_hardware(ctx, Serial, ADC_GAIN, CHIP_SELECT_P);
}  //Fin de setup()

void loop() {
  cmdl.refresh();
  run_commands(cmdl, ctx, Serial);

  if (timer.is_done(millis())) {
    update_sensor_data(ctx);
  }
}  //Fin de loop()

//-----------------------------------------------------------------------
