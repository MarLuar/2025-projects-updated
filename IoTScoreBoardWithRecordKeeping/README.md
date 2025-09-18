# IoT Scoreboard with Record Keeping

## Overview
A comprehensive digital scoreboard system with cloud connectivity and persistent data storage. Features multiple game zones with LED matrix displays, Blynk app integration for remote control, and local file storage using SPIFFS for game record keeping and statistics tracking.

## Features
- **Multi-Zone Display**: 6 separate scoring zones with LED matrix displays
- **Remote Control**: Blynk mobile app integration for wireless operation
- **Data Persistence**: Local file storage system with SPIFFS
- **Game Management**: Save/load/delete game records (up to 10 games)
- **Countdown Timer**: Built-in game timer with pause/resume functionality
- **Timeout LED**: Visual timeout indicator with automatic control
- **Record Keeping**: Comprehensive game statistics and history
- **WiFi Connectivity**: Real-time updates and remote monitoring

## Hardware Components
- **ESP8266** microcontroller (NodeMCU/Wemos D1)
- **24x LED Matrix Displays** (MAX7219-based, FC16 modules)
- **Timeout LED** indicator
- **SPI Connection** for LED matrix communication

## Display Zones Configuration
- **Zone 1**: Team 1 Score (4 LED modules)
- **Zone 2**: Game Details Display (4 modules)
- **Zone 3**: Team 2 Score (4 modules)  
- **Zone 4**: Game Timer (4 modules)
- **Zone 5**: Additional Info (4 modules)
- **Zone 6**: Status Display (4 modules)

## Blynk App Controls
- **Score Management**: Individual team score controls
- **Timer Control**: Start/stop/pause game timer
- **Game Files**: Select from 10 different game records
- **Data Operations**: Save current game, load previous games
- **Reset Functions**: Clear scores and reset system
- **File Management**: Delete game records

## File System Features
- **CSV Format**: Game data stored in readable CSV format
- **Automatic Backup**: Games automatically saved with timestamps
- **Data Structure**: Zone, Score1, Score2, Game Details, Fouls
- **File Naming**: game1.csv through game10.csv for organization
- **SPIFFS Storage**: Reliable flash file system for data persistence

## Setup Instructions
1. Install required libraries:
   - `MD_Parola`
   - `MD_MAX72XX`
   - `BlynkSimpleEsp8266`
   - `ESP8266WiFi`
   - `FS` (SPIFFS)

2. Configure Blynk credentials:
   - Update `BLYNK_AUTH_TOKEN`
   - Set `BLYNK_TEMPLATE_ID`
   - Configure WiFi credentials

3. Wire LED matrices according to SPI configuration
4. Upload code to ESP8266
5. Set up Blynk mobile app dashboard

## Pin Configuration
- **CLK_PIN**: D13 (SPI Clock)
- **DATA_PIN**: D11 (SPI Data)
- **CS_PIN**: D10 (Chip Select)
- **Timeout LED**: D2

## Blynk Virtual Pins
- V0-V7: Score controls and timer functions
- V8: Game file selection (1-10)
- V19: Delete file button
- Additional pins for advanced features

## Applications
- **Sports Events**: Basketball, volleyball, soccer scoreboards
- **Tournament Management**: Multi-game tracking and statistics
- **School Sports**: Gymnasium and field scoreboards
- **Recreation Centers**: Community sports programs
- **E-sports**: Gaming tournament displays
- **Training Sessions**: Practice game monitoring

## Data Management
- **Game Statistics**: Automatic score tracking and history
- **Performance Analytics**: Team and player statistics over time
- **Tournament Records**: Complete game history with timestamps
- **Export Capability**: CSV files for external analysis
- **Backup System**: Reliable data preservation

## Advanced Features
- **Multi-Game Support**: Handle different game types and rules
- **Custom Messages**: Scrolling text for announcements
- **Real-time Updates**: Live score updates via internet
- **Mobile Notifications**: Score alerts and game status updates
- **Web Interface**: Browser-based control panel (future enhancement)

## Maintenance
- Regular SPIFFS cleanup for storage optimization
- WiFi connection monitoring and auto-reconnection
- LED matrix calibration and brightness adjustment
- Blynk token renewal and app updates
