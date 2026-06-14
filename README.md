# 💧 IoT-Based Automated Patchouli Oil Distillation System

An automated control and monitoring system for the Patchouli Oil (*Minyak Nilam*) distillation process. Developed using the **ESP32** microcontroller, this project automates temperature regulation and water level control to optimize oil yield and maintain system safety.

This project was built using the **Wokwi Simulation Platform** to fulfill the assignment for the Piranti Cerdas (Smart Devices) course at Universitas Muhammadiyah Malang.

---

## 🔗 Online Simulation Link
You can test and run the live circuit simulation directly on Wokwi here:
**https://wokwi.com/projects/446131311217502209**

---

## 📸 Circuit Diagram & System Architecture
<p align="center">
  <img src="screenshots/diagram_rangkaian.png" width="600" 
</p>
<p align="center">
  <img src="screenshots/System_Architecture.png" width="600" 
</p>

---

## 🎯 Background & Objectives
Patchouli oil extraction requires precise temperature stability during steam distillation. Manual monitoring often leads to inefficient heat control, lower oil quality, or safety hazards like overheating. 

The main objectives of this automated system are:
1. **Automated Temperature Regulation**
   Keeping the distillation temperature within the optimal range (99°C - 101°C).
3. **Safety Mechanism**
   Automatically shutting down the heater if temperatures reach a dangerous threshold (150°C) or if water levels drop below a critical point.
5. **Real-time Status Display**
   Providing local visual feedback regarding current temperature and hardware statuses.

---

## 🛠️ Hardware Components Used
1. **Microcontroller**
   ESP32
2. **Sensors**
   Temperature Sensor & Water Level Sensor
3. **Actuators**
   Relays controlling the Heater and Water Pump
4. **Display**
   I2C LCD Display
5. **Indicators**
   Buzzer & LEDs for status/emergency alarms

---

## ⚙️ System Workflow & Automation Logic
The firmware operates based on the following automated rules:
1. **Normal Distillation Window:**
   The heater turns **ON** when the temperature drops below `99°C` and turns **OFF** once it exceeds `101°C`.
2. **Emergency Overheat Cutoff**
   If the temperature triggers the `150°C` mark, the system executes an immediate emergency shutdown, turning off the heater and activating the buzzer alarm.
3. **Water Level Control**
   If the water level is low, the system activates the water pump. If it drops to a critical empty state, the heater is disabled to avoid dry-firing hazards.

---

## 👥 Development Team (Kelompok 11 - Kelas H)
* **Anggun Ramadhani** (202310370311077)

Informatics Engineering Study Program - Faculty of Engineering
**Universitas Muhammadiyah Malang (2025)**
