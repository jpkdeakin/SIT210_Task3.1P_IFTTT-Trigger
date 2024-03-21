#include <WiFiNINA.h>
#include "secret.h" 

#define SUNLEVEL 200   // Light level we consider "sunlight"

WiFiClient client;
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS; 

// IFTTT Webhook Trigger
char   HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME   = "/trigger/**Your EVENT Here**/with/key/**Your KEY Here**";


// Sunlight variables
int sensorPin = A0;
int sensorValue;
bool sunLight;

void setup() {
  WiFi.begin(ssid, pass);
  Serial.begin(9600);
}

void loop() {

  sensorValue = analogRead(sensorPin);
  
  if(sunLight == false && sensorValue > SUNLEVEL) {
    sunLight = true;
    SendWebhook();
  } else if(sunLight == true && sensorValue < SUNLEVEL){
    sunLight = false;
    SendWebhook();
  }

  delay(1000);
}


void SendWebhook() {
  String queryString = "?value1=" + String(sunLight) + "&value2=" + String(sensorValue);
  while (!Serial);
  if (client.connect(HOST_NAME, 80)) {
    // if connected
    Serial.println("Connected to IFTTT server");
  }
  else {
    // if not connected:
    Serial.println("connection failed");
  }

  client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
  client.println("Host: " + String(HOST_NAME));
  client.println("Connection: close");
  client.println(); // end HTTP header

  while (client.connected()) {
    if (client.available()) {
      // read an incoming byte from the server and print it to serial monitor:
      char c = client.read();
      Serial.print(c);
    }
  }

  // the server's disconnected, stop the client:
  client.stop();
  Serial.println();
  Serial.println("disconnected");
}
