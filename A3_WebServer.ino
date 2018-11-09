/********************************************************
Snowflakes WiFi 
  A3_WebServer
    interactions with web
********************************************************/
String main_page = "<HTML><head><script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.12.0/jquery.min.js\"></script><script>$(function(){window.setInterval(get_status, 1000);}); function get_status() {var request = new XMLHttpRequest();request.onreadystatechange = function(){if (request.readyState === 4  && request.status === 200){var elt = document.getElementById(\"status\");elt.innerHTML = request.responseText;console.log(request.responseText);}};request.open(\"GET\", \"curr\", true);request.send();}</script></head><form method=\"POST\" enctype=\"multipart/form-data\" action=\"upload\"><input type=\"file\" name=\"filename\"><br /><input type=\"submit\" value=\"Upload pattern\"></form><div class=\"status_container\"><div class=\"status_header\">Recent statuses (most recent at top):</div> </ br><div id=\"status\">No Status Yet Recieved</div></div></HTML>";
  
// These are used during conversion of ASCII hex to bytes
unsigned int line_number = 1;
unsigned int chars_in_line = 0;
unsigned int uploaded_bytes = 0;

// These are state we need to carry over between upload blocks.
char fragment = 0x00;
char during_comment = 0;

void InitializeWebQueue()
{
  int j;
  for(j=0; j<MaxWebQueueSize; ++j)
  {
    WebQueue[j] = "";
  }
}

void WebPrint(String output)
{
  Serial.print(output);

  if(WebQueueSize >= MaxWebQueueSize)
  {
    int j;
    for(j=MaxWebQueueSize-2; j>=0; --j)
    {
      WebQueue[j+1] = WebQueue[j];
    }
    WebQueue[0] = output;
  }
  else
  {
    WebQueue[WebQueueSize++] = output;
  }
}

void StartWebServer(ESP8266WebServer &server)
{

  Serial.println("Starting HTTP server...");

  // ensure we're in station mode
  WiFi.mode(WIFI_STA);

  Serial.print("Connecting to network...");
  WiFi.begin(Ssid, Password);

  AllPWMOff();
  int attempts = 0;
  int max_attempts = 50;
  int attempt_delay = 500; // milliseconds
  while (WiFi.status() != WL_CONNECTED)
  {
    if(attempts >= max_attempts)
    {
      Serial.printf(
        "\nGave up after %d attempts (~%ds). Are the SSID and password correct?\n",
        max_attempts,
        (max_attempts * attempt_delay) / 1000
      );
      Serial.print("Failed to start HTTP server.\n");
      
      AllPWMOff();
      delay(attempt_delay);
      WritePWMChannel(14, 0x1000, 0x0000);
      WritePWMChannel(15, 0x1000, 0x0000);
      delay(attempt_delay);
      AllPWMOff();
      delay(attempt_delay);
      WritePWMChannel(14, 0x1000, 0x0000);
      WritePWMChannel(15, 0x1000, 0x0000);
      delay(attempt_delay);
      AllPWMOff();
      
      return;
    }
    delay(attempt_delay);
    Serial.print(".");
    
    if(attempts % 2 == 0)
    {
      WritePWMChannel(14, 0x1000, 0x0000);
      WritePWMChannel(15, 0x0000, 0x1000);
    }
    else
    {
      WritePWMChannel(14, 0x0000, 0x1000);
      WritePWMChannel(15, 0x1000, 0x0000);
    }
    
    ++attempts;
  }
  Serial.println("CONNECTED");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  AllPWMOff();
  delay(attempt_delay);
  WritePWMChannel(14, 0x1000, 0x0000);
  WritePWMChannel(15, 0x1000, 0x0000);
  delay(attempt_delay);
  AllPWMOff();
  
  // URL dispatching
  server.on("/", HTTP_GET, HandleIndex);
  server.on("/upload", HTTP_POST, HandleUploadRequest, HandleUpload);
  server.on("/curr", HTTP_GET, HandleCurrStatus);

  // start server
  server.begin();
  Serial.println("HTTP server started.");
}

