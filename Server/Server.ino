#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "XXXX"; // <-- INSERIRE QUA SSID
const char* password = "XXXX"; // <-- INSERIRE QUA PASSWORD

ESP8266WebServer server(80);

void handleRoot() {
  if(digitalRead(0) == HIGH) { //ATTIVO BASSO
    server.send(200, "text/html", "<button onclick='location.href=\"/off\"'>TURN OFF</button>");
  } else {
    server.send(200, "text/html", "<button onclick='location.href=\"/on\"'>TURN ON</button>");
  }
}

void handleOn(){
  //Serial.println("served on");
  digitalWrite(0, HIGH);
  server.send(200, "text/html", "<button onclick='location.href=\"/off\"'>TURN OFF</button>");
}

void handleOff(){
  //Serial.println("served off");
  digitalWrite(0, LOW);
  server.send(200, "text/html", "<button onclick='location.href=\"/on\"'>TURN ON</button>");
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void){
  pinMode(0, OUTPUT);
  digitalWrite(0, LOW);
  //Serial.begin(115200);
  WiFi.begin(ssid, password);
  //Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
  /*Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
  */
  server.on("/", handleRoot);

  server.on("/on", handleOn);
  server.on("/off", handleOff);

  server.onNotFound(handleNotFound);

  server.begin();
  //Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}
