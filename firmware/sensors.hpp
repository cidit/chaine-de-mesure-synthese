#pragma once

#include <ADS1X15.h>
#include <Adafruit_INA219.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <MCP_DAC.h>
#include "context.hpp"

namespace sensors {

    /**
     * // TODO
     */
    void init_sensors(Context&, Print&);

    /**
     * // TODO
     */
    void update_sensor_data(Context&);

}