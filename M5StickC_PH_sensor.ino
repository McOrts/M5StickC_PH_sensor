#include <M5StickC.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "settings.h"

// pH variables
const int analogInPin = 3 6; 
int sensorValue = 0; 
unsigned long int avgValue; 
float b;
int buf[10],temp;

// Communication variables
WiFiClient espClient;
PubSubClient clientMqtt(espClient);
String mqttcommand = String(14);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  Serial.begin(115200);
  // Initialize the M5StickC object
  M5.begin();
  pinMode(M5_BUTTON_HOME, INPUT);

  // Wifi
  setup_wifi();
  clientMqtt.setServer(mqtt_server, mqtt_port);
  clientMqtt.setCallback(callback);

  // Lcd display
  M5.Lcd.begin();
  M5.Lcd.print("Starting");
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(0,0);
  M5.Lcd.printf("pH sensor");
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setCursor(0,10);
  double vbat = M5.Axp.GetVbatData() * 1.1 / 1000;
  M5.Lcd.printf("Battery:%.3fV\r\n",vbat);  //battery voltage
  M5.Lcd.setTextColor(GREEN);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(30,30);
  M5.Lcd.print("Ph = ");
  M5.Lcd.setTextSize(3);
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("[MQTT]Incomming message (");
  Serial.print(topic);
  Serial.print(") ");
  mqttcommand = "";
  for (int i = 0; i < length; i++) {
    mqttcommand += (char)payload[i];
  }
  Serial.print(mqttcommand);
  Serial.println();
  // Switch on the LED if an 1 was received as first character
  if (mqttcommand == "LightOn") {
    Serial.println("LightOn");
  } 
}

void reconnect() {
  Serial.print("[MQTT]Intentando conectar a servidor MQTT... ");
  // Loop until we're reconnected
  while (!clientMqtt.connected()) {
    Serial.print(".");
    // Attempt to connect
    if (clientMqtt.connect(mqtt_id)) { // Ojo, para más de un dispositivo cambiar el nombre para evitar conflicto
      Serial.println("");
      Serial.println("[MQTT] connecting server...");
      // Once connected, publish an announcement...
      clientMqtt.publish(mqtt_sub_topic_healthcheck, "starting");
      // ... and subscribe
      clientMqtt.subscribe(mqtt_sub_topic_operation);
    } else {
      Serial.print("[MQTT]Error, rc=");
      Serial.print(clientMqtt.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  if (!clientMqtt.connected()) {
    reconnect();
  }
  clientMqtt.loop();
  
  for(int i=0;i<10;i++) 
  { 
    buf[i]=analogRead(analogInPin);
    Serial.print("AD = ");
    Serial.println(buf[i]);
    delay(10);
  }
  for(int i=0;i<9;i++)
    {
    for(int j=i+1;j<10;j++)
      {
      if(buf[i]>buf[j])
        {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)
    avgValue+=buf[i];
  //float pHVol=(float)avgValue*5.0/1024/6;
  float pHVol=(float)avgValue/6*3.4469/4095;
  Serial.print("v = ");
  Serial.println(pHVol);

  float phValue = -5.70 * pHVol + 21.34;    
  //float phValue = 7 + ((2.5 - pHVol) / 0.18);
  Serial.print("Ph=");
  Serial.println(phValue);

  // LCD display
  M5.Lcd.setTextColor(GREEN);
  M5.Lcd.setCursor(30,50);
  M5.Lcd.print(phValue);
  delay(5000);
  M5.Lcd.setTextColor(BLACK);
  M5.Lcd.setCursor(30,50);
  M5.Lcd.print(phValue);

  if(digitalRead(M5_BUTTON_HOME) == LOW){
      snprintf (msg, 6, "%2.1f", phValue);
      Serial.print("[MQTT] Sending pH: ");
      clientMqtt.publish(mqtt_pub_topic_ph, msg);
  }
}
