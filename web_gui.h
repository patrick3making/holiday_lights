
char *replaceAll( char *page,  char *seach,  char *replace) 
{ 
    char *result; 
    int i, cnt = 0; 
    int newWlen = strlen(replace); 
    int oldWlen = strlen(seach); 
  
    for (i = 0; page[i] != '\0'; i++) 
    { 
        if (strstr(&page[i], seach) == &page[i]) 
        { 
            cnt++; 
            i += oldWlen - 1; 
        } 
    } 

    result = (char *)malloc(i + cnt * (newWlen - oldWlen) + 1); 
  
    i = 0; 
    while (*page) 
    { 
        if (strstr(page, seach) == page) 
        { 
            strcpy(&result[i], replace); 
            i += newWlen; 
            page += oldWlen; 
        } 
        else
            result[i++] = *page++; 
    } 
  
    result[i] = '\0'; 
    return result; 
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

char PROGMEM *pageBase = R"=====(<!DOCTYPE html>
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
        <main>
          <h2>{{title}}</h2>
          {{content}}
        </main>
      </div>
    </div>
  </div>
</body>

</html>)=====";

String homePage(char *title, char *description, char *content) {
  char *page = pageBase;
  page = replaceAll(page, "{{title}}", title);
  page = replaceAll(page, "{{description}}", description);
  page = replaceAll(page, "{{content}}", content);
  return page;
}

String homePage() {
  String page = homePage("Welcome", "The landing page for the Holiday Lights controller", "<p>Holiday Lights Controller</p>");
  return page;
}