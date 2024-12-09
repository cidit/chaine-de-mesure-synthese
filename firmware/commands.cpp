#include <Esp.h>
#include <math.h>
#include "commands.hpp"

namespace commands {

    CMD_DEF(read_current) {
        out.println(ctx.sensor_data.current_mA);
        return true;
    }

    CMD_DEF(read_tension_vbus) {
        out.println(ctx.sensor_data.tension_vBus_V);
        return true;
    }

    CMD_DEF(read_power) {
        out.println(ctx.sensor_data.power_mW);
        return true;
    }

    CMD_DEF(read_electrical_circuit) {
        out.printf(
            "%d;%d;%d;%d;%d",
            ctx.sensor_data.tension_shunt_V,
            ctx.sensor_data.tension_vBus_V,
            ctx.sensor_data.current_mA,
            ctx.sensor_data.power_mW,
            ctx.sensor_data.tension_vBus_V - ctx.sensor_data.tension_shunt_V / 1000
        );
        return true;
    }

    CMD_DEF(read_tachymetry) {
        // ne sera jamais implémenté
        out.println(NACK);
        return false;
    }

    CMD_DEF(read_temperature) {
        if (cmdl.getArgCount() < 1) {
            out.println(NACK);
            return false;
        }

        auto arg0 = cmdl.getArg(0);
        if (isnan(arg0)) {
            // le parametre n'est pas un chiffre valide
            out.println(NACK);
            return false;
        }

        switch (int(arg0)) {
            case 0:
                {
                    out.println(ctx.sensor_data.temperature_cold);
                    return true;
                }
            case 1:
                {
                    out.println(ctx.sensor_data.temperature_hot);
                    return true;
                }
            case 2:
                {
                    out.println(ctx.sensor_data.temperature_ambiant);
                    return true;
                }
            default:
                {
                    // le parametre n'est pas un chiffre qu'on recherche
                    out.println(NACK);
                    return false;
                }
        }
    }

    CMD_DEF(read_fan) {
        if (cmdl.getArgCount() < 1) {
            out.println(NACK);
            return false;
        }

        auto arg0 = cmdl.getArg(0);
        if (isnan(arg0)) {
            // le parametre n'est pas un chiffre valide
            out.println(NACK);
            return false;
        }

        switch (int(arg0)) {
            case 0:
                {
                    out.println(ctx.cold_fan.get() ? 1 : 0);
                    return true;
                }
            case 1:
                {
                    out.println(ctx.hot_fan.get() ? 1 : 0);
                    return true;
                }
            default:
                {
                    // le parametre n'est pas un chiffre qu'on recherche
                    out.println(NACK);
                    return false;
                }
        }
    }

    CMD_DEF(read_main_switch) {
        out.println(ctx.main_switch.get() ? 1 : 0);
        return true;
    }

    CMD_DEF(read_current_target) {
        // mult. par 100 parce que on veut un pourcentage.
        out.printf("%d%\n", round(ctx.current_target * 100));
        return true;
    }

    CMD_DEF(read_program_name) {
        out.println(ctx.program_name);
        return true;
    }

    CMD_DEF(show_menu) {
        out.println("MENU");
        out.println("Les [] demarquent des arguments d'entrée et les {} démarquent des informations sur les valeurs de retour.");
        out.println("----------------------");
        for (auto cmd : commands) {
            out.printf("%c : %s\n", cmd.code, cmd.help_text);
        }
        out.println();
    }

    CMD_DEF(set_fan) {
        if (cmdl.getArgCount() < 2) {
            out.println(NACK);
            return false;
        }

        auto state_to_set = cmdl.getArg(1);
        if (state_to_set > 1 || state_to_set < 0 || isnan(state_to_set)) {
            // l'argument designant l'état n'est pas un chiffre valide.
            out.println(NACK);
            return false;
        }

        auto which_fan = cmdl.getArg(0);
        if (isnan(which_fan)) {
            out.println(NACK);
            return false;
        }

        switch (int(which_fan)) {
            case 0: {
                    ctx.cold_fan.set(state_to_set);
                    out.println(ACK);
                    return true;
                }
            case 1: {
                    ctx.hot_fan.set(state_to_set);
                    out.println(ACK);
                    return true;
                }
            default: {
                    // l'argument représentant le choix de ventilateur n'est pas un chiffre valide
                    out.println(NACK);
                    return false;
                }
        }
    }

    CMD_DEF(set_main_switch) {
        if (cmdl.getArgCount() < 1) {
            out.println(NACK);
            return false;
        }

        auto state_to_set = cmdl.getArg(0);
        if (isnan(state_to_set)) {
            // l'argument désignant l'état n'est pas un chiffre valide.
            out.println(NACK);
            return false;

        }
        switch (int(state_to_set)) {
            case 0: {
                    ctx.main_switch.set(LOW);
                    out.println(ACK);
                    return true;
                }
            case 1: {
                    ctx.main_switch.set(HIGH);
                    out.println(ACK);
                    return true;
                }
            default:
                // l'argument désignant l'état n'est pas un chiffre attendu.
                out.println(NACK);
                return false;
        }
    }

    CMD_DEF(set_current_target) {
        if (cmdl.getArgCount() < 1) {
            out.println(NACK);
            return false;
        }

        auto target = cmdl.getArg(0);
        if (isnan(target) || target < 0 || target > 100) {
            // l'argument désignant la consigne de courant n'est pas un chiffre attendu
            out.println(NACK);
            return false;
        }

        // TODO: devrait changer immediatement.
        ctx.current_target = target;
        out.println(ACK);
        return true;
    }

    CMD_DEF(restart) {
        out.println(ACK);
        ESP.restart();
        return true; // vu que l'ESP redemare, le return est du code mort.
    }

    void run_commands(Decodeur& cmdl, Context& ctx, Print& out) {
        if (!cmdl.isAvailable()) {
            return;
        }

        const auto received_cmd = toupper(cmdl.getCommand());

        // auto ack = false;
        for (auto cmd : commands) {
            if (cmd.code == received_cmd) {
                cmd.fn(cmdl, ctx, out);
                // ack = cmd.fn(cmdl, ctx, out);
            }
        }
        // out.println(ack ? ACK : NACK);
    }
}