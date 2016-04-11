#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

String main_page = "<HTML><form method=\"POST\" enctype=\"multipart/form-data\" action=\"upload\"><input type=\"file\" name=\"filename\"><br /><input type=\"submit\" value=\"LET'S DO THIS\"></form></HTML>";
String upload_page = "<HTML><script>;alert(\"javascript in this\")</script></HTML>";

// These are used during conversion of ASCII hex to bytes
unsigned int line_number = 1;
unsigned int chars_in_line = 0;
unsigned int uploaded_bytes = 0;

void HandleIndex(){
  server.send(200, "text/html", main_page);
}

String ascii_to_bytes(uint8_t* ascii, int len){

  int i,j;
  String returner;
  char newchar = 0x00;
  for(j=0; j<len; ++j){
    switch(ascii[j]){
      case '#':
        // Handle comments
        if(chars_in_line == 0){
          while(j<len && ascii[j] != '\n'){
            ++j;
          }
          line_number += 1;
        }
        else{
          Serial.printf(
            "'#' character not in initial position on line %d; ignoring. '#' characters must start at the beginning of a line to indicate a comment.\n",
            line_number
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
        if(chars_in_line % 2 == 1){
          Serial.printf("Odd number of ASCII hex characters (%d) on line %d; ignoring final nibble.\n",
            chars_in_line,
            line_number
          );
          newchar = 0x00;
        }
        line_number += 1;
        chars_in_line = 0;
        break;
      case ' ':
        // Whitespace; ignore.
        break;
      case '\t':
        // Whitespace; ignore.
        break;
      case '\r':
        // Whitspace; ignore.
        break;
      default:
        Serial.printf(
          "Illegal character '%c' on line %d: Not an ASCII hex character or recognized whitespace. Ignoring.\n",
          ascii[j],
          line_number
        );
        break;
    }
    if(chars_in_line % 2 == 0 && chars_in_line > 0){
      returner += newchar;
      newchar = 0x00;
    }
  }

  return returner;
}

void HandleUpload(){

  // Where is the documentation for HTTPUpload?
  HTTPUpload& upload = server.upload();

  if(upload.status == UPLOAD_FILE_START){
    Serial.println("Stopping execution for upload.");
    StopExecution();
    uploaded_bytes = 0;
  }
  else if(upload.status == UPLOAD_FILE_WRITE){
    int j;
    Serial.printf(
      "uploading %s (%d bytes added to prior %d bytes)\n",
      upload.filename.c_str(),
      upload.currentSize,
      upload.totalSize
    );
    String data = ascii_to_bytes(
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
    for(j=0; j<data.length(); ++j){
      data_bytes[j] = data[j];
    }

    Serial.printf(
      "Writing to EEPROM index %d...",
      uploaded_bytes
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
  else if(upload.status == UPLOAD_FILE_END){
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

void HandleUploadRequest(){

  HTTPUpload& upload = server.upload();

  String success_message = String("Upload initaited for file ");
  success_message += String(upload.filename);
  server.send(200, "text/plain", success_message);
}

void StartWebServer(ESP8266WebServer &server){

  Serial.println("Starting HTTP server...");

  // ensure we're in station mode
  WiFi.mode(WIFI_STA);

  Serial.print("Connecting to network");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED){
      delay(500);
      Serial.print(".");
  }
  Serial.println("Connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // URL dispatching
  server.on("/", HTTP_GET, HandleIndex);
  server.on("/upload", HTTP_POST, HandleUploadRequest, HandleUpload);

  // start server
  server.begin();
  Serial.println("HTTP server started.");
}

