#include "RFID.h"

// Constructeur
RFID::RFID(byte _ssPin, byte _rstPin) : mfrc522(_ssPin, _rstPin) {
    uid = 0;
}

// Initialisation des paramètres RFID et API
void RFID::init(String _api, String _username, String _password) {
    SPI.begin();
    mfrc522.PCD_Init();

    api = _api;
    username = _username;
    password = _password;
    token = "";
}

// Lire l'UID du badge
bool RFID::readUid() {
    // Vérifier si un badge est présent
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        Serial.print("RFID card present: ");

        // Construire une chaîne de caractères représentant l'UID
        String stringUID = "";
        uid = 0;

        for (int i = 0; i < mfrc522.uid.size; i++) {
            stringUID += String(mfrc522.uid.uidByte[i], DEC);
        }
        
        // Convertir la chaîne d'UID en entier
        uid = stringUID.toInt();

        Serial.println(stringUID);

        // Arrêter la lecture du badge
        mfrc522.PICC_HaltA();

        Serial.println("");
        return true;
    }

    return false;
}

// Se connecter à l'API
void RFID::loginAPI() {
    // Construire l'URL pour la requête de connexion à l'API
    String url = "http://" + api + "/login";

    HTTPClient http;

    Serial.print("Login to API: ");
    Serial.println(url);
    http.begin(url);
    
    Serial.println("Sending POST request...");
    http.addHeader("Content-Type", "application/json");

    // Construire les données JSON avec le nom d'utilisateur et le mot de passe
    Serial.println("Sending JSON data...");
    String jsonData = "{\"username\":\"" + username + "\",\"password\":\"" + password + "\"}";

    int httpResponseCode = http.POST(jsonData);

    token = "";

    if (httpResponseCode == 200) {
        Serial.print("POST request success : ");
        Serial.println(httpResponseCode);

        String response = http.getString();
        Serial.println("Response: " + response);

        // Extraire le jeton d'accès de la réponse JSON
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

// POST request
bool RFID::postUid() {
    // Obtenir la liste des UID déjà enregistrés
    std::vector<int> uids = getUids();

    // Vérifier si l'UID est valide et n'existe pas déjà
    if (uid != 0 && find(uids.begin(), uids.end(), uid) == uids.end()) {
        // Construire l'URL pour la requête POST
        String url = "http://" + api + "/addRFID";

        HTTPClient http;

        Serial.print("Sending RFID data to: ");
        Serial.println(url);
        http.begin(url);

        Serial.println("Sending POST request...");
        http.addHeader("Content-Type", "application/json");
        http.addHeader("Authorization", "Bearer " + token);

        // Construire les données JSON avec l'UID
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

// GET request
std::vector<int> RFID::getUids() {
    std::vector<int> uids = {};
    loginAPI();
    
    if (token != "") {
        // Construire l'URL pour la requête GET
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
            
            // Analyser la réponse JSON
            DynamicJsonDocument doc(1024);
            deserializeJson(doc, response);

            if (doc.is<JsonArray>()) {
                JsonArray array = doc.as<JsonArray>();
                Serial.println("RFID data received:");

                for (JsonVariant elt : array) {
                    if (elt.is<JsonObject>()) {
                        // Extraire chaque UID de l'objet JSON
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

// DELETE request (POST request)
void RFID::deleteUid() {
    // Obtenir la liste des UID déjà enregistrés
    std::vector<int> uids = getUids();

    // Vérifier si l'UID est valide et existe
    if (isValid()) {
        // Construire l'URL pour la requête DELETE
        String url = "http://" + api + "/deleteRFID";

        HTTPClient http;

        Serial.print("Sending RFID data to: ");
        Serial.println(url);
        http.begin(url);

        Serial.println("Sending POST request...");
        http.addHeader("Content-Type", "application/json");
        http.addHeader("Authorization", "Bearer " + token);

        // Construire les données JSON avec l'UID
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

// Vérifier si l'UID actuel est valide
bool RFID::isValid() {
    // Obtenir la liste des UID déjà enregistrés
    std::vector<int> uids = getUids();
    // Vérifier si l'UID actuel existe dans la liste
    return uid != 0 && find(uids.begin(), uids.end(), uid) != uids.end();
}

bool RFID::isAdministrator(int adminUid) {
    // Obtenir la liste des UID déjà enregistrés
    std::vector<int> uids = getUids();
    // Vérifier si l'UID actuel existe dans la liste et qu'il est un administrateur
    return uid != 0 && find(uids.begin(), uids.end(), adminUid) != uids.end();
}