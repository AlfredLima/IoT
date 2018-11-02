#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h> 
#include <ESP8266HTTPUpdateServer.h> 
#include <PubSubClient.h>

#define DEBUG 1

#define DEBUGGING(...)    if(DEBUG) Serial.println( __VA_ARGS__ )
#define DEBUGGING_L(...)  if(DEBUG) Serial.print( __VA_ARGS__ )

unsigned long count     = 0;
const char* HOST        = "NODEMCU";
const char* ssid        = "VIVO-9B28";
const char* password    = "57601B9B28";
const char* NAME        = "TEST_NODEMCU";
const char* BROKER_MQTT = "192.168.15.5";
int BROKER_PORT         = 1883;

#define D0    16
#define D1    5
#define D2    4
#define D3    0
#define D4    2
#define D5    14
#define D6    12
#define D7    13
#define D8    15
#define D9    3
#define D10   1

#define PORT_URL  80
#define BAUD  115200

ESP8266HTTPUpdateServer updateOTA;
ESP8266WebServer serverWeb(PORT_URL);
WiFiClient wifiClient;
PubSubClient MQTT(wifiClient);

/*
    Inits
*/

void initWifi() {
  if ( WiFi.status() != WL_CONNECTED ) {
    WiFi.begin(ssid, password);
    reconnectWifi();
    initUpdateCode();
    serverWeb.on("/", handleRootPath); 
  }
}

void initUpdateCode() {
  updateOTA.setup(&serverWeb);
  serverWeb.begin();
  DEBUGGING("O servico de atualizacao remota (OTA) Foi iniciado com sucesso!");
  DEBUGGING_L("Abra http://");
  DEBUGGING_L(HOST);
  DEBUGGING(".local/update no seu browser para iniciar a atualizacao\n");
}

void initMDNS() {
  if (!MDNS.begin(HOST)) {
   DEBUGGING("Erro ao iniciar o servico mDNS!");
    while (1) {
      delay(1000);
    }
  }
  DEBUGGING("O servico mDNS foi iniciado com sucesso!");
  MDNS.addService("http", "tcp", PORT_URL);
}

void initMQTT() {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(mqtt_callback);
}

void initPins() {
  pinMode(D4, OUTPUT);
  digitalWrite(D4, LOW);
}

/*
    Reconnects
*/

void reconnectWifi() {
  if( WiFi.status() != WL_CONNECTED ) {
    DEBUGGING_L("Conectando ");
    while (WiFi.status() != WL_CONNECTED){
      delay(100);
      DEBUGGING_L(".");
    }  
    DEBUGGING();
    DEBUGGING("Conectado!");
    DEBUGGING(WiFi.localIP());
    initMDNS();
  }
}

void reconnectMQTT() {
  while (!MQTT.connected()) {
    DEBUGGING("Tentando se conectar ao Broker MQTT: " + String(BROKER_MQTT));
    if (MQTT.connect(NAME)) {
      DEBUGGING("Conectado");
      MQTT.subscribe("#");
    } else {
      DEBUGGING("Falha ao Reconectar");
      DEBUGGING("Tentando se reconectar em 2 segundos");
      delay(2000);
    }
  }
}

/*
    Setup
*/

void setup() {
  Serial.begin(BAUD);
  initPins();
  initWifi();
  initMQTT();
}

/*
    Loop
*/

void loop() {
  reconnectWifi();
  reconnectMQTT();
  serverWeb.handleClient();
  MQTT.loop();
}

/*
    MQTT
*/

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) {
    char c = (char)payload[i];
    message += c;
  }
  DEBUGGING("Topic: (" + String(topic) + ")\t\t\tValue: " + "(" + String(message) + ")");
  if( String(topic) == "home" ){
    led(message);
  }
}

/*
    Web server
*/

void handleRootPath() {
  serverWeb.send(200, "text/plain", "Hello NodeMCU " + String(++count));
}

/*
    Funções gerais
*/

void led(String message) {
  DEBUGGING("Chamou LED");
  digitalWrite(D4, message == "1" ? HIGH : LOW ); 
}