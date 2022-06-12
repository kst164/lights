#include <Arduino.h>
#include "http-server.h"
#include "specifics.h"

ESP8266WebServer server(80);

void handleChange(String path) {
    Serial.println("handleChange, path: " + path);

    String message;

    for (int i = 0; i < server.args(); i++) {
        if (server.argName(i) == "lightID") {
            for (int j = 0; j < LIGHT_COUNT; j++) {
                if (server.arg(i) == lightIDs[j]) {
                    if (path == "/toggle") {
                        states[j] = !states[j];
                    } else if (path == "/set_on") {
                        states[j] = HIGH;
                    } else {
                        states[j] = LOW;
                    }
                    digitalWrite(lightPins[j], states[j]);

                    message += "lightID: " + lightIDs[j] + "  state: " + (states[j] ? "HIGH" : "LOW") + "\n";
                }
            }
        }
    }

    Serial.println("message: " + message);
    server.send(200, "text/plain", message);
}

void handleToggle() {
    handleChange("/toggle");
}

void handleOn() {
    handleChange("/set_on");
}

void handleOff() {
    handleChange("/set_off");
}

void handleNotFound(){
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET)?"GET":"POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i=0; i<server.args(); i++){
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);


    Serial.println("404. message: " + message);
}

void startServer() {

    server.on("/toggle", handleToggle);
    server.on("/set_on", handleOn);
    server.on("/set_off", handleOff);
    server.onNotFound(handleNotFound);

    server.begin();
}
