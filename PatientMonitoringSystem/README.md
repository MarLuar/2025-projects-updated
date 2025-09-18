# Patient Monitoring System

## Overview
A comprehensive wireless patient vital signs monitoring system using ESP32 and ESP-NOW protocol. The system consists of a bedside patient unit that measures vital signs and a nurse station receiver that displays and monitors multiple patients' data in real-time.

## System Architecture
- **Patient Unit (Sender)**: Collects vital signs and transmits via ESP-NOW
- **Nurse Station (Receiver)**: Displays data from multiple patient units
- **Wireless Communication**: ESP-NOW for reliable, low-latency data transmission

## Measured Vital Signs
- **Heart Rate (BPM)**: Using MAX30105 pulse oximeter sensor
- **Blood Oxygen (SpO2)**: Calculated from pulse oximetry data
- **Body Temperature**: Contactless measurement via MLX90614 IR sensor
- **ECG Signal**: Basic ECG monitoring using AD8232 module
- **Finger Detection**: Automatic sensor activation when finger is detected

## Hardware Components

### Patient Unit (Sender)
- ESP32 microcontroller
- MAX30105 heart rate and SpO2 sensor
- MLX90614 non-contact temperature sensor
- AD8232 ECG sensor module
- 20x4 I2C LCD display
- Custom I2C bus for temperature sensor

### Nurse Station (Receiver)
- ESP32 microcontroller
- Display system for multi-patient monitoring
- Alert system for vital sign abnormalities

## Features
- **Real-time Monitoring**: Continuous vital sign measurement and transmission
- **Multi-patient Support**: One nurse station can monitor multiple patients
- **Automatic Detection**: Finger presence detection for accurate readings
- **Data Validation**: Built-in error checking and data validation
- **Low Power**: Efficient ESP-NOW communication protocol
- **User-friendly Display**: Clear LCD readouts with status indicators

## Technical Specifications
- Update interval: 500ms for display, 5s for SpO2 calculations
- Communication range: Up to 250m (line of sight)
- Temperature accuracy: ±0.5°C
- Heart rate range: 20-255 BPM
- SpO2 simulation: 95-100% (for demonstration)

## Setup Instructions
1. Install required libraries:
   - `MAX30105lib`
   - `Adafruit_MLX90614`
   - `LiquidCrystal_I2C`
   - `ESP32 ESP-NOW`

2. Configure MAC addresses for sender/receiver pairing
3. Wire sensors according to pin definitions
4. Upload sender code to patient unit ESP32
5. Upload receiver code to nurse station ESP32
6. Calibrate sensors for accurate readings

## Pin Configuration

### Patient Unit
- MAX30105: I2C (SDA: 21, SCL: 22)
- MLX90614: Custom I2C (SDA: 18, SCL: 19)
- LCD: I2C address 0x27
- ECG: Analog pin 34

## Applications
- Hospital patient monitoring
- Home healthcare systems
- Elder care facilities
- Remote patient monitoring
- Medical research and data collection
- Telemedicine support

## Data Structure
```cpp
struct SensorData {
    int irValue;           // Infrared sensor value
    int beatAvg;           // Average heart rate
    int fakeSpO2;          // Blood oxygen percentage
    float objectTemp;      // Body temperature
    int ecgValue;          // ECG reading
    bool fingerDetected;   // Finger presence status
}
```

## Future Enhancements
- Data logging to SD card
- Web interface for remote monitoring
- Alert system with SMS/email notifications
- Integration with hospital management systems
- Machine learning for anomaly detection
