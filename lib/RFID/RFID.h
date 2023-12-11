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
        bool postUid(String api);
        std::vector<String> getUids(String api);
        bool isValid(String api);
        bool isAdministrator(String api, String adminUid);
};

#endif // RFID_H