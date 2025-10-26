// Standalone EEPROM writer sketch
// Edit the three placeholders below, then open this file as a sketch in the
// Arduino IDE (or move it to its own folder) and upload to the ESP8266 to
// write encrypted credentials into EEPROM.

#include <EEPROM.h>

// --- EDIT THESE BEFORE UPLOADING ---
// Put your real Blynk auth token, WiFi SSID and WiFi password here.
const char BLYNK_AUTH[] = "PUT_BLYNK_AUTH_HERE";
const char WIFI_SSID[]  = "PUT_YOUR_SSID_HERE";
const char WIFI_PASS[]  = "PUT_YOUR_WIFI_PASS_HERE";
// -----------------------------------

// Fixed sizes (must match main sketch)
const int EEPROM_SIZE = 512;
const int AUTH_ADDR = 0;
const int AUTH_SIZE = 33;
const int SSID_ADDR = AUTH_ADDR + AUTH_SIZE;
const int SSID_SIZE = 33;
const int PASS_ADDR = SSID_ADDR + SSID_SIZE;
const int PASS_SIZE = 65;

// Encryption key (change to your own random value if desired)
const byte encryptionKey = 0xA7;

byte encryptByte(byte data) {
  return data ^ encryptionKey;
}

byte decryptByte(byte data) {
  return data ^ encryptionKey;
}

void writeEncrypted(int startAddr, const char* input, int maxLen) {
  int len = min((size_t)strlen(input), (size_t)(maxLen - 1));
  for (int i = 0; i < len; i++) {
    EEPROM.write(startAddr + i, encryptByte((byte)input[i]));
  }
  EEPROM.write(startAddr + len, encryptByte('\0'));
  for (int i = len + 1; i < maxLen; i++) {
    EEPROM.write(startAddr + i, encryptByte('\0'));
  }
}

void readDecrypted(int startAddr, char* output, int maxLen) {
  for (int i = 0; i < maxLen - 1; i++) {
    output[i] = (char)decryptByte(EEPROM.read(startAddr + i));
  }
  output[maxLen - 1] = '\0';
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

void setup() {
  Serial.begin(115200);
  Serial.println("EEPROM Writer starting...");

  EEPROM.begin(EEPROM_SIZE);
  delay(50);

  Serial.println("Writing encrypted credentials to EEPROM...");
  writeEncrypted(AUTH_ADDR, BLYNK_AUTH, AUTH_SIZE);
  writeEncrypted(SSID_ADDR, WIFI_SSID, SSID_SIZE);
  writeEncrypted(PASS_ADDR, WIFI_PASS, PASS_SIZE);

  if (EEPROM.commit()) {
    Serial.println("Encrypted credentials written successfully!");
  } else {
    Serial.println("ERROR: EEPROM commit failed");
  }

  Serial.println("Verifying stored credentials:");
  verifyStoredCredentials();

  Serial.println("Done. You can now upload the main sketch.");
}

void loop() {
  // Nothing to do. This sketch writes once on boot and then idles.
  delay(1000);
}