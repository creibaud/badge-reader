#include "WIFI.h"

WIFI::WIFI(const String _ssid, const String _password) 
    : ssid(_ssid), password(_password) {
}

void WIFI::connect() {
    Serial.println("Connecting to WiFi network: " + ssid);

    WiFi.begin(ssid, password);

    while (!isConnected()) {
        Serial.print(".");
        delay(1000);
    }

    Serial.println("Connected !");
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