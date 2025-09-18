# Asset Monitoring System

## Overview
A comprehensive RFID-based asset tracking and management system for educational institutions and offices. The system tracks the borrowing and returning of various assets (laptops, projectors, cables, etc.) with automatic Google Sheets logging and real-time status monitoring.

## Features
- **RFID Authentication**: Dual-stage scanning for students and assets
- **Real-time Tracking**: Automatic borrow/return status tracking
- **Google Sheets Integration**: Cloud-based data logging and management
- **Multi-User Support**: Multiple student ID recognition
- **Asset Status Management**: Toggle between borrowed/returned states
- **Visual Feedback**: LCD display and LED indicators
- **WiFi Connectivity**: Internet connection for cloud data synchronization

## Supported Assets
The system currently tracks these assets:
1. **Laptop** - High-value portable computing device
2. **Projector** - Presentation equipment
3. **TV** - Display equipment
4. **HDMI Cable** - Connectivity hardware
5. **Extension Cord** - Power distribution
6. **Stapler** - Office supplies
7. **Speaker** - Audio equipment

## System Architecture
1. **Student Identification**: Students scan their RFID cards first
2. **Asset Selection**: After ID verification, students scan asset RFID tags
3. **Status Toggle**: System automatically toggles between borrow/return
4. **Data Logging**: Transaction details sent to Google Sheets
5. **Status Display**: LCD shows current operation and asset status

## Hardware Components
- **ESP32** microcontroller with WiFi capability
- **MFRC522 RFID Reader** for card and tag scanning
- **16x2 I2C LCD Display** for user interface
- **LED Indicator** for visual feedback
- **RFID Cards** for student identification
- **RFID Tags** for asset identification

## Student Database
Currently registered students:
- **Mar Luar Igot** (ID: 23233299)
- **Hannah Tan** (ID: 24204398)
- **Crisha Luga** (ID: 24204638)
- **Lyka Fujita** (ID: 24204737)
- **Francis Cabanes** (ID: 24205197)

## Pin Configuration
- **RFID Reader**: SDA - GPIO 5, RST - GPIO 4
- **LCD Display**: I2C (SDA - GPIO 21, SCL - GPIO 22)
- **LED Indicator**: GPIO 25
- **Power**: 3.3V and 5V rails

## Setup Instructions
1. Install required libraries:
   - `MFRC522` for RFID functionality
   - `LiquidCrystal_I2C` for display
   - `WiFi` and `HTTPClient` for connectivity

2. Configure Google Apps Script:
   - Create Google Sheets spreadsheet
   - Set up Apps Script for data reception
   - Update script URL in code

3. Hardware assembly:
   - Connect RFID reader to ESP32
   - Wire LCD display via I2C
   - Install LED indicator
   - Mount in appropriate enclosure

4. Software configuration:
   - Update WiFi credentials
   - Configure Google Sheets script URL
   - Upload code to ESP32

## Usage Workflow
1. **Student Scan**: Student places RFID card on reader
2. **Verification**: System displays student name and prompts for asset
3. **Asset Scan**: Student scans asset RFID tag
4. **Status Update**: System determines borrow/return action
5. **Data Logging**: Transaction recorded in Google Sheets
6. **Confirmation**: LCD displays transaction status

## Google Sheets Integration
Data logged includes:
- Asset name and ID
- Student name and ID
- Action (Borrowed/Returned)
- Timestamp
- Transaction status

## Applications
- **Educational Institutions**: School and university equipment management
- **Corporate Offices**: IT equipment and resource tracking
- **Libraries**: Book and media lending systems
- **Laboratories**: Scientific equipment management
- **Workshops**: Tool and equipment checkout systems

## Advanced Features
- **Asset Status Memory**: System remembers current borrow/return state
- **URL Encoding**: Proper handling of special characters in data
- **Error Handling**: Network and scanning error management
- **Visual Feedback**: LED and LCD status indicators
- **Automatic Reset**: System returns to idle state after operations

## Data Security
- **Local Processing**: Asset status maintained locally
- **Secure Transmission**: HTTPS for Google Sheets integration
- **User Authentication**: RFID-based access control
- **Data Integrity**: Error checking and validation

## Maintenance
- **RFID Cards**: Replace damaged or lost student cards
- **Asset Tags**: Maintain and replace worn asset tags
- **Network**: Monitor WiFi connectivity and Google Sheets access
- **Hardware**: Regular cleaning and inspection of components
- **Software**: Update student database and asset list as needed

## Future Enhancements
- **Mobile App**: Smartphone interface for management
- **Barcode Support**: Additional identification methods
- **Reporting Dashboard**: Advanced analytics and reporting
- **Multi-location**: Support for multiple tracking stations
- **Integration**: Connect with existing asset management systems
