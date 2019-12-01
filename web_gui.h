
char *replaceAll( char *page,  char *search,  char *replace) 
{ 
    char *result; 
    int i, cnt = 0; 
    int newWlen = strlen(replace); 
    int oldWlen = strlen(search); 
  
    for (i = 0; page[i] != '\0'; i++) 
    { 
        if (strstr(&page[i], search) == &page[i]) 
        { 
            cnt++; 
            i += oldWlen - 1; 
        } 
    } 

    result = (char *)malloc(i + cnt * (newWlen - oldWlen) + 1); 
  
    i = 0; 
    while (*page) 
    { 
        if (strstr(page, search) == page) 
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

char *htmlPatternSelector(bool error) {
  char *selectorHtml;
  if (error){
    strcat(selectorHtml,"<h2>Selected Pattern Not Available</h2><br/>")
  }
  strcat(selectorHtml, "<form action=\"/pattern\" method=\"get\"><br/><p>Pattern Selector</p><select name=\"pattern\">");

  for (int patternItem = 0; patternItem < ARRAY_SIZE( gPatterns); patternItem++) {
    strcat(selectorHtml,"<option value=\"");
    char index[2];
    strcat(selectorHtml, itoa(patternItem, index, 10));
    strcat(selectorHtml, "\"");
    if ( patternItem == gCurrentPatternNumber) {
      strcat(selectorHtml, " selected");
    }
    strcat(selectorHtml, ">");
    strcat(selectorHtml, gPatternNames[patternItem]);
    strcat(selectorHtml, "</option>");
  }
  strcat(selectorHtml, "</select></br><input type=\"submit\" value=\"Make it so\"></form>");

  return selectorHtml;
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

char *pageBuilder(char *title, char *description, char *content) {
  char *page = pageBase;
  page = replaceAll(page, "{{title}}", title);
  page = replaceAll(page, "{{description}}", description);
  page = replaceAll(page, "{{content}}", content);
  return page;
}

String homePage() {
  String page = pageBuilder("Welcome", "The landing page for the Holiday Lights controller", "<p>Holiday Lights Controller</p>");
  return page;
}

String patternPage(int returnCode) {
  bool error = true;
  if (returnCode != 200){
    error = false;
  }
  char *patternSelector = htmlPatternSelector(error);
  String page = pageBuilder("Pattern", "Select a pattern", patternSelector);
  return page;
}

char *powerPage(bool alteration){
  char *powerStatus;
  strcat(powerStatus, "The power is ");
  char *message;
  if( powerOn){
    strcat(powerStatus, "on");
  }
  else
  {
    strcat(powerStatus, "off");
  }
  
  if(alteration){
    strcat(message, "<h1>Sucessfully set power state.</h1></br>");
  }

  strcat(message, "<p>");
  strcat(message, powerStatus);
  strcat(message, "</p>");

  char *page;
  page = pageBuilder("Power", powerStatus, message);
  return page;
}

