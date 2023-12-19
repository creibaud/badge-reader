#include <Arduino.h>
#include <RFID.h>
#include <WIFI.h>
#include <Preferences.h>
#include <ENCRYPTION.h>

#include "pin.h"
#include "api.h"

// Créer des instances des classes WIFI, RFID et Preferences
WIFI wifi;
RFID rfid(RFID_SS_PIN, RFID_RST_PIN);

Preferences preferences;

// Variables pour stocker les paramètres de configuration
String WIFI_SSID;
String WIFI_PASSWORD;
int RFID_ADMIN;
String USERNAME;
String PASSWORD;

void setup() {
  // Initialiser la communication série
  Serial.begin(115200);

  // Décommentez le bloc de code suivant pour définir les préférences initiales (doit être commenté après la première exécution)
  /*
  preferences.begin("wifi", false);
  preferences.putString("WIFI_SSID", StringToHex("Vôtre WIFI_SSID"));
  preferences.putString("WIFI_PASSWORD", StringToHex("Vôtre WIFI_PASSWORD"));
  preferences.end();

  preferences.begin("rfid", false);
  preferences.putString("RFID_ADMIN", String("Vôtre RFID_ADMIN", HEX));
  preferences.end();

  preferences.begin("api", false);
  preferences.putString("USERNAME", StringToHex("Vôtre USERNAME"));
  preferences.putString("PASSWORD", StringToHex("Vôtre PASSWORD"));
  preferences.end();
  */

  // Lire les paramètres de configuration à partir des préférences
  preferences.begin("wifi", false);
  WIFI_SSID = HexToString(preferences.getString("WIFI_SSID"));
  WIFI_PASSWORD = HexToString(preferences.getString("WIFI_PASSWORD"));
  preferences.end();

  preferences.begin("rfid", false);
  RFID_ADMIN = strtol(preferences.getString("RFID_ADMIN").c_str(), NULL, 16);
  preferences.end();

  preferences.begin("api", false);
  USERNAME = HexToString(preferences.getString("USERNAME"));
  PASSWORD = HexToString(preferences.getString("PASSWORD"));
  preferences.end();

  // Initialiser RFID avec API, USERNAME et PASSWORD
  rfid.init(API, USERNAME, PASSWORD);

  // Définir les modes de broches pour les LEDs et le bouton
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Initialiser et se connecter au WiFi
  wifi.init(WIFI_SSID, WIFI_PASSWORD);
  delay(3000); // Pour pouvoir avoir le temps de voir les Serial.println()
  wifi.connect();
}

void loop() {
  // Vérifier la connexion WiFi
  wifi.verifyConnection();

  // Vérifier si une carte RFID est présente
  if (rfid.readUid()) {
    // Vérifier si la carte RFID est valide
    if (rfid.isValid()) {
      // Vérifier si la carte RFID est un administrateur et si le bouton est enfoncé
      if (rfid.isAdministrator(RFID_ADMIN) && digitalRead(BUTTON_PIN) == LOW) {
        Serial.println("RFID card is Administrator\n");
        digitalWrite(RED_LED_PIN, LOW);
        
        // Boucle tant que le bouton est maintenu enfoncé
        while (digitalRead(BUTTON_PIN) == LOW) {
          // Lire à nouveau l'UID de la carte RFID
          if (rfid.readUid()) {
            // Envoyer l'UID au serveur si la lecture est réussie
            if (rfid.postUid()) {
              digitalWrite(GREEN_LED_PIN, HIGH);
              delay(2000);
            }
          }
          // Clignotement des LEDs pour indiquer l'attente
          digitalWrite(GREEN_LED_PIN, LOW);
          digitalWrite(YELLOW_LED_PIN, HIGH);
          delay(500);
          digitalWrite(YELLOW_LED_PIN, LOW);
          delay(500);
        }
      } else {
        Serial.println("RFID card is Valid\n");
        digitalWrite(GREEN_LED_PIN, HIGH);
        digitalWrite(RED_LED_PIN, LOW);
        delay(2000);
      }
    } else {
      Serial.println("RFID card is Invalid\n");
      digitalWrite(GREEN_LED_PIN, LOW);

      // Clignotement des LEDs pour indiquer une carte invalide
      for (int i = 0; i < 3; i++) {
        digitalWrite(RED_LED_PIN, HIGH);
        delay(500);
        digitalWrite(RED_LED_PIN, LOW);
        delay(500);
      }
    }
  }

  // Vérifier l'état de la connexion WiFi
  if (wifi.isConnected()) {
    digitalWrite(YELLOW_LED_PIN, HIGH);
  } else {
    digitalWrite(YELLOW_LED_PIN, LOW);
  }

  // Éteindre les LEDs verte et rouge, allumer la LED rouge pendant 500 ms
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, HIGH);
  delay(500);
}