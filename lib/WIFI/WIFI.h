#ifndef WIFI_H
#define WIFI_H

#include <Arduino.h>
#include <WiFi.h>

#include "../../include/pin.h"

// Classe pour gérer le WiFi
class WIFI {
    private:
        String ssid;        // SSID du réseau WiFi
        String password;    // Mot de passe du réseau WiFi
    public:
        WIFI();                                     // Constructeur par défaut
        void init(String _ssid, String _password);  // Initialiser les paramètres WiFi
        void connect();                             // Se connecter au réseau WiFi
        bool isConnected() const;                   // Vérifier si le module est connecté au réseau WiFi
        void verifyConnection();                    // Vérifier la connexion WiFi
};

#endif // WIFI_H