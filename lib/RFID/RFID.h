#ifndef RFID_H
#define RFID_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include <HTTPClient.h>
#include <vector>

class RFID {
    private:
        MFRC522 mfrc522;
        int uid;
        String api;
        String username;
        String password;
        String token;
    public:
        RFID(byte _ssPin, byte _rstPin);
        void init(String _api, String _username, String _password);
        bool readUid();
        void loginAPI();
        bool postUid();
        std::vector<int> getUids();
        void deleteUid();
        bool isValid();
        bool isAdministrator(int adminUid);
};

#endif // RFID_H