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

//++StartVariables+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//--WifiVariables----------------------------------------------------------------

//--WifiScanVariables------------------------------------------------------------

//--WifiServerVariables----------------------------------------------------------
const char *ssid = "........";
const char *password = "........";

WebServer server(80);

const int led = 13;
//++EndVariables+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//==StartFuncions================================================================
//__WifiScanFunctions____________________________________________________________
void WifiScanSetup()
{
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Setup done");
}
void WifiScanLoop()
{
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
  {
    Serial.println("no networks found");
  }
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
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
void handleRoot()
{
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp32!");
  digitalWrite(led, 0);
}

void handleNotFound()
{
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void WifiServerSetup()
{
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32"))
  {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", []()
            { server.send(200, "text/plain", "this works as well"); });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}
void WifiServerLoop()
{
  server.handleClient();
  delay(2); //allow the cpu to switch to other tasks
}

//==EndFuncions==================================================================

void setup()
{
}

void loop()
{
}