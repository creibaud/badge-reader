#include <Arduino.h>
#include <RFID.h>
#include <WIFI.h>
#include <iostream>

#include "secret.h"
#include "pin.h"
#include "api.h"

WIFI wifi(WIFI_SSID, WIFI_PASSWORD);
RFID rfid(RFID_SS_PIN, RFID_RST_PIN);

void setup() {
  Serial.begin(115200);
  rfid.init();

  delay(3000);
  wifi.connect();
}

void loop() {
  wifi.verifyConnection();

  if (rfid.readUid()) {
    if (rfid.isValid(String(API))) {
      Serial.println("RFID card is Valid\n");
    } else {
      Serial.println("RFID card is Invalid\n");
    }
  }

  delay(1000);
}