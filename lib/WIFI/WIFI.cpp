#include "WIFI.h"

WIFI::WIFI() {}

void WIFI::init(String _ssid, String _password) {
    ssid = _ssid;
    password = _password;
}

void WIFI::connect() {
    Serial.println("Connecting to WiFi network: " + ssid);

    WiFi.begin(ssid, password);

    while (!isConnected()) {
        Serial.print(".");
        digitalWrite(YELLOW_LED_PIN, HIGH);
        delay(500);
        digitalWrite(YELLOW_LED_PIN, LOW);
        delay(500);
    }

    Serial.println("Connected !");
    digitalWrite(YELLOW_LED_PIN, HIGH);
}

bool WIFI::isConnected() const {
    return WiFi.status() == WL_CONNECTED;
}

void WIFI::verifyConnection() {
    if (!isConnected()) {
        Serial.println("Deconnection detected !");
        connect();
    }
}