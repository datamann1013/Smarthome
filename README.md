![github-header-image.png](github-header-image.png)

### A smart plant monitoring system using ESP8266, Blynk IoT platform, and soil moisture sensor.

# Features
- Real-time soil moisture monitoring
- LED indicator control through Blynk app
- Secure credential storage using EEPROM
- Automatic reconnection handling
- Basic encryption for stored credentials

# Hardware Requirements
- ESP8266 board (NodeMCU, Wemos D1 Mini, etc.)
- Soil moisture sensor
- LED
- 220Ω resistor for LED
- Jumper wires
- USB cable for programming

# Pin Connections
- LED → D1 (with 220Ω resistor)
- Soil Moisture Sensor → A0
- VCC → 3.3V
- GND → GND

# Software Requirements
- Arduino IDE
- Required Libraries:
  - ESP8266WiFi
  - BlynkSimpleEsp8266
  - EEPROM

# Installation Steps

## 1. Install Required Libraries
In Arduino IDE:
- Go to Tools → Manage Libraries
- Search and install:
  - Blynk
  - ESP8266WiFi (comes with ESP8266 board manager)

## 2. Configure Credentials
1. Open `EEPROM.ino`
2. Update the following constants with your values:
```cpp
const char BLYNK_AUTH[] = "YOUR_BLYNK_TOKEN";
const char WIFI_SSID[] = "YOUR_WIFI_SSID";
const char WIFI_PASS[] = "YOUR_WIFI_PASSWORD";
```