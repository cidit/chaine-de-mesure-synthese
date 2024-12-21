#pragma once

#include <Arduino.h>
#include <Adafruit_INA219.h>
#include <ADS1X15.h>
#include <DallasTemperature.h>
#include <MCP_DAC.h>

#include "util.hpp"
#include "hal.hpp"

/*
 * Nom : Context
 * Fonction : La structure `Context` sert à centraliser et organiser l'ensemble des informations relatives
 *            aux capteurs, actionneurs et données du système. Elle inclut les pilotes matériels nécessaires 
 *            à l'interaction avec ces composants, ainsi que les données collectées par ces derniers. 
 *            Cela permet de gérer efficacement l'état des dispositifs et les mesures dans un environnement embarqué.
 * Argument(s) réception : (rien)
 * Argument(s) de retour : (rien)
 * Description détaillée :
 *  - `hardware_drivers` : Contient les pilotes et les composants matériels du système.
 *    - `enabled` : Structure qui contient les états (activés/désactivés) des différents capteurs et actionneurs.
 * - `sensor_data` : Contient les données mesurées (courant, tensions, température, etc.) par les capteurs.
 * Notes : 
 * - Cette structure centralise toutes les informations relatives à la gestion et aux mesures des capteurs et actionneurs. 
 * - Elle permet d'avoir une vue d'ensemble du système et de faciliter la gestion des ressources matérielles.
 * - Les différents capteurs et actionneurs sont activés ou désactivés via la structure `enabled` dans `hardware_drivers`, 
 *   ce qui simplifie le contrôle et la configuration dynamique des composants.
 */
struct Context {

    String program_name;

    struct hardware_drivers {
        SPIClass *spi;
        ADS1X15 adc;
        MCP4921 dac;
        DallasTemperature dallas;
        Adafruit_INA219 ina;
        /**
         * Les états de controles des differents capteurs.
         */
        struct {
            bool spi, adc, dac, dallas, ina;
        } enabled;
        WritePin cold_fan,
            hot_fan,
            main_switch;
        Thermistance cold_thermistance, hot_thermistance;
    } hardware_drivers;


    struct SensorData {
        float current_mA,
            tension_shunt_V,
            tension_vBus_V,
            power_mW,
            temperature_hot,
            temperature_cold,
            temperature_ambiant;
    } sensor_data;

};
