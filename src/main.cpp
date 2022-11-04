#include <Arduino.h>
#include <ESP8266WiFi.h>

const int led_pin = 5;
const int sensor_pin = 4;

const long interval = 200;
unsigned long current_time = millis();
unsigned long last_trigger = 0;
boolean pressednow = false;

String ssid = "yourssid";
const char* password = "yourpass";
const char* host = "yourhost";
const uint16_t port = 6600;

void nextmpd() {
  Serial.print("connecting to ");
  Serial.print(host);
  Serial.print(':');
  Serial.println(port);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    delay(2000);
    return;
  }

  // This will send a string to the server
  Serial.println("sending data to server");
  if (client.connected()) {
    client.println("next");
    client.stop();
  }
}

IRAM_ATTR void button_detection() {
    Serial.println("Button was pressed");
    digitalWrite(led_pin, HIGH);
    pressednow = true;
    last_trigger = millis();
}

void setup() {

  Serial.begin(115200);

  pinMode(sensor_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(sensor_pin), button_detection, RISING);
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, LOW);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
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

void loop() {

  current_time = millis();
  if(pressednow && (current_time - last_trigger > interval)) {
    nextmpd();

    Serial.println("Button was unpressed");
    digitalWrite(led_pin, LOW);
    pressednow = false;
  }
}