#ifndef RFID_H
#define RFID_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include <HTTPClient.h>
#include <vector>

// Classe pour gérer le module RFID et les connexions à l'API
class RFID {
    private:
        MFRC522 mfrc522;    // Instance du module RFID
        int uid;            // UID lu par le module RFID
        String api;         // URL de l'API
        String username;    // Nom d'utilisateur pour l'API
        String password;    // Mot de passe pour l'API
        String token;       // Token d'authentification pour l'API
    public:
        RFID(byte _ssPin, byte _rstPin);                            // Constructeur
        void init(String _api, String _username, String _password); // Initialisation des paramètres RFID et API
        bool readUid();                                             // Lire l'UID du badge
        void loginAPI();                                            // Se connecter à l'API
        bool postUid();                                             // Envoyer l'UID au serveur
        std::vector<int> getUids();                                 // Récupérer les UID enregistrés sur le serveur
        void deleteUid();                                           // Supprimer l'UID du serveur
        bool isValid();                                             // Vérifier si l'UID est valide
        bool isAdministrator(int adminUid);                         // Vérifier si l'UID est un administrateur
};

#endif // RFID_H