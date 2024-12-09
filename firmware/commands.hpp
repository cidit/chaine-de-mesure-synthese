#pragma once

#include <Decodeur.h>
#include "context.hpp"


/**
 * // TODO
 */
#define CMD_DEF(cmd_name) bool cmd_name(Decodeur &cmdl, Context &ctx, Print &out)

namespace commands {

  /**
   * // TODO
   */
  using CommandFunctionPtr = bool (*)(Decodeur&, Context&, Print&);


  /**
   * // TODO
   */
  const char ACK = '*', NACK = '?';

  CMD_DEF(read_current);
  CMD_DEF(read_tension_vbus);
  CMD_DEF(read_power);
  CMD_DEF(read_electrical_circuit);
  CMD_DEF(read_tachymetry);         // NE SERA JAMAIS IMPLÉMENTÉ
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

  /**
   * // TODO
   */
  struct Command {
    char code;
    CommandFunctionPtr fn;
    String help_text;
  };

  /**
   * // TODO
   */
  const Command commands[15] = {
      {'X', read_current,            "Commande de lecture du courant en mAmps. {mA}"},
      {'V', read_tension_vbus,       "Commande de lecture de la tension Vbus en Volts. {V}"},
      {'W', read_power,              "Commande de lecture de la puissance en mWatt. {mW}"},
      {'I', read_electrical_circuit, "Commande de lecture de l'ensemble électrique. {shuntV;Vbus;courant;puissance;loadV}"},
      {'J', read_tachymetry,         "Commande de lecture du tachymètre en RPM. {rpm}"},
      {'T', read_temperature,        "[n] Commande de lecture de la température en celcius. [n: 0=froid, 1=chaud, 2=ambiant] {n celcius}"},
      {'G', read_fan,                "[n] Commande de lecture de l'état des ventilateurs. [n: 0=froid, 1=chaud] {0=OFF, 1=ON}"},
      {'D', read_main_switch,        "Commande de lecture de l'état du commutateur principal. {0=OFF, 1=ON}"},
      {'L', read_current_target,     "Commande de lecture de la consigne de courant en pourcentage. {n%}"},
      {'P', read_program_name,       "Commande de lecture du nom du programme et sa version. {nom-version}"},
      {'M', show_menu,               "Commande pour afficher le menu des commandes (pour utilisation humaine)."},
      {'F', set_fan,                 "[n] [s] Commande d'état du ventilateur. [n: 0=froid, 1=chaud] [s: 0=OFF, 1=ON]"},
      {'C', set_main_switch,         "[n] Commande d'état du commutateur principal. [n: 0=OFF, 1=ON]"},
      {'S', set_current_target,      "[n] Commande de consigne du courant en pourcentage. [n: 0-100]"},
      {'R', restart,                 "Commande de re-démarrage du ESP32."}
  };

  /**
   * // TODO
   */
  void run_commands(Decodeur&, Context&, Print&);

}