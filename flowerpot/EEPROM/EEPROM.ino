#include <EEPROM.h>

// Your credentials here
const char BLYNK_AUTH[] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
const char WIFI_SSID[]  = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
const char WIFI_PASS[]  = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

// Encryption key (change this to your own random value)
const byte encryptionKey = 0xA7;  // Example key

byte encryptByte(byte data) {
  return data ^ encryptionKey;
}

byte decryptByte(byte data) {
  return data ^ encryptionKey;
}

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);
  
  Serial.println("Starting EEPROM write with encryption...");
  
  int addr = 0;
  
  // Write encrypted Blynk auth token
  for(int i = 0; i < strlen(BLYNK_AUTH); i++) {
    EEPROM.write(addr++, encryptByte(BLYNK_AUTH[i]));
  }
  EEPROM.write(addr++, encryptByte('\0'));
  
  while(addr < 33) {
    EEPROM.write(addr++, encryptByte('\0'));
  }
  
  // Write encrypted WiFi SSID
  for(int i = 0; i < strlen(WIFI_SSID); i++) {
    EEPROM.write(addr++, encryptByte(WIFI_SSID[i]));
  }
  EEPROM.write(addr++, encryptByte('\0'));
  
  while(addr < 65) {
    EEPROM.write(addr++, encryptByte('\0'));
  }
  
  // Write encrypted WiFi password
  for(int i = 0; i < strlen(WIFI_PASS); i++) {
    EEPROM.write(addr++, encryptByte(WIFI_PASS[i]));
  }
  EEPROM.write(addr++, encryptByte('\0'));
  
  while(addr < 129) {
    EEPROM.write(addr++, encryptByte('\0'));
  }
  
  if(EEPROM.commit()) {
    Serial.println("Encrypted credentials written successfully!");
  } else {
    Serial.println("ERROR! EEPROM commit failed");
  }
  
  Serial.println("\nVerifying stored encrypted credentials:");
  verifyStoredCredentials();
}

void verifyStoredCredentials() {
  char storedAuth[33];
  char storedSSID[33];
  char storedPass[65];
  
  int addr = 0;
  
  // Read and decrypt auth token
  for(int i = 0; i < 32; i++) {
    storedAuth[i] = decryptByte(EEPROM.read(addr++));
  }
  storedAuth[32] = '\0';
  
  // Read and decrypt SSID
  for(int i = 0; i < 32; i++) {
    storedSSID[i] = decryptByte(EEPROM.read(addr++));
  }
  storedSSID[32] = '\0';
  
  // Read and decrypt password
  for(int i = 0; i < 64; i++) {
    storedPass[i] = decryptByte(EEPROM.read(addr++));
  }
  storedPass[64] = '\0';
  
  Serial.println("Stored Auth Token: " + String(storedAuth));
  Serial.println("Stored SSID: " + String(storedSSID));
  Serial.println("Stored Password: " + String(storedPass));
}

void loop(){}