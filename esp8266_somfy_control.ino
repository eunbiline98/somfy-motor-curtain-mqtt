#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiManager.h>
#include <Ticker.h>

const char *MQTT_SERVER = "xxx.xxx.xxx.xxx";
const char *MQTT_TOPIC_CURTAIN = "home/curtain/state";
const char *MQTT_STATUS_CURTAIN = "home/curtain/status";
const char *CURTAIN = "home/curtain/livingroom";

const char *MQTT_CLIENT_ID = "esp8266-somfy";
const char *MQTT_USERNAME = "";
const char *MQTT_PASSWORD = "";

char const* state_shutter;

WiFiClient espClient;
PubSubClient client(espClient);
Ticker ticker;

// Pin Setup
#define up D3
#define stopp D2
#define down D1

void tick()
{
  //toggle state
  digitalWrite(2, !digitalRead(2)); // set pin GPIO2 Led Wemos On-board
}

void configModeCallback(WiFiManager *myWiFiManager)
{
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
  //entered config mode, make led toggle faster
  ticker.attach(0.2, tick);
}

void setup_wifi()
{
  WiFi.mode(WIFI_STA);
  pinMode(2, OUTPUT);
  ticker.attach(0.6, tick);

  WiFiManager wm;
  wm.setAPCallback(configModeCallback);

  bool res;
  res = wm.autoConnect("somfy_esp_WiFi");
  Serial.println(res  ? "Success!" : "Failed!");
  wm.debugSoftAPConfig();

  if (!res)
  {
    Serial.println("failed to connect");
    wm.resetSettings();
    ESP.restart();
    delay(1000);
  }

  Serial.println("connected...");
//  ticker.detach();
}

void callback(char *topic, byte *message, unsigned int length)
{ Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  String messageTemp;

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)message[i]);
    Serial.println();
    if ((char)message[i] != '"')
      messageTemp += (char)message[i];
  }

  if (String(topic) == CURTAIN)
  {
    // up cutrain
    if (messageTemp == "u") {
      digitalWrite(up, HIGH);
      delay(30);
      digitalWrite(up, LOW);
      state_shutter = "open";
    }
    // stop curtain
    else if (messageTemp == "s") {
      digitalWrite(stopp, HIGH);
      delay(30);
      digitalWrite(stopp, LOW);
      state_shutter = "stop";
    }
    // down curtain
    else if (messageTemp == "d") {
      digitalWrite(down, HIGH);
      delay(30);
      digitalWrite(down, LOW);
      state_shutter = "closed";
    }
    client.publish(MQTT_TOPIC_CURTAIN, state_shutter, true);
  }
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD, MQTT_STATUS_CURTAIN, 1, 1, "Offline"))
    {
      Serial.println("connected");
      //subscribe
      client.subscribe(CURTAIN);
      client.publish(MQTT_STATUS_CURTAIN, "Online", true);
      digitalWrite(2, HIGH);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  // Serial port for debugging purposes
  Serial.begin(115200);
  setup_wifi();

  // MQTT Server Setup
  client.setServer(MQTT_SERVER, 1883);
  client.setCallback(callback);

  pinMode(2, OUTPUT); //WiFi Indicator
  pinMode(up, OUTPUT);
  pinMode(stopp, OUTPUT);
  pinMode(down, OUTPUT);

  digitalWrite(up, LOW);
  digitalWrite(stopp, LOW);
  digitalWrite(down, LOW);

}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}
