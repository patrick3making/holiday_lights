#include <WebServer.h>
#include "web_gui.h"
#include "static_files.h"

WebServer server(80);

// Variable to store the HTTP request
String clientHeader;

void setupWebServer()
{
#ifdef DEBUG
  Serial.print("Setup Web Server");
#endif

  server.on("js/scripts.js", [] {
#ifdef DEBUG
    Serial.print("scripts.js ... ");
#endif
    server.send_P(200, "text/css", scriptsJs);
#ifdef DEBUG
    Serial.println("done");
#endif
  });

  server.on("css/style.css", [] {
#ifdef DEBUG
    Serial.print("style.css ... ");
#endif
    server.send_P(200, "text/css", styleCss);
#ifdef DEBUG
    Serial.println("done");
#endif
  });

  server.on("/", [] {
#ifdef DEBUG
    Serial.print("home page ... ");
#endif
    server.send(200, "text/html", homePage());
#ifdef DEBUG
    Serial.println("done");
#endif
  });

  server.on("/pattern", [] {
#ifdef DEBUG
    Serial.print("pattern page ... ");
#endif
    int newSelection;
    int returnCode;
    if (server.hasArg("pattern"))
    {
      String patternArg = server.arg("pattern");
      newSelection = atoi(patternArg.c_str());
      if (newSelection < ARRAY_SIZE(gPatterns))
      {
        gCurrentPatternNumber = newSelection;
        returnCode = 200;
#ifdef DEBUG
        Serial.print("selected ");
        Serial.print(newSelection);
        Serial.print(" ... ");
#endif
      }
      else
      {
        returnCode = 501;
      }
    }
    String page = patternPage(returnCode);
    server.send(returnCode, "text/html", page.c_str());
#ifdef DEBUG
    Serial.println("done");
#endif
  });

  server.on("/power", [] {
#ifdef DEBUG
    Serial.print("power page ... ");
#endif
    String state;
    bool success = true;
    int returnCode = 200;
    if (server.hasArg("state"))
    {
      if (server.arg("state") == "on")
      {
        powerOn = true;
      }
      else if (server.arg("state") == "toggle")
      {
        powerOn = !powerOn;
      }
      else if (server.arg("state") == "off")
      {
        powerOn = false;
      }
      else{
        success = false;
        returnCode = 400;
      }
    }
      server.send(returnCode, "text/html", powerPage(success));
#ifdef DEBUG
    Serial.println("done");
#endif
  });

  server.begin();

#ifdef DEBUG
  Serial.println("done");
#endif
}

void webServerLoop()
{
  server.handleClient();
}
