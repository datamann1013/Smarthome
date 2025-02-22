# 📡 Radar And PIR Motion Sensor Test

A simple test to verify the functionality of a radar motion sensor using an LED indicator.

## Table of Contents
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Pin Connections](#pin-connections)
- [Installation Steps](#installation-steps)
- [Expected Behavior](#expected-behavior)
- [Debugging](#debugging)

## Features
✅ Reads motion data from a radar sensor.  
✅ Activates an LED when motion is detected.  
✅ Displays sensor readings in the Serial Monitor for debugging.

## Hardware Requirements
- Arduino board (ESP8266, ESP32, or equivalent)
- Radar motion sensor
- LED
- Resistor (if needed for LED)
- Jumper wires

## Pin Connections

| Component          | Pin                             |  
|--------------------|---------------------------------|  
| Radar/PIR Sensor   | D1                              |  
| LED                | D2                              |  
| VCC (Sensor, LED)  | 5V (Possible with 3.3V for PIR) |  
| GND                | GND                             |  

## Installation Steps
1. Connect components following the pin configuration.
2. Open the [basicradar.ino](basicradar.ino) file in the Arduino IDE.
3. Upload the code to the board.
4. Open the Serial Monitor (9600 baud) to view motion sensor readings.

## Expected Behavior
- When motion is detected, the LED turns **on** and "LED on!" is printed to the Serial Monitor.
- When no motion is detected, the LED turns **off** and "LED off!" is printed to the Serial Monitor.

## Debugging
- If the sensor does not detect motion, check wiring and ensure it receives power.
- If the LED does not turn on, verify the correct pin connections.
- Monitor Serial Monitor output to confirm sensor readings.  
