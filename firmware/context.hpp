#pragma once

struct Context {
    float current_mA, tension_vBus_V, power_mW, current_target;
    bool fan_cold, fan_hot, main_switch;
    String program_name;
};