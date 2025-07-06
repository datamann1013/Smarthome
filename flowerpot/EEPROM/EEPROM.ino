#include <EEPROM.h>

// Your credentials here
const char BLYNK_AUTH[] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
const char WIFI_SSID[]  = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
const char WIFI_PASS[]  = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

// Fixed sizes
const int EEPROM_SIZE = 512;
const int AUTH_ADDR = 0;
const int AUTH_SIZE = 33;
const int SSID_ADDR = AUTH_ADDR + AUTH_SIZE;
const int SSID_SIZE = 33;
const int PASS_ADDR = SSID_ADDR + SSID_SIZE;
const int PASS_SIZE = 65;

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
  EEPROM.begin(EEPROM_SIZE);
  
  Serial.println("Starting EEPROM write with encryption...");
  
  writeEncrypted(AUTH_ADDR, BLYNK_AUTH, AUTH_SIZE);
  writeEncrypted(SSID_ADDR, WIFI_SSID, SSID_SIZE);
  writeEncrypted(PASS_ADDR, WIFI_PASS, PASS_SIZE);

  if (EEPROM.commit()) {
    Serial.println("Encrypted credentials written successfully!");
  } else {
    Serial.println("ERROR! EEPROM commit failed");
  }

  Serial.println("\nVerifying stored encrypted credentials:");
  verifyStoredCredentials();
}

void writeEncrypted(int startAddr, const char* input, int maxLen) {
  int len = std::min<size_t>(strlen(input), static_cast<size_t>(maxLen - 1));
  for (int i = 0; i < len; i++) {
    EEPROM.write(startAddr + i, encryptByte(input[i]));
  }
  EEPROM.write(startAddr + len, encryptByte('\0'));
  for (int i = len + 1; i < maxLen; i++) {
    EEPROM.write(startAddr + i, encryptByte('\0'));
  }
}

void readDecrypted(int startAddr, char* output, int maxLen) {
  for (int i = 0; i < maxLen - 1; i++) {
    output[i] = decryptByte(EEPROM.read(startAddr + i));
  }
  output[maxLen - 1] = '\0';  // Ensure null-termination
}

void verifyStoredCredentials() {
  char storedAuth[AUTH_SIZE];
  char storedSSID[SSID_SIZE];
  char storedPass[PASS_SIZE];

  readDecrypted(AUTH_ADDR, storedAuth, AUTH_SIZE);
  readDecrypted(SSID_ADDR, storedSSID, SSID_SIZE);
  readDecrypted(PASS_ADDR, storedPass, PASS_SIZE);

  Serial.println("Stored Auth Token: " + String(storedAuth));
  Serial.println("Stored SSID: " + String(storedSSID));
  Serial.println("Stored Password: " + String(storedPass));
}

void loop() {}