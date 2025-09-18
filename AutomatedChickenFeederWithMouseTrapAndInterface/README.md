# Automated Chicken Feeder with Mouse Trap and Interface

## Overview
A comprehensive automated poultry feeding system with integrated mouse protection, environmental monitoring, and scheduling capabilities. The system features a user-friendly LCD interface for manual control and automatic feeding based on real-time clock scheduling.

## Features
- **Automated Scheduling**: RTC-based feeding schedules (up to 10 different times)
- **Dual Servo Control**: Separate servos for food dispensing and mouse trap
- **Environmental Monitoring**: Temperature and humidity sensing with DHT11
- **Ultrasonic Detection**: Feed level monitoring and presence detection
- **Interactive Menu System**: LCD interface with button navigation
- **Manual Override**: Manual feeding and trap reset capabilities
- **Mouse Protection**: Integrated servo-controlled mouse trap system
- **Customizable Timing**: Adjustable dispense duration for different feed types

## Hardware Components
- **Arduino Uno/Nano** microcontroller
- **DS1302 Real-Time Clock** for scheduling
- **16x2 I2C LCD Display** for user interface
- **DHT11 Sensor** for temperature and humidity monitoring
- **Ultrasonic Sensor (HC-SR04)** for feed level detection
- **2x Servo Motors** for food dispensing and trap control
- **Relay Module** for high-power device control
- **Push Buttons** for menu navigation
- **Buzzer** for audio alerts

## Menu System
The system provides an interactive menu with the following options:

### 0. Set New Schedule
- Configure feeding frequency (1-10 schedules)
- Set specific times for each feeding session
- Store schedules in system memory

### 1. View Schedule
- Display all configured feeding times
- Show current schedule status
- Review active feeding sessions

### 2. Manual Feeding
- Immediate feeding activation
- Manual servo control
- Emergency feeding capability

### 3. Reset Trap
- Manual mouse trap reset
- Servo position reset
- Clear trap status

### 4. Dispense Duration
- Set feeding duration (water/food selection)
- Configure dispense time in milliseconds
- Customize for different feed types

## Pin Configuration
- **LCD Display**: I2C (0x27 address)
- **Ultrasonic Sensor**: Trigger - Pin 11, Echo - Pin 10
- **Navigation Buttons**: A0 (Select), A1 (Cancel), A2 (Up), A3 (Down)
- **Servos**: Pin 6 (Feeder), Pin 5 (Trap)
- **Relay**: Pin 2
- **Buzzer**: Pin 3
- **DHT11**: Pin 12
- **DS1302 RTC**: IO - Pin 8, CLK - Pin 7, RST - Pin 9

## Operational Features

### Automatic Feeding System
- **Schedule Checking**: Monitors time every 60 seconds
- **Feeding Duration**: Configurable dispense time
- **Status Tracking**: Prevents duplicate feedings
- **Servo Control**: Precise food dispensing mechanism

### Mouse Protection
- **Integrated Trap**: Servo-controlled mouse trap
- **Manual Reset**: Button-activated trap reset
- **Status Monitoring**: Trap position tracking
- **Safety Features**: Automatic trap positioning

### Environmental Monitoring
- **Temperature Sensing**: DHT11 temperature readings
- **Humidity Tracking**: Environmental humidity monitoring
- **Display Integration**: Real-time environmental data on LCD
- **Data Logging**: Historical environmental data

### User Interface
- **LCD Navigation**: 16x2 display with clear menus
- **Button Controls**: Four-button navigation system
- **Auto-timeout**: Returns to idle after 7 seconds of inactivity
- **Status Display**: Real-time system status and readings

## Setup Instructions
1. Install required libraries:
   - `LiquidCrystal_I2C`
   - `Servo`
   - `DHT`
   - `RtcDS1302`
   - `Wire`

2. Hardware assembly:
   - Connect all components according to pin configuration
   - Mount servos for food dispenser and mouse trap
   - Install sensors in appropriate positions
   - Set up power supply for relays and servos

3. Initial configuration:
   - Set current time in DS1302 RTC
   - Calibrate ultrasonic sensor for feed level
   - Test servo positions and range
   - Configure initial feeding schedule

4. System calibration:
   - Test manual feeding operation
   - Verify mouse trap mechanism
   - Check environmental sensor readings
   - Validate schedule timing accuracy

## Usage Instructions

### Setting Up Schedules
1. Navigate to "Set New Schedule" in main menu
2. Select number of feeding sessions (1-10)
3. Set hour and minute for each session
4. Confirm schedule settings
5. System will automatically feed at scheduled times

### Manual Operations
- **Manual Feeding**: Use menu option 2 for immediate feeding
- **Trap Reset**: Use menu option 3 to reset mouse trap
- **Duration Setting**: Use menu option 4 to adjust feeding duration

### Monitoring
- **Idle Display**: Shows current time, temperature, humidity
- **Feed Level**: Ultrasonic sensor monitors feed availability
- **Environmental Data**: Continuous temperature and humidity tracking

## Applications
- **Small-scale Poultry**: Backyard chicken feeding automation
- **Commercial Farms**: Scalable feeding system for larger operations
- **Pet Care**: Automated feeding for various small animals
- **Research**: Agricultural automation and monitoring studies
- **Educational**: Learning platform for automation and IoT

## Safety Features
- **Timeout Protection**: Automatic return to safe state
- **Manual Override**: Always available manual control
- **Environmental Monitoring**: Track conditions for animal health
- **Mouse Protection**: Integrated pest control system
- **Power Management**: Efficient power usage with relay control

## Maintenance
- **Feed Refilling**: Monitor ultrasonic sensor for feed level alerts
- **RTC Battery**: Replace DS1302 battery annually
- **Servo Calibration**: Regular calibration of servo positions
- **Sensor Cleaning**: Keep ultrasonic and DHT sensors clean
- **Schedule Updates**: Modify feeding times as needed seasonally

## Advanced Features
- **Multi-schedule Support**: Up to 10 different feeding times
- **Environmental Integration**: Feeding adjustments based on temperature
- **Pest Control**: Automated mouse trap with feeding system
- **Data Display**: Real-time environmental and system status
- **Power Efficiency**: Relay control for high-power components

## Future Enhancements
- **IoT Connectivity**: WiFi integration for remote monitoring
- **Mobile App**: Smartphone control and monitoring
- **Data Logging**: SD card storage for feeding and environmental data
- **Camera Integration**: Visual monitoring of feeding area
- **Solar Power**: Off-grid operation with solar panel integration
