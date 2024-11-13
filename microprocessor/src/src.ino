/******************************************************************************************
 * mc.ino
 * Install library "Arduino_JSON" by Arduino
 * Sensordaten sammeln und per HTTP POST Request an Server schicken (-> load.php).
 * load.php schreibt die Werte dann in die Datenbank
 * Beachte: Passe den Pfad zur load.php in const char* serverURL auf deinen eigenen an.
 ******************************************************************************************/

#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "env_config.h"

unsigned long lastTime = 0;
unsigned long timerDelay = 15000; // alle 15s wird ein neuer Wert verschickt

const char* serverURL = "http://www.waschbaeraugen.ch/php/load.php"; 

//Werte um zu erkennen, an welchem Pin der Gassensor angeschlossen ist
const gpio_num_t gpioPins[] = {GPIO_NUM_1, GPIO_NUM_0, GPIO_NUM_6, GPIO_NUM_5, GPIO_NUM_4};
gpio_num_t usedPin = GPIO_NUM_NC;

// Werte um den Gassensor auszulesen
int sensorValue;
int sensorPin;
String room;

gpio_num_t checkWhichDataPin() {
    for (gpio_num_t pin : gpioPins) {
      gpio_reset_pin(pin);
      gpio_set_direction(pin, GPIO_MODE_INPUT);
      int level = gpio_get_level(pin);
      if (level == 0){
        return pin;
      }
    }
    return GPIO_NUM_NC;
}

void setSensorPinAndRoom(){
  while (usedPin == GPIO_NUM_NC){
    usedPin = checkWhichDataPin();
  }
  switch (usedPin) {
    case GPIO_NUM_1:
      room = "4184bd33-91fb-11ef-be33-c4cbe1d62274"; // für I-0.31
      sensorPin = 1;
      Serial.println(" Daten für I-0.31 werden gesammelt...");
      break;
    case GPIO_NUM_0:
      room = "4184bd0a-91fb-11ef-be33-c4cbe1d62274"; // für I-0.30
      sensorPin = 0;
      Serial.println(" Daten für I-0.30 werden gesammelt...");
      break;
    case GPIO_NUM_6:
      room = "4184bc9a-91fb-11ef-be33-c4cbe1d62274"; // für Foyer
      sensorPin = 6;
      Serial.println(" Daten für Foyer werden gesammelt...");
      break;
    case GPIO_NUM_5:
      room = "4184bb7c-91fb-11ef-be33-c4cbe1d62274"; // für Eduzone
      sensorPin = 5;
      Serial.println(" Daten für Eduzone werden gesammelt...");
      break;
    case GPIO_NUM_4:
      room = "4184bcdc-91fb-11ef-be33-c4cbe1d62274"; // für I-0.11
      sensorPin = 4;
      Serial.println(" Daten für I-0.11 werden gesammelt...");
      break;
  }
}

void setup() {
  Serial.begin(115200);

  ////////////////////////////////////////////////////////////// Verbindung mit Wi-Fi herstellen
  WiFi.begin(NETWORK_SSID, NETWORK_PASS);
  Serial.printf("Connecting to %s with password %s\n", NETWORK_SSID, NETWORK_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("\nWiFi connected: SSID: %s, IP Address: %s\n", NETWORK_SSID, WiFi.localIP().toString().c_str());

  ////////////////////////////////////////////////////////////// SensorPin und Raum-ID festlegen
  setSensorPinAndRoom();
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {   // alle 15 Sekunden...
    lastTime = millis();

    ////////////////////////////////////////////////////////////// sensor auslesen

    sensorValue = analogRead(sensorPin);       //   read analog input pin 0
    Serial.print("Air Quality = ");
    Serial.print(sensorValue,   DEC);               // prints the value read
    Serial.println(" PPM");                                 //   wait 100ms for next reading

    ////////////////////////////////////////////////////////////// JSON zusammenbauen

    JSONVar dataObject;
    dataObject["room"] = room;
    dataObject["air_quality"] = sensorValue;
    dataObject["api_key"] = API_KEY;
    String jsonString = JSON.stringify(dataObject);

    ////////////////////////////////////////////////////////////// JSON string per HTTP POST request an den Server schicken (server2db.php)

    if (WiFi.status() == WL_CONNECTED) {   // Überprüfen, ob Wi-Fi verbunden ist
      // HTTP Verbindung starten und POST-Anfrage senden
      HTTPClient http;
      http.begin(serverURL);
      http.addHeader("Content-Type", "application/json");
      int httpResponseCode = http.POST(jsonString);

      // Prüfen der Antwort
      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.printf("HTTP Response code: %d\n", httpResponseCode);
        Serial.println("Response: " + response);
      } else {
        Serial.printf("Error on sending POST: %d\n", httpResponseCode);
      }

      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }
  }
}