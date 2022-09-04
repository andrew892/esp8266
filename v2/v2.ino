#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>

const char* ssid = "XXXX"; // <-- INSERIRE QUA SSID
const char* password = "XXXX"; // <-- INSERIRE QUA PASSWORD
//WiFiServer telnetServer(8267);
ESP8266WebServer webServer(80);

int duration = 15000;

void handleRoot() {
//  if(webServer.client().remoteIP().toString() != "192.168.1.76") {
//    webServer.send(200, "text/html", "<h1>not authorized!</h1><br><h2>go fuck yourself</h2>");
//    return;
//  }
  int direc = webServer.arg("direc").toInt();
  int quant = webServer.arg("quant").toInt();
  if(abs(direc)!=1 || quant<0 || quant>100) {
    webServer.send(200, "text/html", "<h1>Parameters direct{1,-1} and timed[0-100] missing</h1>");
    return;
  } else {
    webServer.send(200, "text/html", "<h1>OK</h1>");
    
    int pin = -1; 
    if(direc > 0) { //in base al collegamento elettrico
      pin = 0;
    } else {
      pin = 2;
    }
    
    digitalWrite(pin,LOW);
    delay(duration*quant/100);
    digitalWrite(pin,HIGH);
  }
}

void setup() {
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(0, HIGH);
  digitalWrite(2, HIGH);
//  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA.setPort(8266);
  ArduinoOTA.setHostname("esp8266-test");
  ArduinoOTA.setPassword((const char *)"8266passwd");

  ArduinoOTA.begin();
  
//  telnetServer.begin();
  
  webServer.on("/", handleRoot);
  webServer.begin();
}

void loop() {
  ArduinoOTA.handle();

  webServer.handleClient();
  
//  if(telnetServer.hasClient()) {
//    WiFiClient telnetClient = telnetServer.available();
//    telnetClient.println("DEBUG");
//    telnetClient.stop();
//  }
}
