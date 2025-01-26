
#define BLYNK_TEMPLATE_ID "TMPL4GTYTeakI"
#define BLYNK_TEMPLATE_NAME "ESp8266 LED blink"
#define BLYNK_AUTH_TOKEN "lq_hZ-Myf6ZGvKzGNTxo7_tnT-l7ktA-"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <TimeLib.h>
#include <WidgetRTC.h>

// Credentials will be stored in EEPROM
char auth[33];  // Blynk auth token
char ssid[33];  // WiFi SSID
char pass[65];  // WiFi password

int turnOnHour = 0;    // Store the hour to turn on LED
int turnOffHour = 0;   // Store the hour to turn off LED
unsigned long lastTimeCheck = 0;  // Last time we checked the time

// Pin definitions
#define MANUAL_LED_PIN D2
#define ALERT_LED_PIN D1
#define HUMIDITY_PIN A0

// Sensor calibration values
#define SENSOR_DRY 10    // Value when completely dry
#define SENSOR_WET 890     // Value when water level is full

// Global variables
int waterThreshold = 50;
BlynkTimer timer;

BLYNK_WRITE(V3) {
  turnOnHour = param.asInt();
  Serial.print("LED Turn ON hour set to: ");
  Serial.println(turnOnHour);
}

BLYNK_WRITE(V4) {
  turnOffHour = param.asInt();
  Serial.print("LED Turn OFF hour set to: ");
  Serial.println(turnOffHour);
}

// Water level threshold control from Blynk app
BLYNK_WRITE(V1) {
  waterThreshold = param.asInt();
  Serial.print("New water threshold set to: ");
  Serial.println(waterThreshold);
}

// Manual LED control
BLYNK_WRITE(V0) {
  digitalWrite(MANUAL_LED_PIN, param.asInt());
  Serial.printf("Manual LED state changed to: %s\n", param.asInt() ? "ON" : "OFF");
}

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

float getAverageWaterLevel() {
  float sum = 0;
  const int samples = 10;
  
  // Take multiple readings
  for(int i = 0; i < samples; i++) {
    sum += analogRead(HUMIDITY_PIN);
    delay(10);
  }
  
  // Calculate average
  float averageReading = sum / samples;
  

  
  // Constrain the reading between wet and dry values
  averageReading = constrain(averageReading, SENSOR_DRY, SENSOR_WET);
  
  // Calculate percentage
  float percentage = map(averageReading, SENSOR_DRY, SENSOR_WET, 100, 0);
  
  // Ensure the percentage stays within 0-100 range
  percentage = constrain(percentage, 0, 100);
  
  return percentage;
}

void checkWaterLevelAndUpdateLED(float waterLevel) {
  if (waterLevel > waterThreshold) {
    digitalWrite(ALERT_LED_PIN, HIGH);
    Serial.println("ALERT: Water level below threshold!");
    Blynk.virtualWrite(V2, 1);
  } else {
    digitalWrite(ALERT_LED_PIN, LOW);
    Blynk.virtualWrite(V2, 0);
  }
}

void sendSensorData() {
  float waterLevel = getAverageWaterLevel();
  
  // Send water level to Blynk using A0
  Blynk.virtualWrite(A0, waterLevel);
  
  // Check water level and update alert LED
  checkWaterLevelAndUpdateLED(waterLevel);
  
  // Debug output
  Serial.print("Raw Reading: ");
  Serial.print(analogRead(HUMIDITY_PIN));
  Serial.print(" | Water Level: ");
  Serial.print(waterLevel);
  Serial.print("% | Threshold: ");
  Serial.print(waterThreshold);
  Serial.println("%");
}

void checkTimeAndUpdateLED() {
  // Check time every minute (60000 milliseconds)
  if (millis() - lastTimeCheck >= 60000) {
    lastTimeCheck = millis();
    
    // Get the current time from Blynk server
    int currentHour = hour();
    
    // Check if we should turn the LED on or off based on time
    if (turnOnHour < turnOffHour) {
      // Simple case: turn on if time is between on and off hours
      if (currentHour >= turnOnHour && currentHour < turnOffHour) {
        digitalWrite(MANUAL_LED_PIN, HIGH);
        Blynk.virtualWrite(V0, 1);
      } else {
        digitalWrite(MANUAL_LED_PIN, LOW);
        Blynk.virtualWrite(V0, 0);
      }
    } else if (turnOnHour > turnOffHour) {
      // Complex case: turn on if time is after on hour OR before off hour
      if (currentHour >= turnOnHour || currentHour < turnOffHour) {
        digitalWrite(MANUAL_LED_PIN, HIGH);
        Blynk.virtualWrite(V0, 1);
      } else {
        digitalWrite(MANUAL_LED_PIN, LOW);
        Blynk.virtualWrite(V0, 0);
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  // Initialize pins
  pinMode(MANUAL_LED_PIN, OUTPUT);
  pinMode(ALERT_LED_PIN, OUTPUT);
  
  // Initial LED states
  digitalWrite(MANUAL_LED_PIN, LOW);
  digitalWrite(ALERT_LED_PIN, LOW);
  
  // Connect to Blynk
  Serial.println("Connecting to Blynk...");
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  
  if (Blynk.connected()) {
    Serial.println("Connected to Blynk!");
  }
  
  // Setup timer for sensor readings (every 2 seconds)
  timer.setInterval(2000L, sendSensorData);

  // Sync time with Blynk server
  setSyncInterval(24 * 60 * 60); // Sync every day
}

void loop() {
  if (Blynk.connected()) {
    Blynk.run();
    timer.run();
    checkTimeAndUpdateLED();  // Add this line
  } else {
    Serial.println("Blynk connection lost! Reconnecting...");
    Blynk.connect();
    delay(1000);
  }
}