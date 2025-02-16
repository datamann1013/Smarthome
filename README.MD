# Smart House Project

## Overview

This project aims to develop a modular and expandable Smart House System using ESP8266 microcontrollers, various sensors, and IoT platforms like Blynk. The system will be capable of automating and monitoring multiple aspects of a home, such as lighting, security, and environmental conditions.

## Future Deployment

The final system will be deployed on either:

- An old Android tablet running a custom dashboard.
- A Raspberry Pi acting as a central control hub for processing and automation.

## Table of Contents

- [Features](#features)
- [Current Modules](#current-modules)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Installation](#installation)
- [Future Expansions](#future-expansions)

## Features

The Smart House system will include:

- ✅ **Automated Lighting** – Motion-based lighting control.
- ✅ **Soil Moisture Monitoring** – Smart plant watering system.
- ✅ **Remote Control & Monitoring** – Control components via a mobile app (Blynk).
- ✅ **Security System** – PIR motion sensors for detecting movement.
- ✅ **Time-based Automation** – Scheduled LED control and smart behaviors.
- ✅ **Secure Credential Storage** – Encrypted WiFi and Blynk credentials.

## Current Modules

This project consists of multiple subsystems that work together.

### 1️⃣ Smart Plant Monitoring System 🌱

- A system that monitors soil moisture levels and controls watering automatically.
- ESP8266-based
- Blynk integration for remote control
- Automatic & manual watering based on threshold values
- Time-controlled LED system

📄 More details:  [Smart Plant System README](flowerpot/README.md)


### 2️⃣ PIR-Based Staircase Lighting System 💡
- A motion-activated lighting system using PIR sensors for automatic illumination.
- Dual PIR sensors to detect movement direction (upstairs/downstairs).
- Sequential LED activation based on movement.
- Automatic turn-off after a predefined delay.
- Serial monitor logging for real-time debugging.

📄 More details: [Motion Sensor Lighting README](staircaselighting/README.MD)

## Hardware Requirements

The system is built using common microcontrollers and sensors:

- ESP8266 (NodeMCU/Wemos D1 Mini)
- Soil moisture sensor
- PIR motion sensors
- LEDs + Resistors
- Jumper wires & breadboards
- Raspberry Pi or Android Tablet (for final control hub)

## Software Requirements

- Arduino IDE (for programming microcontrollers)
- Blynk IoT Platform (for remote control and automation)
- ESP8266WiFi & Other Required Libraries

## Installation

1️⃣ Set up Arduino IDE and install necessary libraries.  
2️⃣ Flash firmware onto ESP8266 boards.  
3️⃣ Configure WiFi credentials (stored securely).  
4️⃣ Integrate with Blynk for remote access.  
5️⃣ Deploy and test the smart modules in the house.

## Future Expansions

- 🔜 **Central Dashboard**: Develop a web/mobile interface for full system control.
- 🔜 **Temperature & Humidity Monitoring**: Add DHT11 or DHT22 sensors.
- 🔜 **Security Enhancements**: Integrate door/window sensors.
- 🔜 **Voice Control**: Connect with Google Assistant or Alexa.
- 🔜 **Raspberry Pi as Smart Hub**: Run automation scripts on a local server.

This document will be updated as new modules are added. Stay tuned! 🚀
