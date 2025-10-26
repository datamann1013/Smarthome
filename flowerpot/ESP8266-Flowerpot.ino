
#define BLYNK_TEMPLATE_ID "TMPL4GTYTeakI"
#define BLYNK_TEMPLATE_NAME "ESp8266 LED blink"
#define BLYNK_AUTH_TOKEN "lq_hZ-Myf6ZGvKzGNTxo7_tnT-l7ktA-"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <EEPROM.h>
#include <TimeLib.h>
#include <WidgetRTC.h>

// Credentials will be stored in EEPROM
char auth[33];  // Blynk auth token
char ssid[33];  // WiFi SSID
char pass[65];  // WiFi password

int turnOnSecond = 15;    // Store the hour to turn on LED
int turnOffSecond = 45;   // Store the hour to turn off LED
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
  turnOnSecond = param.asInt();
  Serial.print("LED Turn ON hour set to: ");
  Serial.println(turnOnSecond);
}

BLYNK_WRITE(V4) {
  turnOffSecond = param.asInt();
  Serial.print("LED Turn OFF hour set to: ");
  Serial.println(turnOffSecond);
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
  
  // Use the SAME addresses as in your EEPROM writer
  const int AUTH_ADDR = 0;
  const int AUTH_SIZE = 33;
  const int SSID_ADDR = 33;
  const int SSID_SIZE = 33; 
  const int PASS_ADDR = 66;
  const int PASS_SIZE = 65;
  
  // Read auth from correct address (0-32)
  for(int i = 0; i < AUTH_SIZE; i++) {
    auth[i] = decryptByte(EEPROM.read(AUTH_ADDR + i));
  }
  auth[AUTH_SIZE-1] = '\0';  // Ensure null termination
  
  // Read SSID from correct address (33-65)
  for(int i = 0; i < SSID_SIZE; i++) {
    ssid[i] = decryptByte(EEPROM.read(SSID_ADDR + i));
  }
  ssid[SSID_SIZE-1] = '\0';
  
  // Read password from correct address (66-130)
  for(int i = 0; i < PASS_SIZE; i++) {
    pass[i] = decryptByte(EEPROM.read(PASS_ADDR + i));
  }
  pass[PASS_SIZE-1] = '\0';
  
  EEPROM.end();
}

byte decryptByte(byte data) {
  return data ^ 0xA7;  // Use the same encryption key as in the EEPROM writer
}

// Debug helper: dump raw EEPROM bytes and decrypted preview to Serial
void dumpEEPROMDebug(int startAddr = 0, int len = 128) {
  Serial.println("--- EEPROM RAW DUMP ---");
  for (int i = startAddr; i < startAddr + len; i++) {
    if ((i - startAddr) % 16 == 0) {
      Serial.printf("\n%04X: ", i);
    }
    byte b = EEPROM.read(i);
    Serial.printf("%02X ", b);
  }
  Serial.println();

  // Decrypted preview of auth/ssid/pass (sizes must match writer)
  Serial.println("\n--- EEPROM DECRYPTED PREVIEW ---");
  char tAuth[33];
  char tSsid[33];
  char tPass[65];
  int addr = 0;
  for (int i = 0; i < 32; i++) tAuth[i] = decryptByte(EEPROM.read(addr++));
  tAuth[32] = '\0';
  for (int i = 0; i < 32; i++) tSsid[i] = decryptByte(EEPROM.read(addr++));
  tSsid[32] = '\0';
  for (int i = 0; i < 64; i++) tPass[i] = decryptByte(EEPROM.read(addr++));
  tPass[64] = '\0';

  Serial.print("Auth (decrypted): "); Serial.println(tAuth);
  Serial.print("SSID (decrypted): "); Serial.println(tSsid);
  Serial.print("Pass (decrypted len): "); Serial.println(strlen(tPass));
  Serial.println("--- end EEPROM DUMP ---\n");
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

  
  Blynk.run();
  delay(1000);
  
  enterDeepSleep();
}

