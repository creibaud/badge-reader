#include "WIFI.h"

WIFI::WIFI() {} // Constructeur

// Initialisation des paramètres WiFi
void WIFI::init(String _ssid, String _password) {
    ssid = _ssid;
    password = _password;
}

// Gestion de la connexion au réseau WiFi
void WIFI::connect() {
    Serial.println("Connecting to WiFi network: " + ssid);

    // Commencer la connexion au réseau WiFi avec les identifiants fournis
    WiFi.begin(ssid, password);

    // Attendre jusqu'à ce que la connexion soit établie
    while (!isConnected()) {
        Serial.print(".");

        // Clignoter la LED jaune pendant la tentative de connexion
        digitalWrite(YELLOW_LED_PIN, HIGH);
        delay(500);
        digitalWrite(YELLOW_LED_PIN, LOW);
        delay(500);
    }

    Serial.println("Connected !");
    digitalWrite(YELLOW_LED_PIN, HIGH);
}

// Vérifier si le module est connecté au réseau WiFi
bool WIFI::isConnected() const {
    return WiFi.status() == WL_CONNECTED;
}

// Vérifier la connexion WiFi
void WIFI::verifyConnection() {
    if (!isConnected()) {
        Serial.println("Deconnection detected !");
        connect();
    }
}