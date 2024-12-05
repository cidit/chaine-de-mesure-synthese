#include "commands.hpp"

namespace commands {
    void run_commands(Decodeur& cmdl, Context& ctx, Print& out) {

        if (!cmdl.isAvailable()) {
            return;
        }

        const auto received_cmd = toupper(cmdl.getCommand());

        auto ack = false;
        for (auto cmd : commands) {
            if (cmd.code == received_cmd) {
                ack = cmd.fn(cmdl, ctx, out);
            }
        }
        out.println(ack ? ACK : NACK);
    }

    CMD_DEF(read_current) {
        out.println(ctx.current_mA);
        return true;
    }

    CMD_DEF(read_tension_vbus) {
        out.println(ctx.tension_vBus_V);
        return true;
    }

    CMD_DEF(read_power) {
        out.println(ctx.power_mW);
        return true;
    }

    CMD_DEF(read_electrical_circuit) {
        return false; // TODO unimplemented. again, check with yanick on what exactly this is supposed to output.
    }

    CMD_DEF(read_tachymetry) {
        // ne sera jamais implémenté
        return false;
    }

    CMD_DEF(read_temperature);

    CMD_DEF(read_fan) {
        if (cmdl.getArgCount() < 1) {
            return false;
        }

        auto arg0 = cmdl.getArg(0);
        if (isnan(arg0))\
        {
            // le parametre n'est pas un chiffre valide
            return false;
        }

        switch (int(arg0)) {
            case 0:
                {
                    out.println(ctx.fan_cold? 1: 0);
                    break;
                }
            case 1:
                {
                    out.println(ctx.fan_hot? 1: 0);
                    break;
                }
            default:
                {
                    // le parametre n'est pas le chiffre qu'on recherche
                    return false;
                }
        }
        return true;
    }

    CMD_DEF(read_main_switch) {
        out.println(ctx.main_switch? 1: 0);
        return true;
    }

    CMD_DEF(read_current_target) {
        out.println(round(ctx.current_target * 100));
        return true;
    }

    CMD_DEF(read_program_name) {
        out.println(ctx.program_name);
        return true;
    }
    
    CMD_DEF(show_menu) {
        out.println("MENU");
        out.println("----");
        for (auto cmd: commands) {
            out.printf("%c : %s\n", cmd.code, cmd.help_text);
        }
    }
    
    CMD_DEF(set_fan);
    CMD_DEF(set_main_switch);
    CMD_DEF(set_current_target);

    CMD_DEF(restart) {
        out.println(ACK);
        ESP.restart();
        return true; // vu que l'ESP redemare, le return est du code mort.
    }
}