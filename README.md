#  Smart Earthing Monitoring System ( SIH-1582 )

<p align="center">
An ESP32-based IoT system for continuous monitoring of electrical earthing conditions with real-time fault detection, cloud dashboard, and intelligent alert notifications.
</p>

---

#  Overview

Electrical earthing is one of the most important safety systems in residential, commercial, and industrial electrical installations. Traditional earthing inspection methods are manual and periodic, making it difficult to identify faults immediately.

The **Smart Earthing Monitoring System** provides continuous monitoring of earthing conditions using an ESP32 microcontroller and IoT technology. The system measures supply voltage, load current, leakage current, and soil moisture in real time.

Whenever an abnormal condition is detected, the system immediately alerts the user through LEDs, buzzer, LCD display, and the Blynk IoT Cloud Dashboard. Historical data is also stored on the cloud for monitoring and maintenance purposes.

The project aims to improve electrical safety, reduce maintenance costs, and prevent electrical accidents caused by poor earthing.

---

#  Features

-  Real-Time Voltage Monitoring
-  Load Current Measurement
-  Leakage Current Detection
-  Soil Moisture Monitoring
-  ESP32 Wi-Fi Connectivity
-  Blynk IoT Cloud Integration
-  Remote Monitoring
-  Historical Data Logging
-  Automatic Fault Alerts
-  Audible Buzzer Notification
-  LED Status Indicators
-  16×2 LCD Display
-  Continuous Monitoring
-  Low-Cost Electrical Safety Solution

---

#  Project Objectives

- Monitor electrical earthing continuously.
- Detect earth leakage current in real time.
- Measure soil moisture around the earthing pit.
- Display live electrical parameters locally.
- Upload sensor data to the cloud.
- Alert users immediately during unsafe conditions.
- Reduce manual inspection requirements.
- Improve electrical safety and maintenance efficiency.

---

#  System Architecture

```
                 230V AC Supply
                        │
              Voltage Sensor (ZMPT101B)
                        │
        ┌───────────────┴───────────────┐
        │                               │
   ACS712 Load Sensor             ACS712 Leakage Sensor
        │                               │
        └───────────────┬───────────────┘
                        │
                 ESP32 Controller
                        │
      ┌──────────┬───────────┬────────────┐
      │          │           │            │
     LCD        LEDs      Buzzer      Wi-Fi
                                         │
                                 Blynk IoT Cloud
                                         │
                               Mobile / Web Dashboard
```

---

#  Hardware Components

| Component | Quantity |
|------------|----------|
| ESP32 Board | 1 |
| ACS712 Current Sensor | 2 |
| ZMPT101B Voltage Sensor | 1 |
| Soil Moisture Sensor | 1 |
| 16×2 LCD Display | 1 |
| I2C Module | 1 |
| Active Buzzer | 1 |
| LEDs | 3 |
| 330Ω Resistors | 3 |
| Breadboard / PCB / Perfboard | 1 |
| AC Loads | 2 |
| Power Supply 

---

#  Software & Technologies

- Arduino IDE
- ESP32 Board Package
- Blynk IoT Platform
- C/C++
- WiFi Library
- Wire Library
- LiquidCrystal_I2C Library
- Git & GitHub

---

#  Working Principle

1. The ESP32 continuously acquires analog signals from the voltage sensor, load current sensor, leakage current sensor, and soil moisture sensor.

2. The voltage sensor measures the RMS supply voltage.

3. The ACS712 sensors calculate the RMS load current and leakage current.

4. The soil moisture sensor monitors moisture levels around the earthing electrode.

5. The ESP32 processes the acquired sensor values every second.

6. Measured parameters are displayed on the 16×2 LCD.

7. Sensor data is transmitted to the Blynk IoT Cloud through Wi-Fi.

8. The Blynk Dashboard provides real-time visualization and historical data logging.

9. If leakage current exceeds the threshold for more than three seconds, an Earth Fault alert is generated.

10. If soil moisture falls below the safe limit, a Low Moisture alert is generated.

11. LEDs and the buzzer provide immediate local warning notifications.

---

#  ESP32 Pin Configuration

| GPIO | Connected Device |
|--------|----------------|
| GPIO34 | Voltage Sensor |
| GPIO35 | Load Current Sensor |
| GPIO33 | Leakage Current Sensor |
| GPIO32 | Soil Moisture Sensor |
| GPIO21 | LCD SDA |
| GPIO22 | LCD SCL |
| GPIO23 | Active Buzzer |
| GPIO5 | Fault LED |
| GPIO18 | OK LED |
| GPIO17 | Moisture LED |

