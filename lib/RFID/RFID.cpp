#include "RFID.h"

RFID::RFID(byte _ssPin, byte _rstPin) : mfrc522(_ssPin, _rstPin) {
    uid = 0;
}

void RFID::init(String _api, String _username, String _password) {
    SPI.begin();
    mfrc522.PCD_Init();

    api = _api;
    username = _username;
    password = _password;
    token = "";
}

bool RFID::readUid() {
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        Serial.print("RFID card present: ");

        String stringUID = "";
        uid = 0;

        for (int i = 0; i < mfrc522.uid.size; i++) {
            stringUID += String(mfrc522.uid.uidByte[i], DEC);
        }

        uid = stringUID.toInt();

        Serial.println(stringUID);
        mfrc522.PICC_HaltA();

        Serial.println("");
        return true;
    }

    return false;
}

void RFID::loginAPI() {
    String url = "http://" + api + "/login";

    HTTPClient http;

    Serial.print("Login to API: ");
    Serial.println(url);
    http.begin(url);

    Serial.println("Sending POST request...");
    http.addHeader("Content-Type", "application/json");

    Serial.println("Sending JSON data...");
    String jsonData = "{\"username\":\"" + username + "\",\"password\":\"" + password + "\"}";

    int httpResponseCode = http.POST(jsonData);

    token = "";

    if (httpResponseCode == 200) {
        Serial.print("POST request success : ");
        Serial.println(httpResponseCode);

        String response = http.getString();
        Serial.println("Response: " + response);

        DynamicJsonDocument doc(1024);
        deserializeJson(doc, response);

        token = doc["accessToken"].as<String>();
        Serial.println("Access Token: " + token);
    } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);

        String response = http.getString();
        Serial.println("Response: " + response);
    }

    http.end();
}

bool RFID::postUid() {
    std::vector<int> uids = getUids();
    if (uid != 0 && find(uids.begin(), uids.end(), uid) != uids.end()) {
        String url = "http://" + api + "/addRFID";

        HTTPClient http;

        Serial.print("Sending RFID data to: ");
        Serial.println(url);
        http.begin(url);

        Serial.println("Sending POST request...");
        http.addHeader("Content-Type", "application/json");
        http.addHeader("Authorization", "Bearer " + token);

        Serial.println("Sending JSON data...");
        String jsonData = "{\"uid\":\"" + String(uid) + "\"}";

        int httpResponseCode = http.POST(jsonData);

        if (httpResponseCode == 200) {
            Serial.print("POST request success : ");
            Serial.println(httpResponseCode);

            String response = http.getString();
            return true;
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
    return false;
}

std::vector<int> RFID::getUids() {
    std::vector<int> uids = {};
    loginAPI();
    
    if (token != "") {
        String url = "http://" + api + "/getRFID";

        HTTPClient http;

        Serial.print("Getting RFID data from: ");
        Serial.println(url);
        http.begin(url);

        Serial.println("Sending GET request...");
        http.addHeader("Authorization", "Bearer " + token);
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
                        uids.push_back(uidValue.toInt());
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
    }
    Serial.println("");
    return uids;
}

void RFID::deleteUid() {
    std::vector<int> uids = getUids();
    if (uid != 0 && find(uids.begin(), uids.end(), uid) != uids.end()) {
        String url = "http://" + api + "/deleteRFID";

        HTTPClient http;

        Serial.print("Sending RFID data to: ");
        Serial.println(url);
        http.begin(url);

        Serial.println("Sending POST request...");
        http.addHeader("Content-Type", "application/json");
        http.addHeader("Authorization", "Bearer " + token);

        Serial.println("Sending JSON data...");
        String jsonData = "{\"uid\":\"" + String(uid) + "\"}";

        int httpResponseCode = http.POST(jsonData);

        if (httpResponseCode == 200) {
            Serial.print("POST request success : ");
            Serial.println(httpResponseCode);

            String response = http.getString();
        } else {
            Serial.print("Error code: ");
            Serial.println(httpResponseCode);

            String response = http.getString();
            Serial.println("Response: " + response);
        }

        http.end();
    } else {
        Serial.println("RFID card does not exist");
    }

    Serial.println("");
}

bool RFID::isValid() {
    std::vector<int> uids = getUids();
    return uid != 0 && find(uids.begin(), uids.end(), uid) != uids.end();
}

bool RFID::isAdministrator(int adminUid) {
    std::vector<int> uids = getUids();
    return uid != 0 && find(uids.begin(), uids.end(), adminUid) != uids.end();
}