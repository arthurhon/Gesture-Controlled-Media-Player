# Gesture-Controlled Media Player 

Control your computer's media playback using hand gestures, powered by an ESP32, MPU6050 accelerometer, and a custom-trained AI model via Edge Impulse.

## Features
- Gesture recognition with on-device ML inference
- Media control via BLE (play/pause, next/prev track, volume up/down)
- Trained with Edge Impulse and deployed to run entirely on the ESP32

## Tech Stack
- Hardware: ESP32 DevKit, MPU6050
- ML Platform: Edge Impulse
- Firmware: Arduino (ESP32 board support)
- BLE Media Control: ESP32 BLE Keyboard library

## Recognized Gestures
- Swipe Left → Previous Track  
- Swipe Right → Next Track 
- Poke → Play/Pause 
- Clockwise → Volume Up 
- Anticlockwise → Volume Down 

## Demo
https://youtu.be/hSEMkWg0UvM

## How It Works
1. MPU6050 collects accelerometer data.
2. The data is processed and fed into a neural network deployed from Edge Impulse.
3. Prediction result is parsed, and if confidence > 0.95, a corresponding BLE media key is sent to the host PC.

## Folder Structure
```
├── code/               # Arduino sketch (.ino file)
├── model/              # Edge Impulse exported model (C++ SDK files)
├── media/              # Photos, wiring diagrams, demo clips
└── README.md           # This file
```

## Setup Instructions
1. Clone this repo
2. Connect MPU6050 to ESP32 (SDA to GPIO21, SCL to GPIO22 by default)
3. Import the `.ino` sketch into Arduino IDE
4. Install required libraries:
   - Adafruit MPU6050
   - ESP32 BLE Keyboard
   - Edge Impulse SDK (include your model header file)
   - Wire (built-in Arduino I2C library)
5. Flash to ESP32 and pair it with your PC over Bluetooth

## Training
This model was trained using Edge Impulse with ~200 gesture samples. Preprocessing used Raw Accelerometer → Spectral Features (FFT).

## Author
Arthur Hon, UIUC ECE undergrad

## Links
- [Edge Impulse](https://www.edgeimpulse.com)
- [ESP32 BLE Keyboard Library](https://github.com/T-vK/ESP32-BLE-Keyboard)
- [Adafruit MPU6050](https://github.com/adafruit/Adafruit_MPU6050)

## Third-Party Libraries
This project makes use of the following open-source libraries:

- Wire (Arduino I2C Library) – Included with Arduino Core
- Adafruit MPU6050 – [BSD License](https://github.com/adafruit/Adafruit_MPU6050/blob/88b3f5983771ed6efc6c048b2c49c77ed1d417f2/license.txt)
- ESP32 BLE Keyboard 

## License
MIT License
