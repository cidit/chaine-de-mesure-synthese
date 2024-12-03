

/* Étudiant.e.s: N'oubliez pas l'entête ET de vous assurer d'appliquer le modèle correctement et intégralement */


//---LIBRAIRIES----------------------------------------------------------

// Librairie "ADS1X15" par Rob Tillaart, version 0.5.1
#include <ADS1X15.h>

// Librairie "MCP_DAC" par Rob Tillaart, version 0.5.1
#include <MCP_DAC.h>

// Librairie "TGP Decodeur" par TechnoPhys, version 2.0.0
#include <Decodeur.h>

/// ET quelques autres - A VOUS DE COMPLÉTER

//-----------------------------------------------------------------------

//---DÉFINITIONS---------------------------------------------------------

// LabVIEW
#define ACK '*'    // Positive acknowledge / accusé de réception positif
#define NACK '?'   // Negative acknowledge / accusé de réception négatif

// Définition des broches du ESP32
#define MCP_CS_PIN 5                 // broche ChipSelect du bus SPI (MCP4921)
#define ADS_I2C_ADDR 0x48
#define broche_VentiloFroid 25    // identification de la broche de controle sur le ESP32 pour le ventilo froid
#define broche_VentiloChaud 26
#define broche_Commutateur 32
#define broche_DS18B20_data 35
#define broche_X 1  //autre broche? utiliser cette forme... sinon à enlever

// Vitesse de communication du moniteur série
#define SERMON_BAUDRATE 115200


/////////// CONSTANTES ////////////
// Placer ici toutes les constantes connues. AUCUN CHIFFRE EXPLICITE SANS REFERENCE DANS LE CODE!!!

//-----------------------------------------------------------------------

//---OBJETS--------------------------------------------------------------

ADS1115 ADS(ADS_I2C_ADDR);              // Quel est cet objet?

SPIClass * myspi = new SPIClass(VSPI);  // Instanciation d'un objet de la communication SPI
MCP4921 MCP(myspi);                     // Assignation de l'objet SPI au CNA MCP4921

Decodeur monDecodeur(&Serial);          // Quel est cet objet?

// Autres objets ici

//-----------------------------------------------------------------------


//---VARIABLES GLOBALES--------------------------------------------------

// Variables et constantes pour les minuteries
unsigned long minuterieLecture = millis();
const unsigned long periodeLecture = 100000;

// Ventilateurs
bool etatVentiloFroid = LOW;
bool etatVentiloChaud = LOW;

// Commutateur
bool etatCommutateur = LOW;

// Contrôle
float consigneCourant = 0.0;

// Données capteurs
float temperatureFroid;
float temperatureChaud;
float temperatureAmbiant;
float courantINA219;
float voltageBusINA219;
float voltageShuntINA219;
float puissanceINA219;

//-----------------------------------------------------------------------

//--- Prototypes --------------------------------------------------------
/* RIEN à modifier dans cette section */
/* Les prototypes ne sont que des déclarations préliminaires, utiles pour le compilateur, pas tant pour le/la programmeur-e */
bool  initialisationADS1115(int);
float lecture1ChADS1115(int);
float calculResistanceThermistance(float&,float&,float);
float calculTemperature(float);
float lectureTemperatureThermistance(int);
float lectureTemperatureDS18B20();
void  initialisationCommutateur(int);
void  commandeCommutateur(bool);
void  initialisationVentilo(int,int);
void  commandeVentilo(int,int);
bool  initialisationMCP();
void  commandeMCP(float);
bool  initialisationINA219();
float lectureCourantINA219();
float lectureVoltageBusINA219();
float lectureVoltageShuntINA219();
float lecturePuissanceINA219();
bool  lectureCmdLabview();
void  envoieEtatVentilo(int);
void  envoieEtatCommutateur();
void  envoieNomProgramme();
void  envoieCourant();
void  envoieVoltageBus();
void  envoiePuissance();
void  envoieLectureINA219();
void  envoieConsigneCourant();
void  envoieTemperature(int);
//-----------------------------------------------------------------------


//--- FONCTIONS -----------------------------------------------------------

// Les fonctions ont été regroupées par utilité dans des fichiers séparés, soients:
//   01_temperatures.ino: toutes les fonctions liées à la lecture et au traitement de la température
//   02_commandes.ino: toutes les fonctions liées aux diverses commandes: lecture et activation/désactivation
//   03_paramElectriques.ino: toutes les fonctions liées à la lecture et traitement des paramètres électriques

//-----------------------------------------------------------------------

//--- MAIN ----------------------------------------------------------------

void setup() {
  Serial.begin(SERMON_BAUDRATE);
  while (!Serial) { yield(); }
  delay(500);

  Serial.println("--- Initialisation ---");
  initialisationCommutateur(broche_Commutateur);
  commandeCommutateur(LOW);
  
  initialisationADS1115(0);
  initialisationINA219();

  initialisationVentilo(broche_VentiloFroid, broche_VentiloChaud);

  initialisationMCP();
  controleMCP(consigneCourant);

  Serial.println("--- Init complétée ---");
}

void loop() {

  // --- Lecture Capteurs ---
  if (millis() - minuterieLecture > periodeLecture) {
    minuterieLecture = millis();
    //Serial.println("tentative de lecture...");
    temperatureFroid = lectureTemperatureThermistance(0);
    temperatureChaud = lectureTemperatureThermistance(1);
    temperatureAmbiant = lectureTemperatureDS18B20();

    courantINA219 = lectureCourantINA219();
    voltageBusINA219 = lectureVoltageBusINA219();
    voltageShuntINA219 = lectureVoltageShuntINA219();
    puissanceINA219 = lecturePuissanceINA219();
    //Serial.println("Lecture complétée...");
  }

  // --- Communication avec LabVIEW ---
  lectureCmdLabview();
}
