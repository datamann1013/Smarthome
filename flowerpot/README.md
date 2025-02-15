![GitHub Header](github-header-image.png)
### *A plant care system using ESP8266, Blynk IoT, and a soil moisture sensor. 🌱*

📄 [Back to main README](../README.MD)

![C++](https://img.shields.io/badge/C++-11-blue)
![License](https://img.shields.io/badge/license-MIT-green)
![Version](https://img.shields.io/badge/version-1.0.0-blue.svg)

## **Table of Contents**
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Pin Connections](#pin-connections)
- [Software Requirements](#software-requirements)
- [Secure Credential Storage](#secure-credential-storage)
- [Time Control System](#time-control-system)
- [Blynk App Configuration](#blynk-app-configuration)
- [Installation Steps](#installation-steps)
- [System Operation](#system-operation)
- [Debugging](#debugging)
- [Security Notes](#security-notes)
- [Maintenance](#maintenance)

## **Features**
✅ **Real-time soil moisture monitoring** with averaged readings for accuracy.  
✅ **Dual LED control:**
- Manual control via Blynk app
- Automatic alert LED for water level warnings  
  ✅ **Configurable watering threshold** via the Blynk app.  
  ✅ **Advanced time-based LED control:**
- Customizable ON/OFF schedule
- Supports both standard and overnight schedules
- Automatic synchronization with Blynk server time  
  ✅ **Secure credential storage** with encrypted EEPROM.  
  ✅ **Real-time clock integration** for precise scheduling.  
  ✅ **Automatic WiFi and Blynk reconnection handling.**  
  ✅ **Detailed serial debugging output.**  
  ✅ **Basic encryption for stored credentials.**

## **Hardware Requirements**
- **ESP8266 board** (NodeMCU, Wemos D1 Mini, etc.)
- **Soil moisture sensor**
- **2x LEDs:**
  - Manual control LED
  - Alert indicator LED
- **2x 220Ω resistors** (for LEDs)
- **Jumper wires**
- **USB cable** (for programming)

## **Pin Connections**

| Component               | ESP8266 Pin |
|-------------------------|------------|
| **Manual Control LED**  | D2 (via 220Ω resistor) |
| **Alert LED**           | D1 (via 220Ω resistor) |
| **Soil Moisture Sensor**| A0 |
| **VCC**                 | 3.3V |
| **GND**                 | GND |

![Wiring diagram](Wiring%20diagram.png)

## **Software Requirements**
- **Arduino IDE**
- **Required Libraries:**
  - `ESP8266WiFi`
  - `BlynkSimpleEsp8266`
  - `TimeLib`
  - `EEPROM`

## **Secure Credential Storage**
🔒 **Encryption System**
- Basic **XOR encryption** for credential security.
- **Custom encryption key** (user-defined).
- **Encrypted EEPROM storage** for:
  - Blynk authentication token (32 bytes).
  - WiFi SSID (32 bytes).
  - WiFi password (64 bytes).

📌 **Storage Layout**
- **Total EEPROM allocation:** 512 bytes
- **Data Structure:**
  - Bytes 0-32 → Encrypted Blynk token
  - Bytes 33-64 → Encrypted WiFi SSID
  - Bytes 65-128 → Encrypted WiFi password

## **Time Control System**
⏳ **Schedule Types**
- **Standard Schedule** (ON hour < OFF hour)
  - Example: ON at 08:00, OFF at 17:00
  - LED operates during the day.
- **Overnight Schedule** (ON hour > OFF hour)
  - Example: ON at 20:00, OFF at 06:00
  - LED remains ON overnight.

🔄 **Time Synchronization**
- **Daily sync** with Blynk server.
- **Minute-based schedule checking** for accuracy.
- **Automatic state updates** to Blynk app.
- **Failsafe reconnection** in case of connection loss.

## **Blynk App Configuration**
📱 **Virtual Pins Setup**

| Function                  | Virtual Pin |
|---------------------------|------------|
| Manual LED control        | V0 |
| Water threshold slider    | V1 (0-100%) |
| Alert status indicator    | V2 |
| LED ON time setting       | V3 |
| LED OFF time setting      | V4 |
| Water level display       | A0 |

## **Installation Steps**
1️⃣ **Install Required Libraries**
- Open **Arduino IDE** → Go to **Tools > Manage Libraries**
- Install:
  - `Blynk`
  - `ESP8266WiFi` (comes with ESP8266 board manager)

2️⃣ **Blynk Setup**
- Create a **new Blynk project**
- Note down:
  - `BLYNK_TEMPLATE_ID`
  - `BLYNK_TEMPLATE_NAME`
  - `BLYNK_AUTH_TOKEN`

3️⃣ **Configure Credentials**
- Open `EEPROM.ino`
- Update the following constants:
  ```cpp
  const char BLYNK_AUTH[] = "YOUR_BLYNK_TOKEN";
  const char WIFI_SSID[] = "YOUR_WIFI_SSID";
  const char WIFI_PASS[] = "YOUR_WIFI_PASSWORD";
  ```
- Set a **custom encryption key:**
  ```cpp
  const byte encryptionKey = 0xA7;  // Change this value
  ```
- Upload **EEPROM writer program** first.
- Verify **successful credential storage.**
- Upload the **main program.**

4️⃣ **Sensor Calibration**
- Default values:
  ```cpp
  #define SENSOR_DRY 10    // Completely dry
  #define SENSOR_WET 890   // Fully submerged
  ```
- Adjust these values based on your sensor readings.

## **System Operation**
⚙️ **How It Works:**
- **Soil moisture levels** are **monitored in real-time**.
- **Water level is displayed as a percentage** (0-100%).
- **Alert LED activates** when the **water level drops below the threshold**.
- **Manual LED** can be turned **on/off via Blynk app**.
- **Time-based LED control** follows the user’s **custom ON/OFF schedule**.

## **Debugging**
🔍 **Serial Debugging (115200 baud rate)**
- Monitors:
  - Raw sensor readings
  - Calculated water levels
  - Current threshold values
  - LED states
  - Connection status

## **Security Notes**
🔐 **Best Practices:**  
✔ Change the **default encryption key** before deployment.  
✔ Verify credentials **after writing to EEPROM**.  
✔ Keep **encryption key secure**.  
✔ Remember that **EEPROM has limited write cycles (~100,000).**

## **Maintenance**
🛠 **Routine Checks:**
- Regularly **calibrate the soil moisture sensor**.
- **Clean sensor probes** to maintain accuracy.
- Monitor **serial output** for any system errors.
- Keep **Blynk app and libraries up to date**.  
