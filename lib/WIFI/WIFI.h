#ifndef WIFI_H
#define WIFI_H

#include <Arduino.h>
#include <WiFi.h>

#include "../../include/pin.h"

class WIFI {
    private:
        String ssid;
        String password;
    public:
        WIFI();
        void init(String _ssid, String _password);
        void connect();
        bool isConnected() const;
        void verifyConnection();
};

#endif // WIFI_H