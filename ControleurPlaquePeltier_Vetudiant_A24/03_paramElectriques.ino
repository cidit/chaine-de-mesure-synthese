// --- INA219 ---

// Compléter l'entête
bool initialisationINA219() {
  // À COMPLÉTER
  return false;  // À compléter
}
// Compléter l'entête
float lectureCourantINA219() {
  // À COMPLÉTER
  return -1.0;  // À compléter
}
// Compléter l'entête
float lectureVoltageBusINA219() {
  // À COMPLÉTER
  return -1.0;  // À compléter
}
// Compléter l'entête
float lectureVoltageShuntINA219() {
  // À COMPLÉTER
  return -1.0;  // À compléter
}
// Compléter l'entête
float lecturePuissanceINA219() {
  // À COMPLÉTER
  return -1.0;  // À compléter
}
// Compléter l'entête
void envoieCourant() {
  Serial.println(courantINA219);
}
// Compléter l'entête
void envoieVoltageBus() {
  Serial.println(voltageBusINA219);
}
// Compléter l'entête
void envoiePuissance() {
  Serial.println(puissanceINA219);
}
// Compléter l'entête
void envoieLectureINA219() {
  Serial.print(String(voltageShuntINA219) + ";");
  Serial.print(String(voltageBusINA219) + ";");
  Serial.print(String(courantINA219) + ";");
  Serial.print(String(puissanceINA219) + ";");
  Serial.println(String(voltageBusINA219 - voltageShuntINA219 / 1000));
}
// Compléter l'entête
void envoieConsigneCourant() {
  Serial.println(String(consigneCourant) + "%");
}