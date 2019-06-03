
#include <WebSocketsServer.h>
#include <WebSockets.h>

WebSocketsServer webSocket = WebSocketsServer(81);

void webSocketLoop() {
  webSocket.loop();
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // send message to client
        webSocket.sendTXT(num, "Connected");
      }
      break;
    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\n", num, payload);

      if (payload[0] == '#') {
        // decode rgb data
        uint32_t rgb = (uint32_t) strtol((const char *) &payload[1], NULL, 16);
        //        showLEDColor(rgb);
      }
      else if (payload[0] == '@') {
        // decode data
        uint32_t pattern = (uint32_t) strtol((const char *) &payload[1], NULL, 16);
        gCurrentPatternNumber = pattern;
      }

      break;
  }

}

void setupWebSocket() {
  // start webSocket server
#ifdef DEBUG
  Serial.print("Setup Web Socket");
#endif

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

#ifdef DEBUG
  Serial.println("done");
#endif
}
