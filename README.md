# Smart Automated Car Parking System 🚗🅿️

An Arduino-based project to manage car parking slots automatically using sensors and servo motor.

## 🎯 Features
- IR sensors to detect slot availability and car presence
- Ultrasonic sensor to detect approaching vehicles
- Servo motor to open/close gate
- LCD screen for real-time display
- Buzzer alert when parking is full


## 🧰 Components Used
- Arduino Uno
- IR sensors x6
- Ultrasonic sensor (HC-SR04)
- Servo motor
- 16x2 I2C LCD display
- Buzzer
- Breadboard and jumper wires

## 🧠 How It Works
- When a car approaches, the ultrasonic sensor detects it.
- If there are available slots, the gate opens using a servo motor.
- Slot status is checked using IR sensors.
- LCD shows available/free slots dynamically.
- If parking is full, the system alerts the driver via buzzer and display.

## 📁 Code File
See [`SmartCarParking.ino`](SmartCarParking.ino)

---

Made with ❤️ by [Rikha Akther]
