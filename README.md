# Smart Alarm System Using ESP32

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)

A cognitive-based smart alarm system that requires users to solve math problems to disable the alarm, ensuring they are mentally alert before turning it off.

![Smart Alarm System](images/system_preview.png)

## 📖 Table of Contents
- [Overview](#overview)
- [Features](#features)
- [How It Works](#how-it-works)
- [Hardware Components](#hardware-components)
- [Software Dependencies](#software-dependencies)
- [Installation](#installation)
- [Usage Instructions](#usage-instructions)
- [System Architecture](#system-architecture)
- [Future Enhancements](#future-enhancements)
- [Limitations](#limitations)
- [References](#references)
- [Contributing](#contributing)
- [License](#license)

## 🔍 Overview

The Smart Alarm System addresses a common problem: many people tend to dismiss their alarms impulsively and fall back asleep. This project combines embedded systems with cognitive science to create an alarm that requires users to solve simple math problems before it can be turned off, ensuring they are mentally awake.

This system is particularly useful for individuals who:
- Experience difficulty waking up
- Have a habit of snoozing alarms
- Suffer from sleep inertia (mental fogginess upon waking)
- Need to be punctual for work, classes, or other commitments

## ✨ Features

- **Time Synchronization**: Connects to Wi-Fi to retrieve accurate time from NTP servers
- **Cognitive Challenges**: Generates random math problems that must be solved to disable the alarm
- **Multiple Problem Requirement**: Users must solve multiple problems correctly to turn off the alarm
- **Bluetooth Connectivity**: Allows control and interaction through a mobile device
- **Custom Alarm Settings**: Set personalized wake-up times
- **Persistent Alerting**: Continues to alert until cognitive tasks are completed correctly
- **Low-Cost Implementation**: Built using affordable and readily available components

## 🧠 How It Works

1. The system connects to Wi-Fi and syncs with accurate time servers
2. At the preset alarm time, the buzzer activates
3. The system generates a random math problem via Bluetooth
4. The user must solve the problem correctly; incorrect answers generate new problems
5. After solving the required number of problems (default: 2), the alarm deactivates
6. To effectively turn off the alarm, users must engage mentally, preventing accidental dismissal

## 🔌 Hardware Components

- ESP32 Microcontroller
- Passive Buzzer
- Push Buttons (for a physical interface option)
- Power Source (USB or Portable Battery)
- Wires and Breadboard

## 💻 Software Dependencies

- Arduino IDE
- Required Libraries:
  - WiFi.h
  - BluetoothSerial.h
  - esp_bt.h
  - time.h
- Serial Bluetooth Terminal (mobile app for interaction)

## 📥 Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/Sargamchicholikar/Smart-Alarm-System-ESP32.git
   cd Smart-Alarm-System-ESP32
   ```

2. **Hardware Setup**:
   - Connect the ESP32 to the buzzer as shown in the [Hardware Setup Diagram](#system-architecture)
   - For a physical interface, connect push buttons to the designated GPIO pins

3. **Software Setup**:
   - Open the Arduino IDE
   - Install all required libraries through the Library Manager
   - Open the ESP32_Smart_Alarm.ino file
   - Update the Wi-Fi credentials in the code:
     ```cpp
     const char* ssid = "YourWiFiName";
     const char* password = "YourWiFiPassword";
     ```
   - Set your desired alarm time:
     ```cpp
     int alarmHour = 7;
     int alarmMinute = 30;
     ```
   - Upload the code to your ESP32

4. **Mobile Setup**:
   - Install a Serial Bluetooth Terminal app on your mobile device
   - Connect to the "ESP32_Alarm" Bluetooth device
   - You're ready to interact with the alarm system

## 🔄 Usage Instructions

### Setting an Alarm
Via Bluetooth, send:
```
SET 08:30
```
This sets the alarm for 8:30 AM.

### Checking Current Time
Send:
```
TIME
```

### Manual Alarm Trigger (for testing)
Send:
```
ON
```

### Responding to Math Problems
When the alarm triggers, a math problem will be sent to your Bluetooth terminal. Type the answer and press send. You'll need to solve 2 problems correctly to disable the alarm.

## 🏗️ System Architecture

### Block Diagram
The system consists of:
- ESP32 as the central controller
- Wi-Fi module for time synchronization
- Bluetooth module for user interaction
- Buzzer for audio alerts
- Math problem generator and validator

### Source Code Structure
The main components of the code include:
- Wi-Fi and time synchronization
- Alarm logic and scheduling
- Bluetooth communication handling
- Math problem generation and validation
- Buzzer control

## 🚀 Future Enhancements

- **Advanced Cognitive Challenges**: Adding a wider variety of problem types
- **Voice-Based Interaction**: Implementing voice recognition for answers
- **Mobile Application**: Creating a dedicated app for enhanced control
- **Sleep Cycle Integration**: Adding sensors to detect optimal wake-up times
- **Performance Analytics**: Tracking user performance over time
- **Gamification**: Adding scoring systems and achievements
- **Smart Home Integration**: Connecting with other smart devices

## ⚠️ Limitations

- Currently limited to basic arithmetic operations
- Requires Bluetooth for full functionality
- Alarm times must be set through code or Bluetooth commands
- No persistent storage for user preferences
- Limited feedback mechanisms
- No analytics or performance tracking

## 📚 References

1. [NTPClient Library Documentation](https://github.com/arduino-libraries/NTPClient)
2. [ESP32 Technical Reference](https://www.espressif.com/en/products/socs/esp32/resources)
3. [Cognitive Engagement & Sleep Inertia Research](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC5372414/)

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the repository
2. Create your feature branch: `git checkout -b feature/amazing-feature`
3. Commit your changes: `git commit -m 'Add some amazing feature'`
4. Push to the branch: `git push origin feature/amazing-feature`
5. Open a Pull Request

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

**Developed as part of an academic project at Bharati Vidyapeeth (Deemed to Be) University College of Engineering, Pune**

*Note: This is an educational project intended for personal use and learning.*
