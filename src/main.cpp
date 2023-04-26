#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <LittleFS.h>

const char *host = "webhook.site";
const int httpsPort = 443;
const String postUrl = "/b1f90192-bc32-42b3-9f1e-3db2bf5c4ebf";

AsyncWebServer server(80);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // Turn off the LED
  Serial.begin(115200);

  WiFi.begin("oplus_co_apiofqg", "SALIS");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
    digitalWrite(LED_BUILTIN, LOW); // Turn on the LED
    delay(6000);
    digitalWrite(LED_BUILTIN, HIGH); // Turn off the LED
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    digitalWrite(LED_BUILTIN, LOW);
  }

  Serial.println("Connected to WiFi");

//https://www.willpeavy.com/tools/minifier/
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/html", "<!DOCTYPE html><html lang='en' class='bg-black overflow-hidden'><head> <meta charset='UTF-8'> <meta http-equiv='X-UA-Compatible' content='IE=edge'> <meta name='viewport' content='width=device-width, initial-scale=1.0'> <title>Kajaanitech NY </title> <script src='https://cdn.tailwindcss.com/3.3.1'></script></head><body> <div class='grid h-screen place-items-center text-white font-bold animate-pulse transition duration-200'> ESP8266 - Kajaanitech NY Runnaa! </div></body></html>"); });


  server.on("/json", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "application/json", "{\"moi\": \"terve\", \"vuosi\": 2023}"); });
  


  server.begin();
  Serial.println("HTTP server started");
}

bool alreadyConnected = false;

void loop()
{
  if (WiFi.status() == WL_CONNECTED && !alreadyConnected)
  {
    WiFiClientSecure client;
    client.setInsecure();

    if (!client.connect(host, httpsPort))
    {
      Serial.println("Connection failed");
      return;
    }

    String postBody = "WEBSocket Connected: ip=" + WiFi.localIP().toString();
    client.print(String("POST ") + postUrl + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Content-Type: application/x-www-form-urlencoded\r\n" +
                 "Content-Length: " + postBody.length() + "\r\n\r\n" +
                 postBody);

    delay(1000);

    while (client.connected())
    {
      String line = client.readStringUntil('\n');
      if (line == "\r")
      {
        break;
      }
    }

    alreadyConnected = true;

    String response = client.readString();
    Serial.println(response);
  }
}
