# esp32-smart-environment-monitor
The smart environment monitor is a system designed to track the temperature, humidity and light level of any given environment. The system utilizes an ESP32 microcontroller to collect sensor-data, display the information in real-time on an OLED display and has configurable alerts when variables such as light or temperature exceed set thresholds.

# Current Features
- Temperature and humidity tracking using a DHT11 arduino sensor
- Ambient light monitoring using a photoresistor
- Real-time data updates using an SH1106 OLED display
- Configurable temperature and light alerts
- Audible alarm using a passive buzzer
- Status LED for system indication
- Serial command interface for system control and diagnostics
- Non-blocking firmware
- Averaged ADC measurements to reduce light-sensor noise

# Hardware
- ESP32
- DHT11 temperature/humidity sensor
- Photoresistor
- Passive buzzer
- Status LED
- SH1106 128x64 OLED display

# Planned features
- Wi-fi connectability
- Web-based monitoring interface
- Remote system status and alarm control
