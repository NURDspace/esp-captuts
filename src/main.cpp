#include <Arduino.h>

const int led_pin = 5;
const int sensor_pin = 4;

const long interval = 200;
unsigned long current_time = millis();
unsigned long last_trigger = 0;
boolean pressednow = false;

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
}

void loop() {

  current_time = millis();
  if(pressednow && (current_time - last_trigger > interval)) {
    Serial.println("Button was unpressed");
    digitalWrite(led_pin, LOW);
    pressednow = false;
  }
}