#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "LUDUSNET";
const char* password = "QAZ123ludus*";

ESP8266WebServer server(80);

const int led = 2;

void handleRoot() {
  digitalWrite(led, 1);

  String HTML;

  HTML = "<style>  body{background-color:#cac;} </style> <h1>Workshop de IoT e NodeMcu</h1> <hr><h3>Leitura</h3> ";
  HTML += "Porta A0: ";
  HTML += analogRead(A0);
  
  server.send(200, "text/html", HTML);
  digitalWrite(led, 0);
}

void handleNotFound() {

  String message = "PAGE NOT FOUND";
  server.send(404, "text/plain", message);
  
}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
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

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
