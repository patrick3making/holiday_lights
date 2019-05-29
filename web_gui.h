
WiFiServer server(80);

// Variable to store the HTTP request
String header;

void setupWebPage() {
#ifdef DEBUG
  Serial.print("Setup Web Page");
#endif

  server.begin();
  
#ifdef DEBUG
  Serial.println("done");
#endif
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
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Display the HTML web page
            client.print("\
<html><head>\
<script>var connection = new WebSocket('ws://");
            client.print(WiFi.localIP());
            client.println("\
:81/', ['arduino']);\
connection.onopen = function () {  connection.send('Connect ' + new Date()); }; \
connection.onerror = function (error) {    console.log('WebSocket Error ', error);};\
connection.onmessage = function (e) {  console.log('Server: ', e.data);};\
function sendRGB(jscolor) {  var rgb = '#'+jscolor;\
console.log('RGB: ' + rgb); \
connection.send(rgb); }</script>\
<script src=\"https://cdnjs.cloudflare.com/ajax/libs/jscolor/2.0.4/jscolor.min.js\"></script>\
</head>\
<body>LED Control:<br/>\
<br/><input class=\"jscolor {closable:true,closeText:'Close'}\" onchange=\"sendRGB(this.jscolor)\"  value=\"398085\">\
<br/>\
</body>\
</html>\
");

            // The HTTP response ends with another blank line
            client.println();
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
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
