#ifndef WIFI_H
#define WIFI_H

#include <Arduino.h>
#include <WiFi.h>

class WIFI {
    private:
        const String ssid;
        const String password;
    public:
        WIFI(const String _ssid, const String _password);
        void connect();
        bool isConnected() const;
        void verifyConnection();
};

#endif // WIFI_H