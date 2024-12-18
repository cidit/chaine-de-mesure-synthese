#pragma once

#include <Arduino.h>
#include <Adafruit_INA219.h>
#include <ADS1X15.h>
#include <DallasTemperature.h>
#include <MCP_DAC.h>

#include "util.hpp"
#include "hal.hpp"

/**
 * // TODO
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