void checkTimeAndUpdateLED() {
  // Check time every 100ms instead of every minute
  if (millis() - lastTimeCheck >= 100) {
    lastTimeCheck = millis();
    
    // Get the current second from Blynk server
    int currentSecond = second();
    
    // Debug output
    Serial.print("Current second: ");
    Serial.print(currentSecond);
    Serial.print(" | On at: ");
    Serial.print(turnOnSecond);
    Serial.print(" | Off at: ");
    Serial.println(turnOffSecond);
    
    // Check if we should turn the LED on or off based on time
    if (turnOnSecond < turnOffSecond) {
      // Simple case: turn on if time is between on and off seconds
      if (currentSecond >= turnOnSecond && currentSecond < turnOffSecond) {
        digitalWrite(MANUAL_LED_PIN, HIGH);
        Blynk.virtualWrite(V0, 1);
      } else {
        digitalWrite(MANUAL_LED_PIN, LOW);
        Blynk.virtualWrite(V0, 0);
      }
    } else if (turnOnSecond > turnOffSecond) {
      // Complex case: turn on if time is after on second OR before off second
      if (currentSecond >= turnOnSecond || currentSecond < turnOffSecond) {
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
  // Quick EEPROM debug dump before loading credentials
  EEPROM.begin(512);
  dumpEEPROMDebug(0, 128);
  EEPROM.end();
  // Load stored credentials from EEPROM (if any)
  loadCredentials();
  // Debug: print loaded credentials (avoid printing password in production)
  Serial.print("Loaded SSID: ");
  Serial.println(ssid);
  Serial.print("Loaded Auth: ");
  Serial.println(auth);
  Serial.print("Loaded Pass length: ");
  Serial.println(strlen(pass));
  
  // Initialize pins
  pinMode(MANUAL_LED_PIN, OUTPUT);
  pinMode(ALERT_LED_PIN, OUTPUT);
  
  // Initial LED states
  digitalWrite(MANUAL_LED_PIN, LOW);
  digitalWrite(ALERT_LED_PIN, LOW);
  
  // Connect to WiFi first (debug-friendly) and then start Blynk so we can
  // see clear diagnostic output on Serial.
  Serial.println("Connecting to network...");

  if (strlen(ssid) == 0 || strlen(auth) == 0) {
    Serial.println("ERROR: SSID or Auth token empty. EEPROM may not be programmed.");
    Serial.println("If you haven't written credentials to EEPROM, either enable the EEPROM writer sketch or hardcode credentials temporarily for testing.");
  } else {
    Serial.print("Attempting WiFi connect to SSID: "); Serial.println(ssid);
    WiFi.begin(ssid, pass);

    // Wait for WiFi (timeout ~10s)
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
      Serial.print('.');
      delay(500);
      attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println();
      Serial.print("WiFi connected. IP: ");
      Serial.println(WiFi.localIP());
      Serial.print("RSSI: "); Serial.println(WiFi.RSSI());
    } else {
      Serial.println();
      Serial.println("WiFi connection failed (timeout)");
    }

    // Now start Blynk. This will print Blynk library messages as it attempts
    // to connect; leave it non-blocking for now.
    Serial.println("Starting Blynk...");
    Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
    // Give Blynk a short moment and check status
    delay(2000);
    if (Blynk.connected()) {
      Serial.println("Connected to Blynk!");
    } else {
      Serial.println("Blynk not connected yet; library will keep trying in loop().");
    }
  }
  
  // Setup timer for sensor readings (every 2 seconds)
  timer.setInterval(1800000L, sendSensorData);

  // Sync time with Blynk server
  setSyncInterval(24 * 60 * 60); // Sync every day
}

void enterDeepSleep() {
  Serial.println("Entering deep sleep for 30 minutes...");
  ESP.deepSleep(30 * 60 * 1000000); // 30 minutes in microseconds
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