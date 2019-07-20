
WiFiServer server(80);

// Variable to store the HTTP request
String clientHeader;

void setupWebServer() {
#ifdef DEBUG
  Serial.print("Setup Web Server");
#endif

  server.begin();

#ifdef DEBUG
  Serial.println("done");
#endif
}

void webPrintResponseHeader(WiFiClient *client, int responseCode) {
  client->print("HTTP/1.1");
  switch (responseCode) {
    case 200:
      client->println("200 OK");
      break;
    case 400:
      client->println("400 Bad Request");
      break;
    case 404:
      client->println("404 Not Found");
      break;
    case 500:
      client->println("500 Internal Server Error");
      break;
    default:
      client->println("500 Internal Server Error");
      break;
  }
  client->println("Content-type:text/html");
  client->println("Connection: close");
  client->println();
}

void webServerLoop() {
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        clientHeader += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // Return response header
            webPrintResponseHeader(&client);

            // Display the HTML web page
            webPrintHtmlHead(&client);
            webPrintHtmlBody(&client);

            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the clientHeader variable
    clientHeader = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
