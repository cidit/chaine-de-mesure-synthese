#include <Esp.h>
#include <math.h>
#include "commands.hpp"
#include "hardware_ctx.hpp"

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
    // out.printf(
    //     "%d;%d;%d;%d;%d",
    out.print(ctx.sensor_data.tension_shunt_V);
    out.print(";");
    out.print(ctx.sensor_data.tension_vBus_V);
    out.print(";");
    out.print(ctx.sensor_data.current_mA);
    out.print(";");
    out.print(ctx.sensor_data.power_mW);
    out.print(";");
    out.println(ctx.sensor_data.tension_vBus_V + (ctx.sensor_data.tension_shunt_V / 1000));
    // );
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
                out.println(ctx.hardware_drivers.cold_fan.get() ? 1 : 0);
                return true;
            }
        case 1:
            {
                out.println(ctx.hardware_drivers.hot_fan.get() ? 1 : 0);
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
    out.println(ctx.hardware_drivers.main_switch.get() ? 1 : 0);
    return true;
}

CMD_DEF(read_current_target) {
    out.println(ctx.hardware_drivers.dac.getPercentage(0));
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
        //        out.printf("%c : %s\n", cmd.code, cmd.help_text);
        out.print(cmd.code);
        out.print(" : ");
        out.print(cmd.help_text);
        out.println();
    }
    out.println();
    return true;
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
                ctx.hardware_drivers.cold_fan.set(state_to_set);
                out.println(ACK);
                return true;
            }
        case 1: {
                ctx.hardware_drivers.hot_fan.set(state_to_set);
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
                ctx.hardware_drivers.main_switch.set(LOW);
                reset_hardware(ctx); // on doit TOUT mettre a off
                out.println(ACK);
                return true;
            }
        case 1: {
                ctx.hardware_drivers.main_switch.set(HIGH);
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

    ctx.hardware_drivers.dac.setPercentage(target, 0);
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

    const char received_cmd = toupper(cmdl.getCommand());

    // auto ack = false;
    for (auto cmd : commands) {
        if (cmd.code == received_cmd) {
            cmd.fn(cmdl, ctx, out);
            // ack = cmd.fn(cmdl, ctx, out);
            break;
        }
    }
    // out.println(ack ? ACK : NACK);
}

CMD_DEF(run_analysis) {
    const int SAMPLE_TIME = 2 * 60 * 1000; // 2 min
    const int SAMPLE_COUNT = 10;

    const float percentage_interval = 100 / SAMPLE_COUNT;

    out.println("Début de l'analyse...");

    for (auto i = 0; i < SAMPLE_COUNT; i++) {
        auto percentage = percentage_interval * i;
        ctx.hardware_drivers.dac.setPercentage(percentage);

        // on imprime un point à chaques 2 secondes pour voir le progres en temps reel
        for (auto j = 0; j < 60; j++) {
            out.print(".");
            delay(2 * 1000);
        }
        out.println();

        update_sensor_data(ctx);
        out.print("iteration: ");
        out.print(i + 1);
        out.print("\t");
        out.print("perc (%): ");
        out.print(percentage);
        out.print("\t");
        out.print("power (mW): ");
        out.print(ctx.sensor_data.power_mW);
        out.print("\t");
        out.print("cold (C): ");
        out.print(ctx.sensor_data.temperature_cold);
        out.print("\t");
        out.print("hot (C): ");
        out.print(ctx.sensor_data.temperature_hot);
        out.print("\t");
        out.println();
    }
    out.println("fin de l'analyse.");
    
    // on éteint tout.
    reset_hardware(ctx);
    return true;
}