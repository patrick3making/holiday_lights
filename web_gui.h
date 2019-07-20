
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
  String selectorHtml = "<br/><p>Pattern Selector</p><select onchange=sendPatternSelection(this.value)>";

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
  client->print("<body><h1>LED Control</h1><br/>\
  <p>Color picker</p><input class=\"jscolor {closable:true,closeText:'Close'}\" onchange=\"sendRGB(this.jscolor)\"  value=\"398085\">\
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
