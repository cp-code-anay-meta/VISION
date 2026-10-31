# V.I.S.I.O.N.

V.I.S.I.O.N. is an ESP32-based obstacle detection system for a wearable haptic warning device. Five VL53L0X time-of-flight sensors scan five zones (FL / F / FR / L / R), and the ESP32 turns proximity and approach speed into vibration intensity for each zone.

> Prototype build shown below. Replace with your own photos before publishing.

![Prototype build](docs/images/breadboard-1.jpg)

## What it does

Each sensor measures distance to nearby objects. The ESP32 compares current and previous readings to estimate approach speed and combines distance and speed into a danger score for that zone. The resulting score sets the PWM duty cycle of the corresponding coin vibration motor.

This creates a haptic warning that gets stronger as an obstacle gets closer and/or approaches faster, similar to a "Spider-Sense" effect.

## Hardware

| Part | Qty | Notes |
|---|---:|---|
| ESP32 Dev V1 | 1 | Main controller |
| VL53L0X (6-pin, with XSHUT) | 5 | FL, F, FR, L, R |
| Coin vibration motor | 5 | One per zone |
| Breadboard + jumper wires | - | Prototype assembly |
| 3.3V power source | 1 | ESP32 and sensors |

See [hardware/pinout.md](hardware/pinout.md) for the prototype wiring layout.

## Prototype pin map

The current prototype uses the following mapping:

- I2C SDA: GPIO 21
- I2C SCL: GPIO 22
- Sensor XSHUT: GPIO 4, 5, 6, 7, 8
- Motor PWM: GPIO 25, 26, 27, 32, 33

> Update the pin assignments to match your final wearable build before production.

## How the danger score works

For each sensor zone:

1. Read the current VL53L0X distance.
2. Compare with the previous reading to estimate approach speed.
3. Combine distance and speed into a single danger score.
4. Convert the score to vibration strength.

The system is designed to be simple, responsive, and lightweight enough for real-time haptic feedback.

## Build and flash

This project is written in C++ for the ESP32 Arduino core.

### Required tooling

- Arduino IDE or VS Code + PlatformIO
- ESP32 board package
- `Wire` library
- `Adafruit_VL53L0X` library

### Flash procedure

1. Install the ESP32 board support package.
2. Open `src/main.cpp` in the Arduino IDE or PlatformIO.
3. Select the correct ESP32 board and serial port.
4. Compile and upload.
5. Power the board and test each sensor/motor pair with a nearby hand or object.

## Project status

Prototype on breadboard. Planned next steps:

- tune thresholds and sensitivity
- validate with realistic walking and wall scenarios
- finalize a wearable frame and enclosure
- move to an arm PCB or compact integrated board

## Known limitations

- Wall and corner reflections can trigger false positives.
- Walking motion can create vibration bursts or jitter.
- Sensor readings depend on reflective surfaces and ambient conditions.
- Comfort tuning and detection thresholds still need practical validation.

## Credits

Concept inspired by MateoTechLab's Spider-Sense glasses video.

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE).

## Repository structure

```text
VISION/
├── README.md
├── LICENSE
├── .gitignore
├── src/
│   └── main.cpp
├── hardware/
│   ├── pinout.md
│   └── parts-list.md
├── docs/
│   └── images/
│       └── breadboard-1.jpg
├── 3d-print/
│   └── README.md
└── .github/
    └── workflows/
```

The `3d-print/` folder is a placeholder for future CAD/STL or enclosure work.