---

#  Dashboard

The project uses the **Blynk IoT Platform** for cloud monitoring.

Dashboard features include:

- Live Voltage
- Load Current
- Leakage Current
- Soil Moisture
- Fault Status
- Historical Data Logging
- Remote Monitoring
- Real-Time Alerts

---

#  Fault Detection Logic

## Earth Fault Detection

The system continuously compares leakage current with the predefined threshold.

An Earth Fault is confirmed only if:

- Leakage Current exceeds the threshold.
- Load Current is present.
- Fault persists for more than 3 seconds.

When confirmed:

- Fault LED turns ON.
- Buzzer activates.
- Dashboard displays **EARTH FAULT**.

---

## Low Moisture Detection

If soil moisture falls below the safe threshold:

- Moisture LED turns ON.
- Buzzer activates.
- Dashboard displays **LOW MOISTURE**.

---

#  Prototype

## System Prototype

<img width="1600" height="1204" alt="image" src="https://github.com/user-attachments/assets/a8adf2c1-2d54-4315-a8da-ad58c079260b" />

---

## Hardware Setup

<img width="1204" height="1600" alt="image" src="https://github.com/user-attachments/assets/b2427e80-6953-4319-89b8-237fdb185ace" />

---

## Circuit Diagram

<img width="1024" height="768" alt="image" src="https://github.com/user-attachments/assets/e77f0ac1-f71e-4a39-8cdc-565976ea0183" />

---

## Blynk Dashboard

> Replace with dashboard screenshot.

<img width="1600" height="1204" alt="image" src="https://github.com/user-attachments/assets/4865a58f-7ab4-4241-a296-243bb140d5f4" />

<img width="1600" height="1204" alt="image" src="https://github.com/user-attachments/assets/e5e851c9-d759-43ad-9b1f-a00ad62c3eb6" />

---

#  Repository Structure

```
Smart-Earthing-Monitoring-System
│
├── firmware
│   └── Smart_Earthing_ESP32.ino
│
├── hardware
│   ├── Circuit_Diagram.png
│   ├── Prototype.jpg
│   └── Components_List.pdf
│
├── images
│   ├── Dashboard.png
│   ├── Dashboard_Fault.png
│   ├── Prototype.jpg
│   ├── Hardware.jpg
│   └── System_Diagram.png
│
├── videos
│   └── Demo.mp4
│
├── docs
│   └── Project_Report.pdf
│
├── README.md
└── LICENSE
```

---

#  Installation

Clone the repository.

```bash
git clone https://github.com/yourusername/Smart-Earthing-Monitoring-System.git
```

Open the project using Arduino IDE.

Install the required libraries.

Configure:

- Wi-Fi SSID
- Wi-Fi Password
- Blynk Authentication Token

Upload the code to the ESP32.

---

#  Usage

Power the ESP32.

Connect the system to Wi-Fi.

Open the Blynk Dashboard.

Monitor:

- Voltage
- Load Current
- Leakage Current
- Soil Moisture

Receive alerts whenever abnormal conditions occur.

---

#  Results

The developed Smart Earthing Monitoring System successfully:

- Continuously monitors electrical earthing conditions.
- Detects leakage current accurately.
- Measures soil moisture around the earthing system.
- Displays live sensor data on LCD.
- Uploads data to the Blynk IoT Cloud.
- Generates real-time alerts during abnormal conditions.
- Stores historical data for maintenance analysis.
- Improves electrical safety and reduces manual inspection.

---

#  Applications

- Residential Buildings
- Industrial Plants
- Electrical Substations
- Smart Buildings
- Solar Power Systems
- Hospitals
- Data Centers
- Educational Institutions
- Commercial Buildings

---

#  Future Improvements

- GSM/SMS Alert System
- Mobile Push Notifications
- MQTT Integration
- AI-Based Fault Prediction
- LoRa Communication
- Automatic Relay Trip Mechanism
- Three-Phase Monitoring
- Web Dashboard Analytics
- Predictive Maintenance using Machine Learning

---

#  License

This project is licensed under the MIT License.

---

#  Author

**Kailash S**

Electrical and Electronics Engineering

Chennai Institute of Technology

Embedded Systems | IoT | PCB Design | Robotics | Drone Technology

---

⭐ If you found this project useful, consider giving it a **Star** on GitHub!

