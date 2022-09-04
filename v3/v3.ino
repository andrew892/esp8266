#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>

const char* ssid = "xxxx"; // <-- INSERIRE QUA SSID
const char* password = "xxxx"; // <-- INSERIRE QUA PASSWORD
//WiFiServer telnetServer(8267);
int duration = 24000;
int pin_up = 0;
int pin_down = 2;
ESP8266WebServer webServer(80);

int state = -1;

void handleRoot() {
//  if(webServer.client().remoteIP().toString() != "192.168.1.76") {
//    webServer.send(200, "text/html", "<h1>not authorized!</h1><br><h2>go fuck yourself</h2>");
//    return;
//  }
  int perc = webServer.arg("perc").toInt();
  if(perc<1 || perc>100) {
    webServer.send(200, "text/html", "<h1>Parameter perc[0-100] missing</h1>");
    return;
  } else {
    webServer.send(200, "text/html", "<h1>OK, sono a "+String(state)+" e vado a "+String(perc)+"</h1>");
    
    if(state == -1) {
        digitalWrite(pin_up, LOW);
        delay(duration);
        digitalWrite(pin_up, HIGH);
        state = 100;
    }

    int dir = -1;
    if(state < perc) { // deve salire
      dir = pin_up;
    } else { // deve scendere
      dir = pin_down;
    }

    digitalWrite(dir, LOW);
    delay(duration*abs(state-perc)/100);
    digitalWrite(dir, HIGH);

    state = perc;
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
