#include <Arduino.h>

#include <ESP8266WiFiMulti.h>

#include <AsyncDelay.h>

#include "specifics.h"
#include "http-server.h"
#include "http-client.h"
#include "ISRs.h"

#ifndef STASSID1
#define STASSID1 "tammana-60-tc"
#define STASSID2 "tammana-60"
#define STAPSK   "8f13aa2acb"
#endif

ESP8266WiFiMulti WiFiMulti;

// To prevent consecutive clicks by accident
AsyncDelay buttonDelays[LIGHT_COUNT];


void setup() {

    Serial.begin(115200);

    Serial.println();
    Serial.println();
    Serial.print("Connecting to WiFi...");

    WiFi.mode(WIFI_STA);
    WiFi.hostname("esp" STR(BOARD_ID)); // for example, becomes "esp0"
    WiFiMulti.addAP(STASSID1, STAPSK);
    WiFiMulti.addAP(STASSID2, STAPSK);

    while (WiFiMulti.run() != WL_CONNECTED) {
        Serial.print('.');
        delay(500);
    }

    Serial.println();
    Serial.println();

    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // HTTP server
    startServer();
    Serial.println("HTTP server started");

    // Get LED states from server
    loadStates();

    // Setup LEDs and buttons
    for(byte i = 0; i < LIGHT_COUNT; i++) {
        pinMode(lightPins[i], OUTPUT);
        digitalWrite(lightPins[i], states[i]);

        attachInterrupt(digitalPinToInterrupt(buttonPins[i]), ISRlist[i], RISING);
        buttonDelays[i].start(400, AsyncDelay::MILLIS);
    }
}

void loop() {
    server.handleClient();

    for (uint8_t i = 0; i < LIGHT_COUNT; i++) {
        if (triggers[i]) {
            triggers[i] = false;

            if (buttonDelays[i].isExpired()) {
                Serial.printf("CLICK: lightID: %s\n", lightIDs[i].c_str());

                toggleLight(i);
            }

            buttonDelays[i].restart();
        }
    }
}
