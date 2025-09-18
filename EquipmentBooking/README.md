# Equipment Booking System

## Overview
An IoT-based equipment booking and management system using ESP32, RFID technology, and Google Sheets integration. This system allows users to check out and return equipment (projectors, HDMI cables) using RFID cards with automatic tracking and authorization.

## Features
- **RFID-based Authentication**: Dual RFID readers for user identification and authorization
- **Smart Locker System**: Ultrasonic sensors detect equipment presence in lockers
- **Automated Locking**: Solenoid locks control access to equipment compartments
- **Real-time Tracking**: Equipment status monitoring with LED indicators
- **Google Sheets Integration**: Automatic logging of bookings and returns
- **LCD Display**: User-friendly interface showing booking status and instructions
- **WiFi Connectivity**: Remote monitoring and data synchronization

## Hardware Components
- ESP32 microcontroller
- 2x MFRC522 RFID readers
- 16x2 I2C LCD display
- 2x Ultrasonic sensors (HC-SR04)
- 2x Solenoid locks
- LED indicators and buzzer
- Push buttons for navigation

## Setup Instructions
1. Install required libraries:
   - `LiquidCrystal_I2C`
   - `MFRC522`
   - `WiFi`
   - `HTTPClient`

2. Update WiFi credentials in the code
3. Configure Google Apps Script URL for data logging
4. Wire components according to pin definitions
5. Upload code to ESP32

## Usage
1. Select equipment type using navigation buttons
2. Scan student ID card on RFID reader 1
3. Scan authorized staff card on RFID reader 2 for approval
4. Equipment compartment unlocks automatically
5. System tracks equipment removal/return via ultrasonic sensors

## Pin Configuration
- Buttons: GPIO 32, 34, 35
- RFID Readers: GPIO 4, 5 (SS pins)
- Solenoids: GPIO 33, 25
- Ultrasonic: GPIO 12-13, 14-27
- LEDs: GPIO 15, 16
- Buzzer: GPIO 17

## Applications
- University equipment management
- Library resource booking
- Tool lending systems
- Asset tracking and monitoring
