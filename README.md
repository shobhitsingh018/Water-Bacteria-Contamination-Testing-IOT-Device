# 💧 IoT Bio-Telemetry: Real-Time Water Bacteria Detection

An end-to-end IoT framework designed to detect bacteriological contamination in water samples using multi-sensor data fusion and edge-deployed Machine Learning.

## 🛠️ System Architecture
- **Hardware Layer:** ESP32 Microcontroller, TCS3200 Color Sensor, pH Sensor, TDS Meter, and Turbidity Sensor.
- **Edge Analytics:** Python-based server processing sensor data to predict Colony Forming Units (% CFU).
- **Computer Vision:** Optical analysis of water samples to estimate bacterial presence via RGB-to-CFU mapping.

## 📂 Repository Structure
- `/code`: Core firmware (Arduino/ESP32) and Python Deep Learning models.
- `/hardware`: KiCad PCB schematics and 3D-printable housing files.
- `/datasets`: Raw calibration data for TDS, pH, and RGB-CFU correlation.
- `/docs`: Technical presentation and project specifications.

## 🚀 Getting Started
1. **Hardware:** Follow the KiCad schematics in `/hardware` to assemble the testing kit.
2. **Firmware:** Flash the ESP32 using the code in `code/FINAL/FINAL.ino`.
3. **Analytics:** Run `deep_learning_model.ipynb` to calibrate the CFU prediction model with your local data.
