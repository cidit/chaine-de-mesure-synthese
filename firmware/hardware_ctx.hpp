#pragma once

#include "context.hpp"


/*
 * Nom : reset_hardware
 * Fonction : Réinitialise l'état des différents composants matériels du système, en les mettant dans un état "off" par défaut.
 *            Cela inclut les ventilateurs, l'interrupteur principal et le DAC, permettant ainsi de démarrer avec des composants 
 *            dans un état sûr avant toute autre opération.
 * Argument(s) réception :
 * - Context& ctx : L'objet `Context` qui contient les différents périphériques matériels et leur état, et qui permet de réinitialiser 
 *   leurs contrôles.
 * Argument(s) de retour : (rien)
 * Modifie/utilise (globale) : 
 * - `ctx.hardware_drivers` : Cette fonction modifie directement les états des contrôles des composants matériels (interrupteurs, ventilateurs, etc.).
 * Notes : Cette fonction est utilisée pour garantir un état stable des composants matériels avant toute interaction ou démarrage du système.
 */
void reset_hardware(Context&);

/*
 * Nom : init_hardware
 * Fonction : Initialise les différents composants matériels du système (ADC, DAC, capteurs, etc.). Cette fonction configure et vérifie 
 *            les périphériques, et indique s'ils sont correctement connectés. Elle initialise également la communication SPI et 
 *            configure le DAC.
 * Argument(s) réception :
 * - Context& ctx : L'objet `Context` qui contient les informations nécessaires pour initialiser les différents périphériques matériels.
 * - Print& out : L'objet `Print` pour afficher des messages d'état à l'utilisateur (via la sortie série).
 * - int adc_gain : Le gain à utiliser pour l'ADC afin de définir la plage de mesure.
 * - int mcp_chipselect : Le pin de sélection du chip pour le protocole SPI utilisé pour initialiser le DAC.
 * Argument(s) de retour : (rien)
 * Modifie/utilise (globale) : 
 * - `ctx.hardware_drivers` : Cette fonction configure et initialise les périphériques de hardware dans `ctx.hardware_drivers`.
 * Notes : Cette fonction est responsable de la configuration initiale de tous les périphériques matériels et assure leur bon fonctionnement 
 *         avant que le système ne commence à collecter des données ou à effectuer des opérations.
 */
void init_hardware(Context&, Print&, int, int);

/*
 * Nom : update_sensor_data
 * Fonction : Met à jour les données des capteurs dans l'objet `Context` en récupérant les valeurs les plus récentes des capteurs 
 *            connectés. Cette fonction mesure la température, la tension, le courant et la puissance, et les stocke dans `ctx.sensor_data`.
 * Argument(s) réception :
 * - Context& ctx : L'objet `Context` qui contient les données des capteurs et qui sera mis à jour avec les nouvelles valeurs.
 * Argument(s) de retour : (rien)
 * Modifie/utilise (globale) : 
 * - `ctx.sensor_data` : Cette fonction met à jour les données des capteurs (température, courant, etc.) dans la structure `sensor_data`.
 * - `ctx.hardware_drivers` : Cette fonction utilise les différents capteurs et périphériques matériels (ADC, INA219, DallasTemperature, etc.) 
 *   pour récupérer les nouvelles valeurs.
 * Notes : Cette fonction est cruciale pour la collecte continue des données des capteurs. Elle est appelée régulièrement pour maintenir 
 *         les informations à jour, permettant ainsi une gestion dynamique et réactive du système.
 */
void update_sensor_data(Context&);
