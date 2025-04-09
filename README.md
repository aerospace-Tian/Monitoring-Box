# Monitoring-Box
# 📸 Motion-Triggered Environmental Monitoring System
![Image](https://github.com/user-attachments/assets/c25c29aa-9edf-47a1-9925-c048fdb26642)

This project is a **compact, low-power system for environmental monitoring**, designed to automatically detect motion and capture images in the field. It's ideal for use in wildlife observation, outdoor data logging, and environmental sensing applications where continuous human supervision is not feasible.

## 🌿 Overview

The system uses a **PIR (Passive Infrared) sensor** to detect motion in its surroundings. Upon detecting movement, the system activates an onboard **camera module** to capture still images, which can be stored or transmitted depending on the configuration.

At its core is a **Seeed Studio XIAO ESP32S3 Sense** microcontroller, which includes:
- Built-in camera
- WiFi/Bluetooth capabilities
- Ultra-low power operation

To ensure visibility in low-light conditions, **controllable LEDs** are placed around the camera. These LEDs are automatically triggered along with the camera upon motion detection to provide sufficient illumination for image capture.

---

## 🔧 Hardware Components

- **Seeed Studio XIAO ESP32S3 Sense**
  - Integrated camera
  - Small form factor
  - Low power consumption
- **PIR Motion Sensor**
  - Detects movement of humans or animals
- **LED Ring or Discrete LEDs**
  - Provides light for image capture in dark environments
- **1S LiPo Battery (3.7V)**
  - Portable and rechargeable power supply
  - Energy-efficient design ensures longer battery life
- *(Optional)* **MicroSD or cloud storage**
  - For storing captured images

---

## ⚙️ Features

- ✅ Motion-triggered image capture  
- 💡 Auto-activated LED lighting for night/dark use  
- 🔋 Energy-efficient, suitable for off-grid deployment  
- 🔁 Expandable for remote data upload (via WiFi or BLE)  
- 🔧 Compact and modular design for field deployment  

---

## 🔌 Power Management

The entire system runs on a **1-cell LiPo battery** (3.7V). The ESP32S3's low-power sleep modes are used between events to significantly reduce energy consumption and extend battery life.

---

## 📷 Use Cases

- Wildlife camera traps  
- Forest or park surveillance  
- Greenhouse or crop monitoring  
- DIY smart doorbell or motion camera  

---

## 🚀 Getting Started

1. **Flash the firmware** to the XIAO ESP32S3 using the Arduino IDE or PlatformIO.
2. Connect the PIR sensor and LEDs according to your pin configuration.
3. Insert the battery and deploy the system in your desired location.
4. Images will be captured upon motion detection and stored/transmitted as configured.

---

## 🔄 Future Improvements

- Cloud storage integration (e.g., via WiFi to Firebase or MQTT)
- Image classification or object detection on-device
- Solar charging for long-term outdoor deployment
- Data logging with environmental sensors (temperature, humidity, etc.)

---

## 🛠️ License

This project is open-source and available under the [MIT License](LICENSE).



