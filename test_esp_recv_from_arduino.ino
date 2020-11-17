#define D2 4
#define D3 0

#include "ThingSpeak.h"
#include "secrets.h"

#include <ESP8266WiFi.h>

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int keyIndex = 0;
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

#include <SoftwareSerial.h>
SoftwareSerial NodeSerial(D2, D3);


void setup() {
  ThingSpeak.begin(client);     // Initialize ThingSpeak
  pinMode(D2, INPUT);
  pinMode(D3, OUTPUT);
  Serial.begin(115200);
  NodeSerial.begin(57600);
  Serial.println();
  Serial.println("NodeMCU/ESP8266 Run");
}

void loop() {
  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);  
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }

  while (NodeSerial.available() > 0)
  {
    int distance_data = NodeSerial.parseInt();

    if (NodeSerial.read() == '\n')
    {
      Serial.println("===========================ESP8266===========================");
      Serial.print("Distance");
      Serial.print(": ");
      Serial.print(distance_data);
      Serial.print(" cm\n");
      // Write value to Field 1 of a ThingSpeak Channel
      int httpCode = ThingSpeak.writeField(myChannelNumber, 1, distance_data, myWriteAPIKey);

      if (httpCode == 200) {
        Serial.println("Channel write successful.");
      }
      else {
        Serial.println("Problem writing to channel. HTTP error code " + String(httpCode));
      }
    }
  }
}