void HandleIndex()
{
  server.send(200, "text/html", main_page);
}

String AsciiToBytes(uint8_t* ascii, int len)
{

  int i,j;
  String returner;
  String current_line;
  char newchar;

  // Check for state held over from previous upload block
  if (fragment != 0x00)
  {
    newchar = fragment;
  }
  else
  {
    newchar = 0x00;
  }

  if (during_comment)
  {
    if (ascii[0] == '\n'){
      during_comment = 0;
      line_number += 1;
    }
    else{
      ascii[0] = '#';
    }
  }
  for(j=0; j<len; ++j)
  {
    switch(ascii[j])
    {
      case '#':
        // Handle comments
        if(chars_in_line == 0)
        {
          while(j<len && ascii[j] != '\n')
          {
            ++j;
            during_comment = 0;
          }
          if (j == len){
            // Mark the state as being in the middle of a comment
            // so we can skip the rest of it when we get it
            during_comment = 1;
          }
          else
          {
              line_number += 1;
          }
        }
        else
        {
          Serial.printf(
            "'#' character not in initial position on line %d (was %d); ignoring. '#' characters must start at the beginning of a line to indicate a comment.\n",
            line_number,
            chars_in_line
            //chars_in_line += 1;
          );
        }
      break;
      case '0':
        chars_in_line += 1;
        newchar = newchar| (0x00 << ((chars_in_line % 2)*4));
      break;
      case '1':
        chars_in_line += 1;
        newchar = newchar| (0x01 << ((chars_in_line % 2)*4));
      break;
      case '2':
        chars_in_line += 1;
        newchar = newchar| (0x02 << ((chars_in_line % 2)*4));
      break;
      case '3':
        chars_in_line += 1;
        newchar = newchar| (0x03 << ((chars_in_line % 2)*4));
      break;
      case '4':
        chars_in_line += 1;
        newchar = newchar| (0x04 << ((chars_in_line % 2)*4));
      break;
      case '5':
        chars_in_line += 1;
        newchar = newchar| (0x05 << ((chars_in_line % 2)*4));
      break;
      case '6':
        chars_in_line += 1;
        newchar = newchar| (0x06 << ((chars_in_line % 2)*4));
      break;
      case '7':
        chars_in_line += 1;
        newchar = newchar| (0x07 << ((chars_in_line % 2)*4));
      break;
      case '8':
        chars_in_line += 1;
        newchar = newchar| (0x08 << ((chars_in_line % 2)*4));
      break;
      case '9':
        chars_in_line += 1;
        newchar = newchar| (0x09 << ((chars_in_line % 2)*4));
      break;
      case 'A':
        chars_in_line += 1;
        newchar = newchar| (0x0A << ((chars_in_line % 2)*4));
      break;
      case 'B':
        chars_in_line += 1;
        newchar = newchar| (0x0B << ((chars_in_line % 2)*4));
      break;
      case 'C':
        chars_in_line += 1;
        newchar = newchar| (0x0C << ((chars_in_line % 2)*4));
      break;
      case 'D':
        chars_in_line += 1;
        newchar = newchar| (0x0D << ((chars_in_line % 2)*4));
      break;
      case 'E':
        chars_in_line += 1;
        newchar = newchar| (0x0E << ((chars_in_line % 2)*4));
      break;
      case 'F':
        chars_in_line += 1;
        newchar = newchar| (0x0F << ((chars_in_line % 2)*4));
      break;
      case 'a':
        chars_in_line += 1;
        newchar = newchar| (0x0A << ((chars_in_line % 2)*4));
      break;
      case 'b':
        chars_in_line += 1;
        newchar = newchar| (0x0B << ((chars_in_line % 2)*4));
      break;
      case 'c':
        chars_in_line += 1;
        newchar = newchar| (0x0C << ((chars_in_line % 2)*4));
      break;
      case 'd':
        chars_in_line += 1;
        newchar = newchar| (0x0D << ((chars_in_line % 2)*4));
      break;
      case 'e':
        chars_in_line += 1;
        newchar = newchar| (0x0E << ((chars_in_line % 2)*4));
      break;
      case 'f':
        chars_in_line += 1;
        newchar = newchar| (0x0F << ((chars_in_line % 2)*4));
      break;
      case '\n':
      if(chars_in_line % 2 == 1)
        {
          Serial.printf("Odd number of ASCII hex characters (%d) on line %d; ignoring final nibble.\n",
            chars_in_line,
            line_number
          );
          newchar = 0x00;
        }

        //DEBUG
        //Serial.printf("Line %d processed.\n", line_number);
        line_number += 1;
        chars_in_line = 0;

      break;
      case ' ':
        // Whitespace; ignore.
        continue;
      break;
      case '\t':
        // Whitespace; ignore.
        continue;
      break;
      case '\r':
        // Whitspace; ignore.
        continue;
      break;
      default:
        Serial.printf(
          "Illegal character '%c' on line %d: Not an ASCII hex character or recognized whitespace. Ignoring.\n",
          ascii[j],
          line_number
        );
      break;
    }
    if(chars_in_line % 2 == 0 && chars_in_line > 0)
    {
      returner += newchar;
      // DEBUG
      //Serial.printf("!*!*!* Added '%s' to returner\n", String(newchar, HEX).c_str());
      newchar = 0x00;
    }
  }

  // It's possible for upload blocks to deliver us only part of a line.
  // In that case, we can be cut off mid-byte. So, if we have the first
  // nibble of a byte and we need to wait for the second, save that here.
  if (newchar != 0x00)
  {
    fragment = newchar;
  }
  else
  {
    fragment = 0x00;
  }
  return returner;
}

