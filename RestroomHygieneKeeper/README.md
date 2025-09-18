# Restroom Hygiene Keeper

## Overview
An automated restroom hygiene management system that maintains cleanliness through scheduled UV sterilization, automated chemical dispensing, air circulation control, and fragrance distribution. The system operates on a time-based schedule with manual override capabilities.

## Features
- **UV Sterilization**: Automated UV light activation at midnight for disinfection
- **Chemical Dispensing**: Servo-controlled pellet dropper for cleaning agents
- **Air Quality Management**: Exhaust fan control for ventilation
- **Fragrance System**: Automated air freshener activation
- **Real-time Clock**: DS1302 RTC for precise scheduling
- **Manual Controls**: Override buttons for all functions
- **Safety Alerts**: Buzzer notifications during UV operation

## System Operation Schedule
- **Daily UV Cycle**: Activates at 00:00 for 5 minutes with buzzer alert
- **Chemical Dispensing**: First drop after UV, then every 3 days
- **Air Management**: Every 3 hours - exhaust fan off, diffuser on (20 min), then exhaust fan on (45 min)

## Hardware Components
- Arduino Uno/compatible microcontroller
- DS1302 Real-Time Clock module
- Servo motor (chemical dispenser)
- Relay modules for high-voltage control
- UV light fixture
- Exhaust fan
- Air freshener/diffuser
- Buzzer for alerts
- Manual override buttons

## Pin Configuration
- Exhaust Fan: Digital Pin 4
- UV Light: Digital Pin 6
- Buzzer: Digital Pin 5
- Servo (Dropper): Digital Pin 10
- Diffuser Control: Digital Pin 2
- RTC: Pins 7 (IO), 8 (RST), 9 (CLK)
- Manual Controls: Analog pins A2-A5

## Setup Instructions
1. Install required libraries:
   - `Servo`
   - `Wire`
   - `RtcDS1302`

2. Wire components according to pin configuration
3. Set correct time on RTC module
4. Upload code to Arduino
5. Test manual controls before automation

## Safety Features
- UV light operation with audible warning
- Manual override for all functions
- Timed operations to prevent overuse
- Safe chemical dispensing intervals

## Applications
- Public restroom automation
- Healthcare facility hygiene
- Commercial bathroom maintenance
- Smart building integration
- Infection control systems

## Maintenance
- Refill chemical pellets as needed
- Replace UV bulbs periodically
- Clean diffuser system regularly
- Check RTC battery annually
