/**
 * HAL (Hardware Abstraction Layer)
 * // TODO:
 */


//const float TEMP_MIN = -55, TEMP_MAX = 155;


/**
 * // TODO
 */
class WritePin {

    uint8_t _pin;
    bool _state = LOW;

public:
    /**
     * // TODO
     */
    WritePin(uint8_t pin) : _pin(pin) {
        pinMode(pin, OUTPUT);
    }

    /**
     * // TODO
     */
    void set(bool high) {
        _state = high ? HIGH : LOW;
        digitalWrite(_pin, _state);
    }

    /**
     * // TODO
     */
    bool get() {
        return _state;
    }
};

/**
 * // TODO
 */
class Thermistance {
    const float _a, _b, _c;
    float _ref_resistance;

public:
    /**
     * // TODO
     */
    Thermistance(
        float ref_resistance,
        float a = 1.12585 * pow(10, -3),
        float b = 2.34609 * pow(10, -4),
        float c = 8.61871 * pow(10, -8)
    ) : _ref_resistance(ref_resistance),
        _a(a),
        _b(b),
        _c(c) {
    }

    float calculate_resistance_thermistance(float vref, float vres) {
        if (vref <= vres) {
            return -1;
        }
        return _ref_resistance * (vres / (vref - vres));
    }

    /**
     * // TODO
     * takes resistance of thermistance
     * returns celcius
     */
    float steinharthart(float r) {
        return (1 / (_a + _b * log(r) + _c * pow(log(r), 3))) - 273.15;
    }

    /**
     * // TODO
     */
    float get_temperature(float vref, float vres) {
        float r_thermistance = calculate_resistance_thermistance(vref, vres);
        if (r_thermistance > 100000 || r_thermistance < 0) {
            return -2;
        }
        auto temperature = steinharthart(r_thermistance);
//        if (temperature < TEMP_MIN || temperature > TEMP_MAX) {
//            return -3;
//        }
        return temperature;
    }
};
