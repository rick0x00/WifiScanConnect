#include <Arduino.h>
//##StartIncludes################################################################
//--WifiIncludes------------------------------------------------------------------
#include <WiFi.h>
//--WifiScanIncludes-------------------------------------------------------------
#include <WiFi.h>
//--WifiServerIncludes-----------------------------------------------------------
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
//--HttpAdvancedAuthIncludes-----------------------------------------------------
#include <WiFi.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include <WebServer.h>
//--AdvancedWebServerIncludes-----------------------------------------------------
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
//##EndIncludes##################################################################


//**StartDefineVariables*********************************************************
//LedStatus status WiFi
#define LedStatus 2 
//**EndDefineVariables***********************************************************


//++StartVariables+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//--MsgPrintFunctionvariable-----------------------------------------------------
String msgtxt; //creating variable to traffic text messages;

//--WifiVariables----------------------------------------------------------------
//--WifiScanVariables------------------------------------------------------------

//--WifiServerVariables----------------------------------------------------------
const char *ssid = "henrique";
const char *password = "grampovoador";
WebServer server(80);
//--httpAdvancedAuthVariables----------------------------------------------------
const char* www_username = "admin";
const char* www_password = "esp32";
// allows you to set the realm of authentication Default:"Login Required"
const char* www_realm = "Custom Auth Realm";
// the Content of the HTML response in case of Unautherized Access Default:empty
String authFailResponse = "Authentication Failed";
//++EndVariables+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//==StartFuncions================================================================
//__Msgprintfuncion______________________________________________________________
void msg(String txt, int ln, int USBSerial, int BLESerial, int WIFISerial, int I2C16x2LCD){
  //function created to be responsible for any type of message;
  msgtxt = txt;
  //it is necessary to inform the text

  // PRINT USB SERIAL
  if (USBSerial == 0){
  }
  if (USBSerial == 1){
    if (ln == 0){
      Serial.print(msgtxt);
    }
    if (ln == 1){
      Serial.println(msgtxt);
    }
  }
  // PRINT BLE SERIAL
  if (BLESerial == 0){
  }
  if (BLESerial == 1){
    if (ln == 0){
      Serial.print(msgtxt);
    }
    if (ln == 1){
      Serial.println(msgtxt);
    }
  }
  // PRINT WIFI SERIAL
  if (WIFISerial == 0){
  }
  if (WIFISerial == 1){
    if (ln == 0){
      Serial.print(msgtxt);
    }
    if (ln == 1){
      Serial.println(msgtxt);
    }
  }
  // PRINT I2C 16x2 LCD
  if (I2C16x2LCD == 0){
  }
  if (I2C16x2LCD == 1){
    if (ln == 0){
      Serial.print(msgtxt);
    }
    if (ln == 1){
      Serial.println(msgtxt);
    }
  }
}
//__WifiScanFunctions____________________________________________________________
void WifiScanSetup(){
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  Serial.println("WifiScanSetup Setup done.");
}
void WifiScanLoop(){
  Serial.println("scan start");
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0){
    Serial.println("no networks found");
  }
  else{
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i){
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
      delay(10);
    }
  }
  Serial.println("");
  // Wait a bit before scanning again
  delay(5000);
}
//__WebServerFunctions___________________________________________________________
/*
void handleRoot(){
  digitalWrite(LedStatus, 1);
  server.send(200, "text/plain", "hello from esp32!");
  digitalWrite(LedStatus, 0);
}
void handleNotFound(){
  digitalWrite(LedStatus, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(LedStatus, 0);
}

void WifiServerSetup(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")){
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", [](){ server.send(200, "text/plain", "this works as well"); });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}
void WifiServerLoop(){
  server.handleClient();
  delay(2); //allow the cpu to switch to other tasks
}
*/
//__httpAdvancedAuthFunctions____________________________________________________
void HttpAdvancedAuthSetup(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Connect Failed! Rebooting...");
    delay(1000);
    ESP.restart();
  }
  ArduinoOTA.begin();

  server.on("/", []() {
    if (!server.authenticate(www_username, www_password))
      //Basic Auth Method with Custom realm and Failure Response
      //return server.requestAuthentication(BASIC_AUTH, www_realm, authFailResponse);
      //Digest Auth Method with realm="Login Required" and empty Failure Response
      //return server.requestAuthentication(DIGEST_AUTH);
      //Digest Auth Method with Custom realm and empty Failure Response
      //return server.requestAuthentication(DIGEST_AUTH, www_realm);
      //Digest Auth Method with Custom realm and Failure Response
    {
      return server.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse);
    }
    server.send(200, "text/plain", "Login OK");
  });
  server.begin();

  Serial.print("Open http://");
  Serial.print(WiFi.localIP());
  Serial.println("/ in your browser to see it working");
}
void HttpAdvancedAuthLoop(){
  ArduinoOTA.handle();
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
}
//__AdvancedWebServerFunctions____________________________________________________
void handleRoot() {
  digitalWrite(LedStatus, 1);
  char temp[400];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf(temp, 400,

           "<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>ESP32 Demo</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Hello from ESP32!</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <img src=\"/test.svg\" />\
  </body>\
</html>",

           hr, min % 60, sec % 60
          );
  server.send(200, "text/html", temp);
  digitalWrite(LedStatus, 0);
}
void handleNotFound() {
  digitalWrite(LedStatus, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
  digitalWrite(LedStatus, 0);
}
void drawGraph() {
  String out = "";
  char temp[100];
  out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"400\" height=\"150\">\n";
  out += "<rect width=\"400\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
  out += "<g stroke=\"black\">\n";
  int y = rand() % 130;
  for (int x = 10; x < 390; x += 10) {
    int y2 = rand() % 130;
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 10, 140 - y2);
    out += temp;
    y = y2;
  }
  out += "</g>\n</svg>\n";

  server.send(200, "image/svg+xml", out);
}
void AdvancedWebServerSetup(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/test.svg", drawGraph);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}
void AdvancedWebServerLoop(){
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
}
//==EndFuncions==================================================================

void setup(){
  pinMode(LedStatus, OUTPUT);
  digitalWrite(LedStatus, 0);
  Serial.begin(115200);
  AdvancedWebServerSetup();
}

void loop(){
  AdvancedWebServerLoop();
}