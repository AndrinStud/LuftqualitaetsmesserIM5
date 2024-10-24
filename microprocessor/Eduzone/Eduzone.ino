/******************************************************************************************
 * mc.ino
 * Install library "Arduino_JSON" by Arduino
 * Sensordaten sammeln und per HTTP POST Request an Server schicken (-> load.php).
 * load.php schreibt die Werte dann in die Datenbank
 * Beachte: Passe den Pfad zur load.php in const char* serverURL auf deinen eigenen an.
 ******************************************************************************************/

#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h> // by Arduino

unsigned long lastTime = 0;
unsigned long timerDelay = 15000; // alle 15s wird ein neuer Wert verschickt

const char* ssid     = "Igloo";
const char* pass     = "1glooVision";

// const char* serverURL = "https://192.168.0.98/05_mc2db/server2db.php"; // Server-Adresse: hier kann http oder https stehen, aber nicht ohne
const char* serverURL = "http://www.waschbaeraugen.ch/php/load.php"; 

// Werte um den Gassensor auszulesen
int sensorValue;
int sensorpin;
String room;

void setup() {
  Serial.begin(115200);

  ////////////////////////////////////////////////////////////// Verbindung mit Wi-Fi herstellen
  WiFi.begin(ssid, pass);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("\nWiFi connected: SSID: %s, IP Address: %s\n", ssid, WiFi.localIP().toString().c_str());

  ////////////////////////////////////////////////////////////// Sensorpin und Raum-ID festlegen
  while (sensorpin == NULL){
    sensorpin = setSensorpinAndRoom();
  }
}

int setSensorpinAndRoom(){
  if (analogRead(1) != 0){
    room = "4184bd33-91fb-11ef-be33-c4cbe1d62274"; // für I-0.31
    Serial.print(analogRead(1));
    Serial.println(" Daten für I-0.31 werden gesammelt...");
    return sensorpin = 1;
  }
  else if (analogRead(0) != 0){
    room = "4184bd0a-91fb-11ef-be33-c4cbe1d62274"; // für I-0.30  
    Serial.print(analogRead(0));
    Serial.println(" Daten für I-0.30 werden gesammelt...");
    return sensorpin = 0;
  }
  else if (analogRead(6) != 0){
    room = "4184bc9a-91fb-11ef-be33-c4cbe1d62274"; // für Foyer
    Serial.print(analogRead(6));
    Serial.println(" Daten für Foyer werden gesammelt...");
    return sensorpin = 6;
  }
  else if (analogRead(5) != 0){
    room = "4184bb7c-91fb-11ef-be33-c4cbe1d62274"; // für Eduzone
    Serial.print(analogRead(5));
    Serial.println(" Daten für Eduzone werden gesammelt...");
    return sensorpin = 5;
  }
  else if (analogRead(4) != 0){
    room = "4184bcdc-91fb-11ef-be33-c4cbe1d62274"; // für I-0.11
    Serial.print(analogRead(4));
    Serial.println(" Daten für I-0.11 werden gesammelt...");
    return sensorpin = 4;
  }
  else {
    Serial.println("Sensor wurde falsch angeschlossen oder kann nicht erkannt werden... Versuche es in 5s erneut");
    return NULL;
  }
}

void loop() {
  
  if ((millis() - lastTime) > timerDelay) {   // alle 15 Sekunden...
    lastTime = millis();


    ////////////////////////////////////////////////////////////// sensor auslesen

    sensorValue = analogRead(sensorpin);       //   read analog input pin 0
    Serial.print("Air Quality = ");
    Serial.print(sensorValue,   DEC);               // prints the value read
    Serial.println(" PPM");                                 //   wait 100ms for next reading

    
    ////////////////////////////////////////////////////////////// JSON zusammenbauen

    JSONVar dataObject;
    dataObject["room"] = room;
    dataObject["air_quality"] = sensorValue;
    String jsonString = JSON.stringify(dataObject);
    // String jsonString = "{\"sensor\":\"fiessling\", \"wert\":77}";  // stattdessen könnte man den JSON string auch so zusammenbauen

  
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
