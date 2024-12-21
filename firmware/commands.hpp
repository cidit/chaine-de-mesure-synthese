#pragma once

#include <Decodeur.h>
#include "context.hpp"


/*
 * Nom : CMD_DEF
 * Fonction : Macro permettant de définir une fonction de commande. Ce macro génère une fonction de commande qui sera 
 *            utilisée pour manipuler des commandes reçues via un décodeur et un contexte donné.
 * Argument(s) réception : (Decodeur &cmdl, Context &ctx, Print &out)
 * - cmdl : Référence à l'objet `Decodeur` qui gère la commande reçue.
 * - ctx : Référence à l'objet `Context` qui contient les informations globales du système.
 * - out : Référence à l'objet `Print` utilisé pour envoyer la réponse (par exemple, via le port série).
 * Argument(s) de retour : vrais si la commande a eu aucune erreur, faux si il y a eu une erreur d'exécution
 * Modifie/utilise (globale) : (rien)
 * Notes : Ce macro est utilisée pour déclarer de manière uniforme des fonctions de commande.
 */
#define CMD_DEF(cmd_name) bool cmd_name(Decodeur &cmdl, Context &ctx, Print &out)

/*
 * Nom : CommandFunctionPtr
 * Fonction : Déclare un type pour un pointeur de fonction de commande. Ce type pointe vers des fonctions de commandes qui
 *            acceptent des paramètres `Decodeur`, `Context` et `Print`, et retournent un booléen tels que décrit plus hot.
 */
typedef bool (*CommandFunctionPtr)(Decodeur&, Context&, Print&);


/*
 * caractères utilisés pour indiquer respectivement une réponse "OK" (ACK) ou une réponse "erreur" (NACK).
 */
const char ACK = '*', NACK = '?';


/**
 * Ici, on définit une par une toutes les commandes qu'on s'attend à recevoir.
 * Les fonctions sont documentées individuellement dans `commands.cpp`.
 */
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
CMD_DEF(run_analysis);

/*
 * Nom : Command
 * Fonction : La structure `Command` représente une commande spécifique.
 * Notes : Chaque commande est définie par un code unique, une fonction de traitement et un texte d'aide pour l'utilisateur.
 */
struct Command {
  char code;
  CommandFunctionPtr fn;
  String help_text;
};

/*
 * Tableau contenant toutes les commandes disponibles pour le système. Chaque élément du tableau est une commande
 * avec son code, sa fonction associée et son texte d'aide.
 * Notes : Ce tableau sert à référencer toutes les commandes avec leurs détails respectifs (fonction, texte d'aide).
 */
const Command commands[16] = {
   {'X', read_current,            F("Commande de lecture du courant en mAmps. {mA}")},
   {'V', read_tension_vbus,       F("Commande de lecture de la tension Vbus en Volts. {V}")},
   {'W', read_power,              F("Commande de lecture de la puissance en mWatt. {mW}")},
   {'I', read_electrical_circuit, F("Commande de lecture de l'ensemble électrique. {shuntV;Vbus;courant;puissance;loadV}")},
   {'J', read_tachymetry,         F("Commande de lecture du tachymètre en RPM. {rpm}")},
   {'T', read_temperature,        F("[n] Commande de lecture de la température en celcius. [n: 0=froid, 1=chaud, 2=ambiant] {n celcius}")},
   {'G', read_fan,                F("[n] Commande de lecture de l'état des ventilateurs. [n: 0=froid, 1=chaud] {0=OFF, 1=ON}")},
   {'D', read_main_switch,        F("Commande de lecture de l'état du commutateur principal. {0=OFF, 1=ON}")},
   {'L', read_current_target,     F("Commande de lecture de la consigne de courant en pourcentage. {n%}")},
   {'P', read_program_name,       F("Commande de lecture du nom du programme et sa version. {nom-version}")},
   {'M', show_menu,               F("Commande pour afficher le menu des commandes (pour utilisation humaine).")},
   {'F', set_fan,                 F("[n] [s] Commande d'état du ventilateur. [n: 0=froid, 1=chaud] [s: 0=OFF, 1=ON]")},
   {'C', set_main_switch,         F("[n] Commande d'état du commutateur principal. [n: 0=OFF, 1=ON]")},
   {'S', set_current_target,      F("[n] Commande de consigne du courant en pourcentage. [n: 0-100]")},
   {'R', restart,                 F("Commande de re-démarrage du ESP32.")},
   {'A', run_analysis,            F("Commande pour lancer l'analyse. effectue 10 echantillons de temperature à des puissances differentes. Temps necessaire: 20 minutes") }
};

/*
 * Nom : run_commands
 * Fonction : Fonction principale qui exécute les commandes reçues via le décodeur. Elle analyse la commande et appelle 
 *            la fonction correspondante en fonction du code de la commande.
 * Argument(s) réception : 
 * - Decodeur& cmdl : L'objet `Decodeur` qui contient la commande à exécuter.
 * - Context& ctx : L'objet `Context` qui contient les informations globales du système.
 * - Print& out : L'objet `Print` utilisé pour envoyer des réponses ou des résultats via un port série ou une autre interface.
 * Argument(s) de retour : (rien)
 * Modifie/utilise (globale) : (rien)
 * Notes : Cette fonction traite les commandes reçues, vérifie la validité de la commande et exécute la fonction associée à chaque commande.
 */
void run_commands(Decodeur&, Context&, Print&);
