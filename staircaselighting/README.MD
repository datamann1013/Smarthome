# PIR Sensor Motion Detection System

📄 [Back to main README](../README.MD)
## Description
This project is a simple motion detection system using two PIR (Passive Infrared) 
sensors and LEDs. When motion is detected by either sensor, the corresponding LED lights up, and a message is printed to the serial monitor.
This project will evolve into a staircase lighting solution.

## Table of Contents
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Pin Connections](#pin-connections)
- [Software Requirements](#software-requirements)
- [Installation Steps](#installation-steps)
- [System Operation](#system-operation)
- [Debugging](#debugging)
- [Future Improvements](#future-improvements)

## Features
- Uses two PIR sensors to detect motion.
- Two LED indicators to signal detected motion.
- Serial output messages for debugging.
- Simple and efficient motion tracking.

## Hardware Requirements
- 1x Arduino (Uno, Mega, etc.)
- 2x PIR sensors
- 2x LEDs
- 2x 220Ω resistors
- Jumper wires
- Breadboard

## Pin Connections
```
PIR Sensor 1 (Up) → Pin 2
PIR Sensor 2 (Down) → Pin 3
LED 1 (Up) → Pin 13 (with 220Ω resistor)
LED 2 (Down) → Pin 12 (with 220Ω resistor)
VCC (both sensors) → 5V
GND (both sensors) → GND
```

## Software Requirements
- Arduino IDE

## Installation Steps
1. Open the Arduino IDE.
2. Download the file staircaselighting.ino, and open it in Arduino IDE.
3. Connect your Arduino to the computer and upload the code.
4. Open the Serial Monitor (9600 baud) to see motion detection logs.

## System Operation
- When motion is detected by the upper PIR sensor, the LED at pin 13 lights up, and a message is printed.
- When motion is detected by the lower PIR sensor, the LED at pin 12 lights up, and a message is printed.
- The system continuously checks for motion every 100ms.

## Debugging
- Ensure PIR sensors are properly connected and powered.
- Open Serial Monitor to see sensor readings.
- Adjust PIR sensor sensitivity and delay settings if needed.

## Future Improvements
- Add a buzzer for audible alerts.
- Implement a time delay to prevent false triggers.
- Use wireless communication (e.g., ESP8266) to send motion alerts to a mobile app.

---
🚀 **Enjoy your motion detection system!**
