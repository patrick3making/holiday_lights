void replaceAll(std::string& str, const std::string& from, const std::string& to) {
  if (from.empty())
    return;
  size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
  }
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

const char PROGMEM pageBase[] = R"=====(<!DOCTYPE html>
<html lang="en">

<head>
  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1">

  <title>Holiday Lights - {{title}}</title>

  <meta name="description" content="{{description}}">
  <meta name="author" content="patrick3making">

  <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css">
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
  <script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.7/umd/popper.min.js"></script>
  <script src="https://maxcdn.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js"></script>

</head>

<body>
  <div class="container-fluid">
    <div class="row">
      <div class="col-md-12">
        <header class="page-header bg-primary text-white">
          <h1 class="text-center">Holiday Lights</h1>
          <nav class="bg-secondary">
            <ul class="nav text-white">
              <li class="nav-item">
                <a class="nav-link active text-white" href="/">Home</a>
              </li>
              <li class="nav-item">
                <a class="nav-link text-white" href="/schedule">Schedule</a>
              </li>
              <li class="nav-item dropdown ml-md-auto">
                <a class="nav-link dropdown-toggle text-white" href="/info" id="navbarDropdownMenuLink" data-toggle="dropdown">System</a>
                <div class="dropdown-menu dropdown-menu-right" aria-labelledby="navbarDropdownMenuLink">
                  <a class="dropdown-item" href="/settings">Settings</a>
                  <a class="dropdown-item" href="/info">Info</a>
                  <div class="dropdown-divider">
                  </div> <a class="dropdown-item" href="/about">About</a>
                </div>
              </li>
            </ul>
          </nav>
        </header>
      </div>
    </div>
    <div class="row">
      <div class="col-md-12">
        <main>
          <h2>{{title}}</h2>
          {{content}}
        </main>
      </div>
    </div>
  </div>
</body>

</html>)=====";

String homePage() {
  String page;
  page = pageBase;
  return page;
}
