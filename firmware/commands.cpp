#include <Esp.h>
#include <math.h>
#include "commands.hpp"
#include "hardware_ctx.hpp"

/**
 * Nom : read_current
 * Fonction : Commande pour lire la valeur du courant en mA. Affiche la valeur dans le terminal.
 * Arguments de commande attendus :
 * - Aucun argument.
 * Argument(s) de retour :
 * - bool : Retourne `true` après avoir affiché la valeur du courant.
 * Modifie/utilise (globale) :
 * - Utilise la donnée `current_mA` de `sensor_data` dans `Context`.
 * Notes : Cette commande renvoie le courant mesuré par le système.
 */
CMD_DEF(read_current) {
    out.println(ctx.sensor_data.current_mA);
    return true;
}

/**
 * Nom : read_tension_vbus
 * Fonction : Commande pour lire la tension de l'alimentation Vbus en Volts. Affiche la valeur dans le terminal.
 * Arguments de commande attendus :
 * - Aucun argument.
 * Argument(s) de retour :
 * - bool : Retourne `true` après avoir affiché la valeur de la tension.
 * Modifie/utilise (globale) :
 * - Utilise la donnée `tension_vBus_V` de `sensor_data` dans `Context`.
 * Notes : Cette commande renvoie la tension mesurée sur le Vbus du système.
 */
CMD_DEF(read_tension_vbus) {
    out.println(ctx.sensor_data.tension_vBus_V);
    return true;
}

/**
 * Nom : read_power
 * Fonction : Commande pour lire la puissance en mW. Affiche la valeur dans le terminal.
 * Arguments de commande attendus :
 * - Aucun argument.
 * Argument(s) de retour :
 * - bool : Retourne `true` après avoir affiché la puissance.
 * Modifie/utilise (globale) :
 * - Utilise la donnée `power_mW` de `sensor_data` dans `Context`.
 * Notes : Cette commande renvoie la puissance calculée en milliwatts.
 */
CMD_DEF(read_power) {
    out.println(ctx.sensor_data.power_mW);
    return true;
}

/**
 * Nom : read_electrical_circuit
 * Fonction : Commande pour lire plusieurs paramètres électriques du circuit, incluant la tension de shunt, la tension du bus,
 *            le courant, la puissance, et la tension du chargeur. Affiche ces valeurs sous forme de chaîne séparée par des points-virgules.
 * Arguments de commande attendus :
 * - Aucun argument.
 * Argument(s) de retour :
 * - bool : Retourne `true` après avoir affiché les informations.
 * Modifie/utilise (globale) :
 * - Utilise les données `tension_shunt_V`, `tension_vBus_V`, `current_mA`, `power_mW` de `sensor_data` dans `Context`.
 * Notes : Cette commande permet de vérifier l'état global du circuit électrique.
 */
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


/**
 * Nom : read_tachymetry
 * Fonction : Commande pour lire la valeur du tachymètre en RPM. Cette fonction n'est pas implémentée et renvoie une réponse d'erreur.
 * Arguments de commande attendus :
 * - Aucun argument.
 * Argument(s) de retour :
 * - bool : Retourne `false` car la commande n'est pas implémentée.
 * Modifie/utilise (globale) :
 * - Aucun changement dans les données globales.
 * Notes : Cette commande est indiquée comme non implémentée et renvoie un NACK.
 */
CMD_DEF(read_tachymetry) {
    // ne sera jamais implémenté
    out.println(NACK);
    return false;
}

/**
 * Nom : read_temperature
 * Fonction : Commande pour lire la température. Elle prend un argument pour spécifier quel capteur de température lire (froid, chaud ou ambiant).
 * Arguments de commande attendus :
 * - int arg0 : Indicateur du capteur de température à lire (0 = froid, 1 = chaud, 2 = ambiant).
 * Argument(s) de retour :
 * - bool : Retourne `true` si la commande est réussie et la température est affichée, sinon retourne `false`.
 * Modifie/utilise (globale) :
 * - Utilise les données `temperature_cold`, `temperature_hot`, `temperature_ambiant` de `sensor_data` dans `Context`.
 * Notes : La commande renvoie la température en fonction de l'argument passé.
 */
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