void HandleUploadRequest()
{

  HTTPUpload& upload = server.upload();

  String success_message = String("Upload initaited for file ");
  success_message += String(upload.filename);
  server.send(200, "text/plain", success_message);
}

void HandleUpload()
{

  // Where is the documentation for HTTPUpload?
  HTTPUpload& upload = server.upload();

  if(upload.status == UPLOAD_FILE_START)
  {
    Serial.println("Stopping execution for upload.");
    StopExecution();
    // Set any state that might be hanging around to
    // initial settings
    uploaded_bytes = StartTableAddress;
    chars_in_line = 0;
    line_number = 1;
    during_comment = 0;
    fragment = 0x00;
  }
  else if(upload.status == UPLOAD_FILE_WRITE)
  {
    int j;
    Serial.printf(
      "uploading %s (%d bytes added to prior %d bytes)\n",
      upload.filename.c_str(),
      upload.currentSize,
      upload.totalSize
    );

    String data = AsciiToBytes(
      upload.buf,
      upload.currentSize
    );

    Serial.printf(
      "Converted %d bytes of ASCII to %d bytes. ",
      upload.currentSize,
      data.length()
    );

    // I don't know why String.getBytes() wasn't working,
    // but it wasn't, so we do a cast manually here.
    uint8_t* data_bytes = (uint8_t*)malloc(sizeof(uint8_t)*data.length());
    for(j=0; j<data.length(); ++j)
    {
      data_bytes[j] = data[j];
    }

    Serial.printf(
      "Writing to EEPROM index %d...",
      (uploaded_bytes - StartTableAddress)
    );

    WriteEeprom(
      uploaded_bytes,
      data_bytes,
      data.length()
    );
    uploaded_bytes += data.length();
    free(data_bytes);
    Serial.printf("Written.\n");
  }
  else if(upload.status == UPLOAD_FILE_END)
  {
    Serial.printf(
      "upload complete: %d bytes uploaded.\n",
      upload.totalSize
    );
    Serial.printf(
      "Total pattern size after conversion: %d\n",
      uploaded_bytes
    );
    Serial.println("Starting execution.");
    StartExecution();
  }
}

void HandleCurrStatus()
{
  int j;
  String output = "";
  for(j=0; j<WebQueueSize; ++j){
    output += "<p>" + WebQueue[j] + "</p>";
  }
  server.send(200, "text/plain", output);
}



