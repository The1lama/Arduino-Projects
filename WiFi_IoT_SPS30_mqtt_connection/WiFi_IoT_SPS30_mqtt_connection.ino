
#include "arduino_secrets.h"
#include <sps30.h>
#include <SPI.h>
#include <WiFiUdp.h>
#include <ArduinoMqttClient.h>
#include <ArduinoUniqueID.h>
#include <WiFiNINA.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <utility/wifi_drv.h>

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

const char    broker[] = "13.79.52.149";//"35.228.19.45"; "40.112.67.9";
int           port     = 1883;
String        topic_1  = "", topic_2  = "", topic_3  = "", topic_4  = "";
const long    interval = 10000;
unsigned long previousMillis = 0;
unsigned long epoch;
int           count = 0;
int16_t       ret;
uint8_t       auto_clean_days = 1;
uint32_t      auto_clean;
int           A1Value = 0;
int           A2Value = 0;
const int     analogInPin1 = A1;
const int     analogInPin2 = A2;
String        serienummer = "", topic = "3nine/sen/";
String        serienr = "    ";
int           status = WL_IDLE_STATUS; 
int           numberOfTries = 0, maxTries = 15;
double        value = 0;
struct        sps30_measurement m;
char          serial[SPS30_MAX_SERIAL_LEN];
uint16_t      data_ready;
float         mc_1p0_pre, mc_2p5_pre, mc_10p0_pre;

WiFiClient  wifiClient;
MqttClient  mqttClient(wifiClient);
WiFiUDP     ntpUDP;
//NTPClient timeClient(ntpUDP, "192.168.1.1:123", 0, 60000);
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 0, 60000);

void setup() {

WiFiDrv::pinMode(25, OUTPUT); //green pin
WiFiDrv::pinMode(26, OUTPUT); //red pin
WiFiDrv::pinMode(27, OUTPUT); //blue pin

Serial.begin(9600);
delay(2000);

get_serienr();
topic_1 = topic + serienummer + "/PM1/mg_m3";
topic_2 = topic + serienummer + "/PM2.5/mg_m3";
topic_3 = topic + serienummer + "/PM10/mg_m3";

Serial.print("  topic :");
Serial.println(topic_1);
Serial.println(" ");

WIFI_connect();
get_time();
mqtt_connect();
sps30_init();

}

int sps30_init() {
  sensirion_i2c_init();
  while (sps30_probe() != 0) {
    Serial.print("SPS sensor probing failed\n");
    delay(500);
  }
ret = sps30_set_fan_auto_cleaning_interval_days(auto_clean_days);
if (ret) {
  Serial.print("error setting the auto-clean interval: ");
  Serial.println(ret);
  }
ret = sps30_start_measurement();
if (ret < 0) {
    Serial.print("error starting measurement\n");
  }
}

int RGB_control(int bl_ant,int bl_freq,int red,int green,int blue){

int i;
for (i=0; i < bl_ant; i++){
  WiFiDrv::analogWrite(25, red);
  WiFiDrv::analogWrite(26, green);
  WiFiDrv::analogWrite(27, blue);
  delay(bl_freq);
if (bl_ant > 1) {
  WiFiDrv::analogWrite(25, 0);//blue
  WiFiDrv::analogWrite(26, 0);//green
  WiFiDrv::analogWrite(27, 0);//red
  delay(bl_freq);
  }
 }
}

int WIFI_connect() {
  Serial.println(WiFi.status());
  RGB_control(1,100,255,255,0); //yellow 
  Serial.print("Attempting to connect to WPA SSID: ");
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    WiFi.disconnect();
    delay(3000);
    Serial.print(".");
    RGB_control(5,200,255,0,0);
    RGB_control(1,100,255,255,0); //yellow 
    delay(10000);
    }
Serial.println(" You're connected to the network");
RGB_control(5,200,0,255,0);
}

