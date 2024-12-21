
#include <Arduino.h>
#include <ADS1X15.h>
#include <Adafruit_INA219.h>
#include <DallasTemperature.h>
#include <MCP_DAC.h>
#include "hardware_ctx.hpp"
#include "context.hpp"


void reset_hardware(Context& ctx) {
    auto hw = &ctx.hardware_drivers;
    hw->main_switch.set(LOW);
    hw->hot_fan.set(LOW);
    hw->cold_fan.set(LOW);
    hw->dac.setPercentage(0);
}

void init_hardware(Context& ctx, Print& out, int adc_gain, int mcp_chipselect) {
    auto hw = &ctx.hardware_drivers;
    Wire.begin();

    if (hw->adc.begin()) {
        out.println("!: L'ADS1115 est connecté.");
        hw->enabled.adc = true;
        hw->adc.setGain(0);
    }
    else {
        out.println("E: L'ADS1115 n'est pas branché ou son adresse est invalide.");
        hw->enabled.adc = false;
    }

    hw->spi->begin();
    hw->dac.begin(mcp_chipselect);
    // vu que on n'a pas de moyen direct de tester si le dac fonctionne, on assume qu'il est en fonction.
    hw->enabled.dac = true;

    hw->dallas.begin();
    // pareil pour le dallas.
    hw->enabled.dallas = true;

    if (hw->ina.begin()) {
        out.println("!: L'INA219 est connecté.");
        hw->enabled.ina = true;
    }
    else {
        out.println("E: L'INA219 n'est pas branché ou son adresse est invalide.");
        hw->enabled.ina = false;
    }

    // on met tout s'assure que l'etat de nos sorties sont biens toutes éteintes.
    reset_hardware(ctx);
}


void update_sensor_data(Context& ctx) {
    auto sd = &ctx.sensor_data;
    auto hw = &ctx.hardware_drivers;

    if (hw->enabled.adc) {
        hw->dallas.requestTemperatures();

        double v1=0, v2=0, v3=0, v4=0;
        for (int i = 0; i < 4; i++) {
          hw->adc.readADC(0);
          v1 += hw->adc.getValue();
          hw->adc.readADC(1);
          v2 += hw->adc.getValue();
          hw->adc.readADC(2);
          v3 += hw->adc.getValue();
          hw->adc.readADC(3);
          v4 += hw->adc.getValue();
        }
        v1 /=4;
        v2 /=4;
        v3 /=4;
        v4 /=4;

        sd->temperature_ambiant = hw->dallas.getTempCByIndex(0);
        if (sd->temperature_ambiant == DEVICE_DISCONNECTED_C) {
            sd->temperature_ambiant = -1;
        }
        sd->temperature_cold = hw->cold_thermistance.get_temperature(v1, v2);
        sd->temperature_hot = hw->hot_thermistance.get_temperature(v3, v4);
    }

    if (hw->enabled.ina) {
        sd->current_mA = hw->ina.getCurrent_mA();
        sd->tension_vBus_V = hw->ina.getBusVoltage_V();
        sd->power_mW = hw->ina.getPower_mW();
        sd->tension_shunt_V = hw->ina.getShuntVoltage_mV();
    }

}
