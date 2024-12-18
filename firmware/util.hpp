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
        if (now - _last < _length) {
            return false;
        }
        _last = now;
        return true;
    }
 
};