int get_time() {
  RGB_control(1,100,0,0,255);
  timeClient.begin();
  timeClient.update();
  do {
    epoch = timeClient.getEpochTime();
    Serial.println(epoch);
    numberOfTries++;
    delay(2000);
  }
while ((epoch < 1600000000) && (numberOfTries < maxTries));
if (numberOfTries == maxTries) {
  Serial.println("NTP unreachable!!");
  epoch = 1600000000;
  RGB_control(5,200,255,0,0);
  }
  else {
    delay(100);
    RGB_control(5,200,0,255,0);
    }
  }

int mqtt_connect() {
RGB_control(1,100,255,0,255);
mqttClient.setUsernamePassword("cams_1", "abc");
if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
    RGB_control(5,200,255,0,0);
    RGB_control(1,100,255,0,255);
    delay(1000);
  }
else {
   Serial.println("You're connected to the MQTT broker!");
   RGB_control(5,200,0,255,0);
  }
}

int check_connection() {
  if (WiFi.status() == WL_CONNECTED) {
    mqtt_connect();
    }
  else {
    WIFI_connect();
    mqtt_connect;
    }
}

int get_serienr() {
UniqueIDdump(Serial);
  for (size_t i = 0; i < UniqueIDsize; i++)
  {
    if (UniqueID[i] < 0x10)
    serienummer = serienummer + "0";
    serienr = String(UniqueID[i], HEX);
      serienummer = serienummer + serienr;
     }
    // topic_serienummer = topic_serienummer + serienummer;
    Serial.print(serienummer);  
  // serienr = serienummer;
  // serienummer = serienummer + "/sensor";
}


void loop() {
RGB_control(2,25,0,255,0);
mqttClient.poll();
if (mqttClient.connected() == 0)
    check_connection();

timeClient.update();
epoch = timeClient.getEpochTime();
 
unsigned long currentMillis = millis();  
if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;

do {
    ret = sps30_read_data_ready(&data_ready);
    if (ret < 0) {
      Serial.print("error reading data-ready flag: ");
      Serial.println(ret);
    }
    else if (!data_ready)
      Serial.print("data not ready, no new measurement available\n");
    else
    break;
    delay(100);
  }
  while (1);

ret = sps30_read_measurement(&m);
if (ret < 0) {
    Serial.print("error reading measurement\n");
  }
else
  {
    Serial.print("PM  1.0: ");
    Serial.println(m.mc_1p0);
    Serial.print("PM  2.5: ");
    Serial.println(m.mc_2p5);     
    Serial.print("PM  4.0: ");
    Serial.println(m.mc_4p0);
    Serial.print("PM 10.0: ");
    Serial.println(m.mc_10p0); 
   

    Serial.print("Typical partical size: ");
    Serial.println(m.typical_particle_size);

  if ( abs(mc_1p0_pre - m.mc_1p0 ) >= 000.1 ) {
    mqttClient.beginMessage(topic_1);
    mqttClient.println(m.mc_1p0/1000,3);
    mqttClient.endMessage();
    Serial.println(mc_1p0_pre - m.mc_1p0);
    mc_1p0_pre = m.mc_1p0;
  }

  if ( abs(mc_2p5_pre - m.mc_2p5 ) >= 000.1 ) {
    mqttClient.beginMessage(topic_2);
    mqttClient.println(m.mc_2p5/1000,3);
    mqttClient.endMessage();
    Serial.println(mc_2p5_pre - m.mc_2p5);
    mc_2p5_pre = m.mc_2p5;
  }

  if ( abs(mc_10p0_pre - m.mc_10p0 ) >= 000.1 ) {
    mqttClient.beginMessage(topic_3);
    mqttClient.println(m.mc_10p0/1000,3);
    mqttClient.endMessage();
     Serial.println(mc_10p0_pre - m.mc_10p0);
    mc_10p0_pre = m.mc_10p0;
  }

    delay(5000);
    count++;
  }
 }
}
