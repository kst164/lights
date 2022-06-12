#include "http-client.h"
#include "specifics.h"

HTTPClient http;
WiFiClient client;

const size_t capacity = JSON_OBJECT_SIZE(LIGHT_COUNT) + 10 * (1 + LIGHT_COUNT / 5);
DynamicJsonDocument jsonDoc(capacity);

bool loadStates() {
    String uri = "/esp_get_states?boardID=";
    uri += BOARD_ID;
    uri += "&ip=";
    uri += WiFi.localIP().toString();
    http.begin(client, MAIN_SERVER_ADDR, 8080, uri.c_str());
    int httpCode = http.GET();
    if (httpCode > 0) {
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK) {
            String payload = http.getString();
            Serial.print("payload: ");
            Serial.println(payload);

            DeserializationError err = deserializeJson(jsonDoc, payload);
            if (err) {
                Serial.println("JSON parse error: ");
                Serial.println(err.c_str());
            } else {
                for (int i = 0; i < LIGHT_COUNT; i++) {
                    states[i] = (bool) jsonDoc[lightIDs[i]];
                    Serial.printf("light %s: %s\n", lightIDs[i].c_str(), states[i] ? "HIGH" : "LOW");
                }
            }

            http.end();
            return true;
        } else {
            Serial.printf("HTTP Code %d\n", httpCode);
        }

    } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
    return false;
}

bool toggleLight(uint8_t i) {

    states[i] = !states[i];
    digitalWrite(lightPins[i], states[i]);

    String uri = states[i] ? "/set_on" : "/set_off";
    uri += "?sameDevice=1&lightID=" + lightIDs[i];

    Serial.print("URI: ");
    Serial.println(uri);

    http.begin(client, MAIN_SERVER_ADDR, 8080, uri.c_str());

    int httpCode = http.GET();
    if (httpCode > 0) {
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        if (httpCode == HTTP_CODE_OK) {
            String payload = http.getString();
            Serial.print("payload: ");
            Serial.println(payload);
            Serial.println();

            http.end();
            return true;
        } else {
            Serial.printf("HTTP Code %d\n\n", httpCode);
        }

    } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n\n", http.errorToString(httpCode).c_str());
    }

    http.end();
    return false;
}
