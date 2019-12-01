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
    server.send(200, "text/html", homePage());
  });
  
  server.on("/pattern", [] {
    int newSelection, returnCode;
    if(server.hasArg("pattern")){
      newSelection = atoi(server.arg("pattern"));
      if( newSelection < ARRAY_SIZE(gPatterns)){
        gCurrentPatternNumber = newSelection;
        returnCode = 200;
      }
      else{
        returnCode = 501;
      }
    }
    server.send(returnCode, "text/html", patternPage(returnCode));
  });
  
  server.on("/power", [] {
    String state;
    if(server.hasArg("state")){
      if(server.arg("state") == "on"){
        powerOn = true;
      }
      else if(server.arg("state") == "toggle"){
        powerOn = !powerOn;
      }
      else{
        powerOn = false;
      }
      server.send(200, "text/html", "<p>Success</p>");
    }
    else{
      server.send(400, "text/html", "<p>no state specified</p>");
    }

  });

  server.begin();

#ifdef DEBUG
  Serial.println("done");
#endif
}

void webServerLoop() {
  server.handleClient();
}
