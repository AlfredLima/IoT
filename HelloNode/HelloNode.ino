#define DEBUGGING(...) Serial.println( __VA_ARGS__ )
#define DEBUGGING_L(...) Serial.print( __VA_ARGS__ )

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h> 
#include <ESP8266HTTPUpdateServer.h> 

#ifndef DEBUGGING
#define DEBUGGING(...)
#endif
#ifndef DEBUGGING_L
#define DEBUGGING_L(...)
#endif

unsigned long count = 0;
const char* host      = "NODEMCU";
const char* ssid      = "VIVO-9B28";
const char* password  = "57601B9B28";

ESP8266HTTPUpdateServer updateOTA;
ESP8266WebServer serverWeb(80);

void setup() {
  Serial.begin(115200);
  initWifi();
  initMDNS();
  serverWeb.on("/", handleRootPath);
  initUpdateCode();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    initWifi();
    initMDNS();
  }
  serverWeb.handleClient();
}

void initUpdateCode() {
  updateOTA.setup(&serverWeb);
  serverWeb.begin();
  DEBUGGING("O servico de atualizacao remota (OTA) Foi iniciado com sucesso!");
  DEBUGGING_L("Abra http://");
  DEBUGGING_L(host);
  DEBUGGING(".local/update no seu browser para iniciar a atualizacao\n");
}

void initWifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(100);
  }  
  DEBUGGING("Conectado!");
  DEBUGGING(WiFi.localIP());
}

void initMDNS() {
  if (!MDNS.begin(host)) {
   DEBUGGING("Erro ao iniciar o servico mDNS!");
    while (1) {
      delay(1000);
    }
  }
  DEBUGGING("O servico mDNS foi iniciado com sucesso!");
  MDNS.addService("http", "tcp", 80);
}

void handleRootPath() {
  serverWeb.send(200, "text/plain", "Hello NodeMCU " + String(++count));
}
