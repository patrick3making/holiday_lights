
std::string htmlPatternSelector(bool error) {
#ifdef DEBUG
  Serial.println("pattern selector");
#endif
  std::string selectorHtml = "";
  if (error){
    selectorHtml += "<h2>Selected Pattern Not Available</h2><br/>";
  }
  selectorHtml += "<form action=\"/pattern\" method=\"get\"><br/><p>Pattern Selector</p><select name=\"pattern\">";

  for (int patternItem = 0; patternItem < ARRAY_SIZE( gPatterns); patternItem++) {
    selectorHtml += "<option value=\"";
    char index[2];
    selectorHtml += itoa(patternItem, index, 10);
    selectorHtml += "\"";
    if ( patternItem == gCurrentPatternNumber) {
      selectorHtml += " selected";
    }
    selectorHtml += ">";
    selectorHtml += gPatternNames[patternItem];
    selectorHtml += "</option>";
  }
  selectorHtml += "</select></br><input type=\"submit\" value=\"Make it so\"></form>";

  return selectorHtml;
}

char const PROGMEM *pageBase = R"=====(<!DOCTYPE html>
<html lang="en">

<head>
  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1">

  <title>Holiday Lights - {{title}}</title>

  <meta name="description" content="{{description}}">
  <meta name="author" content="patrick3making">

</head>

<body>
        <header>
          <h1 >Holiday Lights</h1>
        </header>
        <nav>
          <ul>
            <li>
              <a href="/home">Home</a>
            </li>
            <li>
              <a href="/pattern">Pattern</a>
            </li>
            <li>
              <a href="/power">Power</a>
            </li>
          </ul>
        </nav>
        <main>
          <h2>{{title}}</h2>
          {{content}}
        </main>
      </div>
    </div>
  </div>
</body>

</html>)=====";


std::string templateReplace(std::string pageTemplate, std::string templateField, std::string replacement){
  bool keepSearching = true;
  while(keepSearching){
    int foundIndex = pageTemplate.find(templateField);
    if(foundIndex!=std::string::npos){
      pageTemplate.replace(foundIndex,templateField.length(),replacement);
    }
    else{
      keepSearching = false;
    }
  }

  return pageTemplate;
}

std::string pageBuilder(std::string title, std::string description, std::string content) {
  std::string page = pageBase;
  page = templateReplace(page, "{{title}}", title);
  page = templateReplace(page, "{{description}}", description);
  page = templateReplace(page, "{{content}}", content);
  return page;
}

String homePage() {
  std::string page = pageBuilder("Welcome", "The landing page for the Holiday Lights controller", "<p>Holiday Lights Controller</p>");
  return page.c_str();
}

String patternPage(int returnCode) {
  bool error = true;
  if (returnCode = 200){
    error = false;
  }
  std::string patternSelector = htmlPatternSelector(error);
  std::string page = pageBuilder("Pattern", "Select a pattern", patternSelector);
  return page.c_str();
}

String powerPage(bool success){
  std::string powerStatus;
  std::string message;
  
  if(success){
    message += "<h1>Sucessfully set power state.</h1></br>";
  }
  
  if( powerOn){
    powerStatus += "<p>The power is on</p><br><button onclick=\"window.location.href = '/power?state=off';\">Power off</button>";
  }
  else
  {
    powerStatus += "<p>The power is off</p><br><button onclick=\"window.location.href = '/power?state=on';\">Power on</button>";
  }

  message += powerStatus;

  std::string page;
  page = pageBuilder("Power", powerStatus, message);
  return page.c_str();
}

