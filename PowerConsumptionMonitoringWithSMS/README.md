# Power Consumption Monitoring with SMS Alerts

## Overview
An intelligent energy management system that monitors electrical consumption in real-time and automatically controls power supply when consumption limits are exceeded. Features SMS notifications, automatic power cutoff, and comprehensive monitoring through Blynk mobile app integration.

## Features
- **Real-time Monitoring**: Continuous current and energy consumption tracking
- **Smart Automation**: Automatic power cutoff when kWh limit exceeded
- **SMS Notifications**: Multi-recipient SMS alerts for energy threshold breaches
- **Mobile Control**: Blynk app integration for remote monitoring and control
- **LCD Display**: Local display of current consumption and energy totals
- **Servo Control**: Automated power switch control via servo motor
- **Buzzer Alerts**: Audio warnings for energy threshold violations
- **Remote Reset**: System reset capability via mobile app

## System Components
- **ESP32** microcontroller for processing and connectivity
- **EnergyMonitor Library** for accurate current measurement
- **SIM800L GSM Module** for SMS functionality
- **16x2 I2C LCD** for local data display
- **Current Transformer (CT)** for non-invasive current sensing
- **Servo Motor** for automated power switch control
- **Buzzer** for audio alerts

## Technical Specifications
- **Voltage**: 220V AC monitoring
- **Current Range**: Configurable with CT sensor
- **Measurement Accuracy**: ±2% typical
- **Update Interval**: Real-time with change detection
- **Communication**: WiFi + GSM connectivity
- **Power Control**: Servo-actuated switch (180° rotation)
- **Energy Calculation**: Wh/kWh with cumulative tracking

## Monitoring Capabilities
- **Current (Amperes)**: Real-time electrical current measurement
- **Power (Watts)**: Instantaneous power consumption
- **Energy (kWh)**: Cumulative energy consumption tracking  
- **Cost Estimation**: Bill calculation based on configurable rates
- **Threshold Monitoring**: Automatic limit checking and alerts

## Blynk App Integration
- **V0**: Current reading display
- **V2**: Energy consumption (kWh) display
- **V3**: Estimated bill amount
- **V4**: Rate per kWh configuration
- **V5**: kWh limit setting
- **V6**: System reset button

## SMS Alert System
- **Multi-recipient**: Supports multiple phone numbers
- **Automatic Triggers**: Threshold-based alert system
- **15-minute Warning**: Advance notice before power cutoff
- **Status Updates**: Real-time consumption alerts
- **Emergency Contacts**: Configurable recipient list

## Safety Features
- **15-minute Delay**: Grace period before automatic shutoff
- **Manual Override**: Remote reset capability
- **Visual Indicators**: LED and LCD status displays
- **Audio Alerts**: Buzzer warnings during threshold violations
- **Startup Delay**: 3-second stabilization period on boot

## Setup Instructions
1. Install required libraries:
   - `WiFi`, `BlynkSimpleEsp32`
   - `EmonLib`, `LiquidCrystal_I2C`
   - `HardwareSerial`, `ESP32Servo`

2. Configure hardware connections:
   - Current transformer on GPIO 34
   - SIM800L on pins 16/17
   - Servo on GPIO 33
   - LCD on I2C bus (0x27)

3. Update configuration:
   - WiFi credentials
   - Blynk authentication token
   - SMS recipient numbers
   - Energy rate per kWh

4. Calibrate current sensor for accurate readings

## Pin Configuration
- **Current Sensor**: GPIO 34 (Analog input)
- **SIM800L**: TX-17, RX-16 (UART1)
- **Servo Motor**: GPIO 33 (PWM)
- **Buzzer**: GPIO 27 (Digital output)
- **LCD**: I2C (SDA-21, SCL-22)

## Applications
- **Home Energy Management**: Residential power monitoring
- **Small Business**: Commercial energy cost control
- **Industrial Monitoring**: Equipment power consumption tracking
- **Educational**: Energy awareness and conservation projects
- **Smart Buildings**: Automated building energy management
- **Solar Systems**: Grid consumption monitoring with solar integration

## Energy Management Workflow
1. **Continuous Monitoring**: Real-time current and power measurement
2. **Data Logging**: Energy consumption accumulation
3. **Threshold Checking**: Compare against set kWh limit
4. **Alert Generation**: SMS notifications when approaching limits
5. **Automated Control**: Power cutoff after 15-minute warning period
6. **Remote Management**: Mobile app control and monitoring

## Advanced Features
- **Historical Data**: Energy consumption trends and analysis
- **Cost Optimization**: Rate-based consumption recommendations
- **Load Scheduling**: Automated high-consumption device control
- **Grid Integration**: Smart grid compatibility for demand response
- **Backup Communication**: Multiple alert methods (SMS, WiFi, local)

## Installation Guidelines
- **Current Transformer**: Install on main electrical feed
- **Safety**: Ensure proper electrical isolation and safety measures
- **GSM Signal**: Verify cellular coverage for SMS functionality
- **Power Supply**: Stable 5V supply for ESP32 and peripherals
- **Weatherproofing**: Protect electronics in appropriate enclosures

## Maintenance and Monitoring
- **Regular Calibration**: Monthly current sensor verification
- **SIM Card Management**: Monitor cellular plan and balance
- **System Updates**: Firmware updates and feature enhancements
- **Data Backup**: Regular export of consumption data
- **Performance Monitoring**: System health and connectivity checks
