#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "your_ssid_here";
const char* password = "your_password_here";

String main_page = "<HTML><form method=\"POST\" enctype=\"multipart/form-data\" action=\"upload\"><input type=\"file\" name=\"filename\"><br /><input type=\"submit\" value=\"LET'S DO THIS\"></form></HTML>";
String upload_page = "<HTML><script>;alert(\"javascript in this\")</script></HTML>";

unsigned int uploaded_bytes = 0;

void HandleIndex(){
  server.send(200, "text/html", main_page);
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
    Serial.printf(
      "uploading %s (%d bytes added to prior %d bytes)\n",
      upload.filename.c_str(),
      upload.currentSize,
      upload.totalSize
    );
    WriteEeprom(upload.totalSize, upload.buf, upload.currentSize);
    uploaded_bytes += upload.currentSize;
  }
  else if(upload.status == UPLOAD_FILE_END){
    Serial.printf("upload complete: %d bytes uploaded.\n",
      upload.totalSize
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

