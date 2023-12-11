#include "RFID.h"

RFID::RFID(byte _ssPin, byte _rstPin) : mfrc522(_ssPin, _rstPin) {
    uid = "";
}

void RFID::init() {
    SPI.begin();
    mfrc522.PCD_Init();
}

bool RFID::readUid() {
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        Serial.print("RFID card present: ");

        uid = "";

        for (int i = 0; i < mfrc522.uid.size; i++) {
            uid += (mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
            uid += String(mfrc522.uid.uidByte[i], HEX);
        }

        uid.remove(0, 1);
        uid.toUpperCase();
        Serial.println(uid);
        mfrc522.PICC_HaltA();

        Serial.println("");
        return true;
    }

    return false;
}

void RFID::postUid(String api) {
    std::vector<String> uids = getUids(api);
    if (uid != "" && find(uids.begin(), uids.end(), uid) == uids.end()) {
        String url = "https://" + api;

        HTTPClient http;

        Serial.print("Sending RFID data to: ");
        Serial.println(url);
        http.begin(url);

        Serial.println("Sending POST request...");
        http.addHeader("Content-Type", "application/json");

        Serial.println("Sending JSON data...");
        String jsonData = "{\"uid\":\"" + uid + "\"}";

        int httpResponseCode = http.POST(jsonData);

        if (httpResponseCode > 0) {
            Serial.print("POST request success : ");
            Serial.println(httpResponseCode);

            String response = http.getString();
            Serial.println("Response: " + response);
        } else {
            Serial.print("Error code: ");
            Serial.println(httpResponseCode);

            String response = http.getString();
            Serial.println("Response: " + response);
        }

        http.end();
    } else {
        Serial.println("RFID card already exists");
    }

    Serial.println("");
}

std::vector<String> RFID::getUids(String api) {
    std::vector<String> uids = {};
    String url = "https://" + api;

    HTTPClient http;

    Serial.print("Getting RFID data from: ");
    Serial.println(url);
    http.begin(url);

    Serial.println("Sending GET request...");
    int httpResponseCode = http.GET();

    if (httpResponseCode == 200) {
        Serial.println("GET request success");

        String response = http.getString();
        
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, response);

        if (doc.is<JsonArray>()) {
            JsonArray array = doc.as<JsonArray>();
            Serial.println("RFID data received:");

            for (JsonVariant elt : array) {
                if (elt.is<JsonObject>()) {
                    String uidValue = elt["uid"].as<String>();
                    Serial.println(uidValue);
                    uids.push_back(uidValue);
                } else {
                    Serial.println("Error: Each element in the array should be an object.");
                }
            }
        } else {
            Serial.println("Error: Response should be an array.");
        }
    } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);

        String response = http.getString();
        Serial.println("Response: " + response);
    }

    http.end();
    Serial.println("");
    return uids;
}

bool RFID::isValid(String api) {
    std::vector<String> uids = getUids(api);
    return uid != "" && find(uids.begin(), uids.end(), uid) != uids.end();
}