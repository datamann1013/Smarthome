#define BLYNK_TEMPLATE_ID "TMPL4GTYTeakI"
#define BLYNK_TEMPLATE_NAME "ESp8266 LED blink"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <EEPROM.h>

// Pin definitions
#define LED_PIN D1
#define HUMIDITY_PIN A0

// Credentials will be stored in EEPROM
char auth[33];  // Blynk auth token
char ssid[33];  // WiFi SSID
char pass[65];  // WiFi password

BlynkTimer timer;

void loadCredentials() {
  EEPROM.begin(512);
  int addr = 0;
  
  // Read and decrypt credentials from EEPROM
  for(int i = 0; i < 32; i++) {
    auth[i] = decryptByte(EEPROM.read(addr++));
  }
  auth[32] = '\0';
  
  for(int i = 0; i < 32; i++) {
    ssid[i] = decryptByte(EEPROM.read(addr++));
  }
  ssid[32] = '\0';
  
  for(int i = 0; i < 64; i++) {
    pass[i] = decryptByte(EEPROM.read(addr++));
  }
  pass[64] = '\0';
  
  EEPROM.end();
}

byte decryptByte(byte data) {
  return data ^ 0xA7;  // Use the same encryption key as in the EEPROM writer
}

void sendSensorData() {
  int rawValue = analogRead(HUMIDITY_PIN);
  
  // Add some bounds checking
  if(rawValue < 0) rawValue = 0;
  if(rawValue > 1023) rawValue = 1023;
  
  float humidityPercentage = map(rawValue, 0, 1023, 0, 100);
  
  Blynk.virtualWrite(V1, humidityPercentage);
  
  Serial.print("Humidity: ");
  Serial.print(humidityPercentage);
  Serial.println("%");
}

BLYNK_WRITE(V0) {
  int ledState = param.asInt();
  if(ledState == 0 || ledState == 1) {  // Input validation
    digitalWrite(LED_PIN, ledState);
    Serial.printf("LED state changed to: %s\n", ledState ? "ON" : "OFF");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  
  // Load credentials from EEPROM
  loadCredentials();
  
  // Connect to Blynk with timeout
  Serial.println("Connecting to Blynk...");
  unsigned long startAttempt = millis();
  
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  
  while (!Blynk.connected() && millis() - startAttempt < 30000) {
    delay(100);
  }
  
  if (Blynk.connected()) {
    Serial.println("Connected to Blynk!");
  } else {
    Serial.println("Failed to connect to Blynk");
  }
  
  timer.setInterval(2000L, sendSensorData);
}

void loop() {
  static unsigned long lastReconnectAttempt = 0;
  
  if (Blynk.connected()) {
    Blynk.run();
    timer.run();
  } else {
    // Try to reconnect every 5 seconds
    if (millis() - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = millis();
      Serial.println("Blynk connection lost! Reconnecting...");
      Blynk.connect();
    }
  }
}