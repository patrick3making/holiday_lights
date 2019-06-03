
WiFiServer server(80);

// Variable to store the HTTP request
String clientHeader;

void setupWebPage() {
#ifdef DEBUG
  Serial.print("Setup Web Page");
#endif

  server.begin();

#ifdef DEBUG
  Serial.println("done");
#endif
}

void webPrintResponseHeader(WiFiClient *client) {
  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:
  client->println("HTTP/1.1 200 OK");
  client->println("Content-type:text/html");
  client->println("Connection: close");
  client->println();
}

void webPrintWebSocketScript(WiFiClient *client) {
  client->print("\
<script>var connection = new WebSocket('ws://");
  client->print(WiFi.localIP());
  client->print("\
:81/', ['arduino']);\
connection.onopen = function () {  connection.send('Connect ' + new Date()); }; \
connection.onerror = function (error) {    console.log('WebSocket Error ', error);};\
connection.onmessage = function (e) {  console.log('Server: ', e.data);};\
function sendRGB(jscolor) {  var rgb = '#'+jscolor;\
console.log('RGB: ' + rgb); \
connection.send(rgb); }\
function sendPatternSelection(selection){ var pattern = '@'+selection;\
console.log('Pattern: ' + pattern);\
connection.send(pattern);}\
</script>\
<script src=\"https://cdnjs.cloudflare.com/ajax/libs/jscolor/2.0.4/jscolor.min.js\"></script>\
  ");
}

void webPrintHtmlHead(WiFiClient *client) {
  client->print("<html><head>");
  webPrintWebSocketScript(client);
  client->print("</head>");
}

String htmlPatternSelector() {
  String selectorHtml = "<select onchange=sendPatternSelection(this.value)>";

  for (int p = 0; p < ARRAY_SIZE( gPatterns); p++) {
    selectorHtml += "<option value=\"" + String(p) + "\"";
    if ( p == gCurrentPatternNumber) {
      selectorHtml += " selected";
    }
    selectorHtml += ">" + gPatternNames[p] + "</option>";
  }
  selectorHtml += "</select>";

  return selectorHtml;
}

void webPrintHtmlBody(WiFiClient *client) {
  client->print("<body>LED Control:<br/>\
  <br/><input class=\"jscolor {closable:true,closeText:'Close'}\" onchange=\"sendRGB(this.jscolor)\"  value=\"398085\">\
  ");
  client->print(htmlPatternSelector());
  client->println("\
<br/> \
</body> \
</html> \
");
  // The HTTP response ends with another blank line
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
