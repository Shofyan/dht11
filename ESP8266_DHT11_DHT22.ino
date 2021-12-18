
const int refresh=2;//3 seconds
// DHT settings starts
#include "DHT.h"
#define DHTPIN 2     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);
float tValue;// 
float hValue;// 


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "JerukManis" // Your WiFi SSID
#define STAPSK  "apelhijau" //Your WiFi password
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);


void sendTemp() {

  String page = "<!DOCTYPE html>";
  page +="    <meta http-equiv='refresh' content='";
  page += String(refresh);// how often temperature is read
  page +="'/>";  
  page +="<html>";
  page +="<body>"; 
  page +="<h1>Shofyan nodemcu 8266</h1>";    
  page +="<p style=\"font-size:50px;\">Temperature<br/>";  
  page +="<p style=\"color:red; font-size:50px;\">";
  page += String(tValue, 2);
  page +=" Celcius</p>";  
    page +="</br>";  
    page +="<p style=\"font-size:50px;\">Humidity<br/>";  
  page +="<p style=\"color:red; font-size:50px;\">";
  page += String(hValue, 2);
  page +="</p>";  
  page +="</body>";  
  page +="</html>";  
 server.send(200,  "text/html",page);

}

void handleNotFound() {
 
  String message = "File Not Found";
  message += "URI: ";
  message += server.uri();
  message += "Method: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "Arguments: ";
  message += server.args();
  message += "";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "";
  }
  server.send(404, "text/plain", message);

}

void setup(void) {
  // Robojax.com code for ESP8266 and DHT11 DHT22
   dht.begin();
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    uint8_t macAddr[6];
    WiFi.macAddress(macAddr);
    Serial.printf("Connected, mac address: %02x:%02x:%02x:%02x:%02x:%02x", macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
  }  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("robojaxDHT")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", sendTemp);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {

  server.handleClient();
  MDNS.update();
  float c = dht.readTemperature();// Read temperature as Celsius (the default)
  float h = dht.readHumidity();// Reading humidity 
  float f = dht.readTemperature(true);// Read temperature as Fahrenheit (isFahrenheit = true)
    Serial.println(c);
    Serial.println(h);
 tValue =c;
 hValue = h;
  delay(100);// change this to larger value (1000 or more) if you don't need very often reading
}
