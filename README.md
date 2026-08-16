# esp32-smart-environment-monitor
The smart environment monitor is a system designed to track the temperature, humidity and light level of any given environment. The system utilizes an ESP32 microcontroller to collect sensor data, display the information in real-time on an OLED display and has configurable alerts when variables such as light or temperature exceed set thresholds.

![Overhead View of Hardware](images/project_overhead.jpeg)

# Current Features
- Temperature and humidity tracking using a DHT11 sensor
- DHT11 error handling
- Ambient light monitoring using a photoresistor
- Local sensor display using an SH1106 128x64 OLED
- Configurable temperature and light alerts
- Audible alarm using a passive buzzer
- Status LED for system indication
- Serial command interface for system control and diagnostics
- Non-blocking firmware
- Averaged ADC measurements to reduce light-sensor noise
- Wi-Fi connectivity
- Embedded HTTP web server for remote system monitoring

# Hardware
- ESP32 dev board
- DHT11 temperature/humidity sensor
- Photoresistor
- Passive buzzer
- Status LED
- SH1106 128x64 OLED display

![Local OLED Display Dashboard](images/OLED_dashboard.jpeg)

# System Architecture
The embedded firmware is organized into separate tasks responsible for individual system functions. Collected data is stored in a centralized structure called 'systemData'. The use of non-blocking firmware using millis()-based task scheduling allows for each separate task to periodically update 'systemData', manage the OLED and web interface without interruption.

# Serial Commands
The following commands are supported through the Serial Monitor:
- STATUS
- ALARM ON
- ALARM OFF
- LED ON
- LED OFF

Status provides a summary of the current sensor readings and alarm state.

# Web Interface
When the ESP32 is successfully connected to a Wi-Fi network, it starts an embedded HTTP web server. The IP address is printed to the serial monitor and can be entered into a web browser to view the local web dashboard.

The dashboard currently displays:
- Temperature
- Humidity
- Ambient Light level
- Alarm status
- Wi-Fi connection status

![Web Based HTTP Dashboard](images/web_dashboard.jpeg)

# Planned features
- Remote system status and alarm control
- Automatic refresh for system updates
- Historical sensor data logging
- Additional environmental sensors

# Project Goals
The intention of developing this project was to gain practical experience with embedded systems, sensor interfacing, ADC measurements, I2C communication, non-blocking firmware, Wi-Fi networking and Git-based version controlled software development. 
