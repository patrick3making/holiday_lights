#include <ESPmDNS.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include "secrets.h"

// Network credentials in secrets.h
//const char* ssid     = "SSID";
//const char* password = "Password";

void setupWifi() {
#ifdef WIFI_DEBUG
  Serial.print("Setup WiFi");
#endif

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(THING_BOARD_LED, LOW);
    delay(250);
    digitalWrite(THING_BOARD_LED, HIGH);
    delay(250);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("holidaylights")) {
    Serial.println("MDNS responder started");
  }

}
