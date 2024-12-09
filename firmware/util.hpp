#pragma once

/**
 * // TODO
 */
class Timer {
    long _last = 0;
    long _length;

public:
    /**
     * // TODO
     */
    Timer(long length) : _length(length) {}

    /**
     * // TODO
     */
    bool is_done(long now) {
        if (now - _last > _length) {
            _last = now;
            return true;
        }
        return false;
    }

    /**
     * // TODO
     */
    long time_left(long now) {
        return _length - (now - _last);
    }
};

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