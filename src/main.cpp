#include <Arduino.h>
#include <RFID.h>
#include <WIFI.h>
#include <Preferences.h>

#include "pin.h"
#include "api.h"

WIFI wifi;
RFID rfid(RFID_SS_PIN, RFID_RST_PIN);

Preferences preferences;

String WIFI_SSID;
String WIFI_PASSWORD;

int RFID_ADMIN;

String USERNAME;
String PASSWORD;

String StringToHex(const String& str) {
  String hexString = "";
  for (size_t i = 0; i < str.length(); i++) {
    char c = str.charAt(i);
    hexString += String(c, HEX);
  }
  return hexString;
}

String HexToString(const String& hex) {
  String str = "";
  for (size_t i = 0; i < hex.length(); i += 2) {
    String hexChar = hex.substring(i, i + 2);
    char c = strtol(hexChar.c_str(), NULL, 16);
    str += c;
  }
  return str;
}

void setup() {

  Serial.begin(115200);

  /*
  preferences.begin("wifi", false);
  preferences.putString("WIFI_SSID", StringToHex(WIFI_SSID));
  preferences.putString("WIFI_PASSWORD", StringToHex(WIFI_PASSWORD));
  preferences.end();

  preferences.begin("rfid", false);
  preferences.putString("RFID_ADMIN", String(RFID_ADMIN, HEX));
  preferences.end();

  preferences.begin("api", false);
  preferences.putString("USERNAME", StringToHex(USERNAME));
  preferences.putString("PASSWORD", StringToHex(PASSWORD));
  preferences.end();
  */

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

  rfid.init(API, USERNAME, PASSWORD);

  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  wifi.init(WIFI_SSID, WIFI_PASSWORD);

  delay(3000);
  wifi.connect();
}

void loop() {
  wifi.verifyConnection();

  if (rfid.readUid()) {
    if (rfid.isValid()) {
      if (rfid.isAdministrator(RFID_ADMIN) && digitalRead(BUTTON_PIN) == LOW) {
        Serial.println("RFID card is Administrator\n");
        digitalWrite(RED_LED_PIN, LOW);
        
        while (digitalRead(BUTTON_PIN) == LOW) {
          if (rfid.readUid()) {
            if (rfid.postUid()) {
              digitalWrite(GREEN_LED_PIN, HIGH);
              delay(2000);
            }
          }

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

      for (int i = 0; i < 3; i++) {
        digitalWrite(RED_LED_PIN, HIGH);
        delay(500);
        digitalWrite(RED_LED_PIN, LOW);
        delay(500);
      }
    }
  }

  if (wifi.isConnected()) {
    digitalWrite(YELLOW_LED_PIN, HIGH);
  } else {
    digitalWrite(YELLOW_LED_PIN, LOW);
  }

  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, HIGH);
  delay(500);
}