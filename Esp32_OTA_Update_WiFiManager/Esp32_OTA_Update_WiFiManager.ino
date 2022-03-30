/*--------------------------------------------------------*/
/*                                                        */
/*  Author: Farhan Tariq Janjua                           */
/*  Fiverr: https://www.fiverr.com/users/a1_developers/   */
/*                                                        */
/*--------------------------------------------------------*/

#include "WiFi.h"
#include <AsyncTCP.h>
#include "SPIFFS.h"
#include "SDDSerial.h"
#include "PubSubClient.h"
#include <WebServer.h>
#include <ElegantOTA.h>
#include <WiFiManager.h>      // https://github.com/tzapu/WiFiManager
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"


#define CONNECT_TIMEOUT 10000     // should connect in 20 sec 

const char *mqtt_publish_topic; //= "username/feeds/camera";
String publishTopic;
const char* mqttServer = "csmd.yourbsg.com";
const int mqttPort = 1883;
const char* mqttUser = "ubuntu";
const char* mqttPassword = "CSmd-2020BSG";

//const char* WIFI_SSID = "SSID"; // Your Wifi network name here
//const char* WIFI_PASS = "PW";    // Your Wifi network password here


WiFiClient espClient;
PubSubClient client(espClient);

WiFiManager wifiManager;
WebServer server(80);

char inst2_data;
String ser2_read;
String ser_read;// = Serial.read();
char inst_data;

//=====================================
//functions decleration

String processor(const String& var);

String mac = "";
String msgq = "";

void callback(char* topic, byte *payload, unsigned int length) {
  for (int i = 0; i < length; i++)
  {
   Serial2.print((char)payload[i]);
  }
  Serial2.println();
}

// uncomment all the above

void setup() {
  Serial.begin(115200);
  
  String MACaddress;
  MACaddress = WiFi.macAddress();
  Serial.print ("hello from ");
  Serial.println (MACaddress);
  publishTopic = MACaddress + "/result";//mac address of espe32/result
  mqtt_publish_topic = publishTopic.c_str();
  Serial.print("TOPIC:  ");
  Serial.println(mqtt_publish_topic);

  cmd_init(&Serial2);
  
  wifiManager.autoConnect("AutoConnectAP"); 
  Serial.println("AutoConnectAP started");  
  
  // Connect to Wi-Fi
//  WiFi.begin( WIFI_SSID, WIFI_PASS);
//
//  Serial.println("Connecting Wifi...");
//  uint32_t now = millis();
//  while (WiFi.status() != WL_CONNECTED)
//  {
//    delay(100);
//    Serial.print(".");
//    if ((millis() - now) > CONNECT_TIMEOUT)
//    {
//      ESP.restart();
//    }
//  }

  server.on("/", []() {
//  server.on("/", HTTP_GET, []() {
    server.send(200, "text/plain", "Hi! Maurice This shit works.");
  });

  Serial.println("Ready!");
  ElegantOTA.begin(&server);    // Start ElegantOTA
  server.begin();
  Serial.println("HTTP server started");
  
  Serial.println("");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Max Free Heap: ");
  Serial.println(ESP.getMaxAllocHeap());
  Serial.println(WiFi.macAddress());
  mac = (WiFi.macAddress());
  Serial.println("");
  Serial.println(F("Ready. Press d"));


  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
  ;

  //MQTT send message

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP32Client", mqttUser, mqttPassword )) {
      Serial.println("connected");
    } 
    else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(200);
    }
  }
  char espmac[33];
  char inespmac[28];
  String msg1 = "";
  String msg2 = "";
  msg1 = "hello mjr from " + mac;
  msg1.toCharArray(espmac, 33);
  msg2 = mac + "/cmd";
  msg2.toCharArray(inespmac, 28);
  client.subscribe(inespmac);
  
 // client.publish(espmac, "hello maurice");
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), mqttUser, mqttPassword)) {
      Serial.println("connected");

      client.setCallback(callback);
    } 
    
//    else {
//      Serial.print("failed, rc=");
//      Serial.print(client.state());
//      Serial.println(" try again in 5 seconds");
//      delay(5000);
//    }
  }

}

void publishSerialData(char *serialData) {
  if (!client.connected()) {
    reconnect();
  }

  String MACaddress;
  String publishTopic;
  MACaddress = WiFi.macAddress();
  publishTopic = MACaddress + "/result";//mac address of espe32/result
  mqtt_publish_topic = publishTopic.c_str();
  client.publish(mqtt_publish_topic, serialData);
}

void loop() {
  server.handleClient(); 
  client.loop();  

  TIMERG0.wdt_wprotect = TIMG_WDT_WKEY_VALUE;
  TIMERG0.wdt_feed = 1;
  TIMERG0.wdt_wprotect = 0;
 
  
  // The loop in this example acts as a serial passthrough for manual testing
  if (Serial.available() > 0)
  {
    inst_data = char(Serial.read());
    
    if (inst_data == 'a')
    {
      wifiManager.resetSettings();
      ESP.restart();    //Comment this line
      Serial2.println("ESP32 is Restarted.");
    }
    
    else if (inst_data == '\n') {
      Serial.print(ser_read);
    }
    else if (inst_data == ']')
    {
      ser_read = "";
    }
    else
    {
      ser_read += inst_data;
    }
    Serial2.write(inst_data);

  }
  
  if (Serial.available()) {     // If anything comes in Serial (USB),
    // read it and send it out Serial2
    Serial2.write(Serial.read());
  }

  if (Serial2.available() > 0) {
    char bfr[501];
    memset(bfr, 0, 501);
    Serial2.readBytesUntil( '\n', bfr, 500);
    publishSerialData(bfr);
  }
}
