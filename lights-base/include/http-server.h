#ifndef YEETBOI
#define YEETBOI

#include <ESP8266WebServer.h>

extern ESP8266WebServer server;

void handleToggle();
void handleOn();
void handleOff();
void handleNotFound();

void startServer();

#endif
