#include <Arduino.h>
//##StartIncludes################################################################
//--WifiIncludes------------------------------------------------------------------
#include <WiFi.h>
//--WifiScanIncludes-------------------------------------------------------------

//--WifiServerIncludes-----------------------------------------------------------
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
// const int LedStatus = 2; //LedStatus status WIFI
//--WifiScanVariables------------------------------------------------------------

//--WifiServerVariables----------------------------------------------------------
const char *ssid = "henrique";
const char *password = "grampovoador";
WebServer server(80);
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
//==EndFuncions==================================================================

void setup(){
  pinMode(LedStatus, OUTPUT);
  digitalWrite(LedStatus, 0);
  Serial.begin(115200);
  WifiServerSetup();
}

void loop(){
  WifiServerLoop();

}