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
        String uid;
    public:
        RFID(byte _ssPin, byte _rstPin);
        void init();
        bool readUid();
        String loginAPI(String api, String username);
        bool postUid(String api, String username);
        std::vector<String> getUids(String api, String username);
        void deleteUid(String api, String username);
        bool isValid(String api, String username);
        bool isAdministrator(String api, String username, String adminUid);
};

#endif // RFID_H