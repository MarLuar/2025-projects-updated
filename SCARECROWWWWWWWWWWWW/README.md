# Automated Scarecrow System

## Overview
An intelligent agricultural pest deterrent system that uses motion detection to automatically activate mechanical movements and audio alerts. The system detects wildlife or pests approaching crops and responds with coordinated motor movements and sound to scare them away effectively.

## Features
- **Motion Detection**: PIR sensor for reliable wildlife detection
- **Mechanical Movement**: Dual motor system for realistic scarecrow animation
- **Audio Deterrent**: Buzzer activation for enhanced effectiveness
- **Smart Timing**: Configurable operation duration with automatic shutoff
- **Continuous Operation**: Re-triggers if motion persists after cycle completion
- **Debounce Protection**: Prevents false triggers from sensor noise
- **Power Management**: Efficient operation with minimal power consumption

## System Operation
1. **Detection Phase**: PIR sensor monitors for motion
2. **Activation**: Upon detection, motors and buzzer activate simultaneously
3. **Forward Motion**: Motors run forward for 5 seconds
4. **Reverse Motion**: Motors reverse direction for another 5 seconds  
5. **Cooldown Period**: 2-second delay before re-arming system
6. **Persistence Check**: Re-activates if motion still detected after cooldown

## Hardware Components
- **Arduino Uno/Nano** microcontroller
- **PIR Motion Sensor** (HC-SR501) for detection
- **2x DC Motors** for mechanical movement
- **Motor Driver** (L298N or similar) for motor control
- **Buzzer** for audio deterrent
- **Power Supply** (12V for motors, 5V for logic)
- **Mechanical Frame** for scarecrow mounting

## Technical Specifications
- **Detection Range**: Up to 7 meters (depending on PIR sensor)
- **Motion Duration**: 10 seconds total (5s forward + 5s reverse)
- **Cooldown Period**: 2 seconds between cycles
- **Debounce Time**: 500ms for stable detection
- **Operating Voltage**: 5V logic, 12V motors
- **Power Consumption**: Low standby, moderate during activation

## Pin Configuration
- **Motor 1**: Pins 3, 5 (PWM control)
- **Motor 2**: Pins 6, 9 (PWM control)  
- **PIR Sensor**: Pin 7 (Digital input)
- **Buzzer**: Pin 2 (Digital output)

## Setup Instructions
1. Mount scarecrow frame in target area
2. Install PIR sensor with clear field of view
3. Connect motors to mechanical movement system
4. Wire components according to pin configuration
5. Upload code to Arduino
6. Test detection range and adjust PIR sensitivity
7. Deploy in agricultural setting

## Installation Guidelines
- **Height**: Mount PIR sensor 2-3 meters above ground
- **Angle**: Position for optimal coverage of protected area
- **Power**: Use weatherproof enclosure for electronics
- **Maintenance**: Check connections and clean PIR lens regularly

## Applications
- **Crop Protection**: Defend vegetable gardens and farms
- **Orchard Security**: Protect fruit trees from birds and animals
- **Garden Defense**: Residential garden pest control
- **Agricultural Automation**: Large-scale farm protection systems
- **Wildlife Management**: Humane animal deterrent for sensitive areas

## Customization Options
- **Timing Adjustment**: Modify motion duration and cooldown periods
- **Sensitivity Tuning**: Adjust PIR sensor detection range
- **Movement Patterns**: Program different motor sequences
- **Sound Variations**: Add different buzzer patterns or external speakers
- **Multi-Zone**: Deploy multiple units for comprehensive coverage

## Advanced Features
- **Solar Power**: Add solar panel for off-grid operation
- **Wireless Control**: Remote monitoring and control capabilities  
- **Data Logging**: Track activation frequency and patterns
- **Weather Resistance**: Weatherproof enclosures for outdoor use
- **Multiple Sensors**: Expand detection coverage with additional PIR units

## Troubleshooting
- **False Triggers**: Adjust PIR sensitivity and add physical shields
- **Motor Issues**: Check power supply and driver connections
- **Detection Problems**: Clean PIR lens and verify wiring
- **Power Management**: Monitor battery levels for solar systems

## Maintenance Schedule
- **Weekly**: Visual inspection and battery check
- **Monthly**: Clean PIR sensor and test all functions
- **Seasonally**: Weather seal inspection and motor lubrication
- **Annually**: Component replacement and system upgrade
