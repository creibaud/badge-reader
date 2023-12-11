#include <Arduino.h>
#include <RFID.h>
#include <WIFI.h>

#include "secret.h"
#include "pin.h"
#include "api.h"

WIFI wifi(WIFI_SSID, WIFI_PASSWORD);
RFID rfid(RFID_SS_PIN, RFID_RST_PIN);

void setup() {
  Serial.begin(115200);
  rfid.init();

  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);

  delay(3000);
  wifi.connect();
}

void loop() {
  wifi.verifyConnection();

  if (rfid.readUid()) {
    if (rfid.isValid(String(API))) {
      Serial.println("RFID card is Valid\n");
      digitalWrite(GREEN_LED_PIN, HIGH);
      digitalWrite(RED_LED_PIN, LOW);
    } else {
      Serial.println("RFID card is Invalid\n");
    }
  }

  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, HIGH);
  delay(1000);
}