#include <WebServer.h>
#include "web_gui.h"
#include "static_files.h"

WebServer server(80);

// Variable to store the HTTP request
String clientHeader;

void setupWebServer() {
#ifdef DEBUG
  Serial.print("Setup Web Server");
#endif

  server.on("js/scripts.js", [] {
    server.send_P(200, "text/css", scriptsJs);
  });

  server.on("css/style.css", [] {
    server.send_P(200, "text/css", styleCss);
  });
  
  server.on("/", [] {
    server.send(200, "text/html", (String)homePage());
  });

  server.begin();

#ifdef DEBUG
  Serial.println("done");
#endif
}

void webServerLoop() {
  server.handleClient();
}