/**
 * Nom : read_fan
 * Fonction : Commande pour lire l'état des ventilateurs (froid ou chaud). Elle prend un argument pour spécifier le ventilateur à vérifier.
 * Arguments de commande attendus :
 * - int arg0 : Indicateur du ventilateur à contrôler (0 = froid, 1 = chaud).
 * Argument(s) de retour :
 * - bool : Retourne `true` si la commande est réussie et l'état du ventilateur est affiché, sinon retourne `false`.
 * Modifie/utilise (globale) :
 * - Utilise les états des ventilateurs dans `hardware_drivers` de `Context`.
 * Notes : Cette commande permet de lire l'état (ON/OFF) des ventilateurs froid et chaud.
 */
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

/**
 * Nom : read_main_switch
 * Fonction : Commande pour lire l'état du commutateur principal (ON/OFF). Affiche l'état du commutateur.
 * Arguments de commande attendus :
 * - Aucun argument.
 * Argument(s) de retour :
 * - bool : Retourne `true` après avoir affiché l'état du commutateur.
 * Modifie/utilise (globale) :
 * - Utilise l'état du `main_switch` dans `hardware_drivers` de `Context`.
 * Notes : Cette commande permet de lire l'état du commutateur principal du système.
 */
CMD_DEF(read_main_switch) {
    out.println(ctx.hardware_drivers.main_switch.get() ? 1 : 0);
    return true;
}

/**
 * Nom : read_current_target
 * Fonction : Commande pour lire la consigne de courant en pourcentage. Affiche la valeur dans le terminal.
 * Arguments de commande attendus :
 * - Aucun argument.
 * Argument(s) de retour :
 * - bool : Retourne `true` après avoir affiché la consigne de courant.
 * Modifie/utilise (globale) :
 * - Utilise la méthode `getPercentage` de `dac` dans `hardware_drivers` pour obtenir la consigne.
 * Notes : Cette commande renvoie la consigne de courant en pourcentage.
 */
CMD_DEF(read_current_target) {
    out.println(ctx.hardware_drivers.dac.getPercentage(0));
    return true;
}

/**
 * Nom : read_program_name
 * Fonction : Commande pour lire le nom du programme et sa version.
 * Arguments de commande attendus :
 * - Aucun argument.
 * Argument(s) de retour :
 * - bool : Retourne `true` après avoir affiché le nom et la version du programme.
 * Modifie/utilise (globale) :
 * - Utilise `program_name` dans `Context`.
 * Notes : Cette commande permet de connaître le nom du programme actuellement en cours d'exécution.
 */
CMD_DEF(read_program_name) {
    out.println(ctx.program_name);
    return true;
}

/**
 * Nom : show_menu
 * Fonction : Commande pour afficher le menu des commandes disponibles, avec une explication de chaque commande.
 * Arguments de commande attendus :
 * - Aucun argument.
 * Argument(s) de retour :
 * - bool : Retourne `true` après avoir affiché le menu des commandes.
 * Modifie/utilise (globale) :
 * - Utilise le tableau `commands` pour afficher la liste des commandes.
 * Notes : Cette commande fournit un menu d'utilisation pour l'utilisateur, détaillant les commandes disponibles.
 */
CMD_DEF(show_menu) {
    out.println("MENU");
    out.println("Les [] demarquent des arguments d'entrée et les {} démarquent des informations sur les valeurs de retour.");
    out.println("----------------------");
    for (auto cmd : commands) {
        out.print(cmd.code);
        out.print(" : ");
        out.print(cmd.help_text);
        out.println();
    }
    out.println();
    return true;
}

