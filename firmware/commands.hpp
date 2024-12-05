#pragma once

#include <Decodeur.h>
#include "context.hpp"


namespace commands {

  /**
   * 
   */
  using CommandFunctionPtr = bool (*)(Decodeur&, Context&, Print&);

  /**
   * 
   */
  #define CMD_DEF(cmd_name) bool cmd_name(Decodeur &cmdl, Context &ctx, Print &out)

  /**
   * 
   */
  const char ACK = '*', NACK = '?';

  CMD_DEF(read_current);
  CMD_DEF(read_tension_vbus);
  CMD_DEF(read_power);
  CMD_DEF(read_electrical_circuit); // TODO: kind of weird on protocol, ask my boy Yanick about it
  CMD_DEF(read_tachymetry);         // PAS IMPLÉMENTÉ
  CMD_DEF(read_temperature);
  CMD_DEF(read_fan);
  CMD_DEF(read_main_switch);
  CMD_DEF(read_current_target);
  CMD_DEF(read_program_name);
  CMD_DEF(show_menu);
  CMD_DEF(set_fan);
  CMD_DEF(set_main_switch);
  CMD_DEF(set_current_target);
  CMD_DEF(restart);

  struct Command {
    char code;
    CommandFunctionPtr fn;
    String help_text;
  };

  static const Command commands[15] = {
      {'X', read_current, "Commande de lecture du courant en mA."},
      {'V', read_tension_vbus, "Commande de lecture de la tension Vbus en V."},
      {'W', read_power, "Commande de lecture de la puissance en mW."},
      {'I', read_electrical_circuit, "Commande de lecture de l'ensemble électrique."},
      {'J', read_tachymetry, "Commande de lecture du tachymètre en RPM."},
      {'T', read_temperature, "[0|1|2] Commande de lecture de la température. (0=froid, 1=chaud, 2=ambiant)"},
      {'G', read_fan, "[0|1] Commande de lecture de l'état des ventilateurs. (0=froid, 1=chaud)"},
      {'D', read_main_switch, "Commande de lecture de l'état du commutateur principal."},
      {'L', read_current_target, "Commande de lecture de la consigne de courant en pourcentage."},
      {'P', read_program_name, "Commande de lecture du nom du programme et sa version."},
      {'M', show_menu, "Commande pour afficher le menu des commandes (pour utilisation humaine)."},
      {'F', set_fan, "[n] [s] Commande d'état du ventilateur. (n: 0=froid, 1=chaud) (s: 0=OFF, 1=ON)"},
      {'C', set_main_switch, "[0|1] Commande d’état du commutateur principal. (0=OFF, 1=ON)"},
      {'S', set_main_switch, "[0-100] Commande de consigne du courant en pourcentage"},
      {'R', restart, "Re-démarrage du ESP32"}
  };

  void run_commands(Decodeur&, Context&, Print&);

}