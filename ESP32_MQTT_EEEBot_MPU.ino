//****************************************//
//* ESP32 MQTT EEEBot Template           *//
//* Modified from Rui Santos'            *//
//* https://randomnerdtutorials.com      *//
//*                                      *//
//* ESP32 Code                           *//
//*                                      *//
//* UoN 2022 - ND                        *//
//****************************************//

#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>


// Add your required sensor/component libraries here
// --
#include <MPU6050_tockn.h>
// --

MPU6050 mpu6050(Wire);

// Replace the next variables with your SSID/Password combination
const char* ssid = "table13";                      //CHANGE ME
const char* password = "benchthirteen";              //CHANGE ME     

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";                      
const char* mqtt_server = "192.168.137.125";          //CHANGE ME

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

float angle;

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
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

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Add your subscribed topics here i.e. statements to control GPIOs with MQTT
  // --
  // --
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      
      // Add your subscribe topics here
      // --
      
      // --
         
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 100) {
    lastMsg = now;
    
    // Add your own code here i.e. sensor measurements, publish topics & subscribe topics for GPIO control
    // --
    angle = mpu6050.getAngleZ();
    mpu6050.update();

    char angleString[8];
    dtostrf(angle, 1, 2, angleString);
    Serial.print("Angle Z: ");
    Serial.println(angleString);
    client.publish("esp32/mpu6050", angleString);
    // --
  }
}