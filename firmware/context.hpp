#pragma once

#include <Arduino.h>
#include <Adafruit_INA219.h>
#include <ADS1X15.h>
#include <DallasTemperature.h>
#include <MCP_DAC.h>

#include "util.hpp"

/**
 * // TODO
 */
struct Context {

    String program_name;

    struct hardware_drivers {
        SPIClass spi;
        ADS1X15 adc;
        MCP_DAC dac;
        DallasTemperature dallas;
        Adafruit_INA219 ina;
        struct {
            bool spi, adc, dac, dallas, ina;
        } enabled;
    } hardware_drivers;


    WritePin cold_fan,
        hot_fan,
        main_switch;
    float current_target;
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