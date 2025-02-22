# 🔍 Component Testing

A collection of tests designed to verify the functionality of individual components before full system integration.

📄 [Back to main README](../README.MD)

## Table of Contents
- [Tested Components](#tested-components)
- [Purpose](#purpose)
- [Running Tests](#running-tests)
- [Debugging](#debugging)

## Tested Components

✅ **PIR Motion Sensors** – Detect movement and trigger responses.  
✅ **LED Control** – Ensure proper activation and deactivation of LEDs.  
✅ **Radar Sensor** – Test radar-based motion detection.

All these can be done with the [Basic Radar Test README](basicradar/README.md)

## Purpose
Component tests help diagnose issues before assembling the entire system. Each test isolates a specific component to verify:
- Correct wiring and functionality.
- Expected sensor responses and LED behaviors.
- Reliable serial output for debugging.

## Running Tests
1. Open the corresponding test script in the Arduino IDE.
2. Upload the code to the board.
3. Open the Serial Monitor (9600 baud) to view real-time output.
4. Observe expected behaviors (e.g., LED turning on when motion is detected).

## Debugging
- If a component does not respond, check wiring and connections.
- Use the Serial Monitor output to identify issues.
- Adjust sensitivity or delay values if needed.  