/**
 * Nom : set_fan
 * Fonction : Commande pour activer ou désactiver un ventilateur (froid ou chaud). Cette fonction prend deux arguments : le ventilateur et son état.
 * Arguments de commande attendus :
 * - int arg0 : Indicateur du ventilateur à contrôler (0 = froid, 1 = chaud).
 * - int arg1 : L'état à définir (0 = OFF, 1 = ON).
 * Argument(s) de retour :
 * - bool : Retourne `true` si la commande est réussie et l'état du ventilateur est correctement configuré, sinon retourne `false`.
 * Modifie/utilise (globale) :
 * - Modifie l'état des ventilateurs dans `hardware_drivers` de `Context`.
 * Notes : Cette commande permet de contrôler l'état des ventilateurs en fonction des arguments passés.
 */
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

/**
 * Nom : set_main_switch
 * Fonction : Commande pour activer ou désactiver le commutateur principal. Cette fonction prend un argument pour définir l'état du commutateur.
 * Arguments de commande attendus :
 * - int arg0 : L'état à définir pour le commutateur principal (0 = OFF, 1 = ON).
 * Argument(s) de retour :
 * - bool : Retourne `true` après avoir configuré l'état du commutateur et effectué les actions nécessaires.
 * Modifie/utilise (globale) :
 * - Modifie l'état du `main_switch` dans `hardware_drivers` de `Context`.
 * Notes : Cette commande permet d'activer ou de désactiver le commutateur principal et de réinitialiser tout le système lorsque l'état est OFF.
 */
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

/**
 * Nom : set_current_target
 * Fonction : Commande pour définir la consigne de courant en pourcentage. Prend un argument entre 0 et 100 pour configurer la consigne.
 * Arguments de commande attendus :
 * - float arg0 : La consigne de courant à définir en pourcentage (0-100).
 * Argument(s) de retour :
 * - bool : Retourne `true` si la consigne est définie avec succès, sinon retourne `false`.
 * Modifie/utilise (globale) :
 * - Utilise `dac` dans `hardware_drivers` pour définir la consigne de courant.
 * Notes : Cette commande ajuste la consigne de courant à la valeur spécifiée. Si l'argument est invalide, la commande échoue.
 */
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

/**
 * Nom : restart
 * Fonction : Commande pour redémarrer le système ESP. Affiche un ACK avant de redémarrer l'appareil.
 * Arguments de commande attendus :
 * - Aucun argument.
 * Argument(s) de retour :
 * - bool : Retourne `true` après l'affichage de l'ACK et le redémarrage du système.
 * Modifie/utilise (globale) :
 * - Effectue un redémarrage de l'ESP.
 * Notes : Cette commande redémarre l'appareil, ce qui rend le code suivant le redémarrage inaccessible.
 */
CMD_DEF(restart) {
    out.println(ACK);
    ESP.restart();
    return true; // vu que l'ESP redemare, le return est du code mort.
}

/**
 * Nom : run_analysis
 * Fonction : Lance une analyse en effectuant plusieurs itérations où la consigne de courant est modifiée et les capteurs sont mis à jour. 
 *            L'état du système est affiché à chaque itération.
 * Arguments de commande attendus :
 * - Aucun argument.
 * Argument(s) de retour :
 * - bool : Retourne `true` après l'achèvement de l'analyse et l'affichage des résultats.
 * Modifie/utilise (globale) :
 * - Utilise les données `sensor_data` dans `Context` pour afficher l'état du système.
 * Notes : Cette commande effectue une analyse sur plusieurs itérations et affiche des informations sur la puissance, la température et l'état des capteurs à chaque itération.
 */
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

void run_commands(Decodeur& cmdl, Context& ctx, Print& out) {
    if (!cmdl.isAvailable()) {
        return;
    }

    const char received_cmd = toupper(cmdl.getCommand());

    for (auto cmd : commands) {
        if (cmd.code == received_cmd) {
            cmd.fn(cmdl, ctx, out);
            break;
        }
    }
}
