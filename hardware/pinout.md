# V.I.S.I.O.N. pinout

This file documents the prototype wiring used during the breadboard build. Update it to match your final wearable design as needed.

## Shared I2C bus

| Signal | ESP32 GPIO |
|---|---:|
| SDA | 21 |
| SCL | 22 |
| 3V3 | 3V3 |
| GND | GND |

## Sensor wiring

| Zone | Sensor XSHUT | Motor PWM |
|---|---:|---:|
| FL | GPIO 4 | GPIO 25 |
| F | GPIO 5 | GPIO 26 |
| FR | GPIO 6 | GPIO 27 |
| L | GPIO 7 | GPIO 32 |
| R | GPIO 8 | GPIO 33 |

## Notes

- All five sensors share the same I2C bus.
- Each sensor uses `XSHUT` to be enabled one at a time during startup.
- The motors are driven with PWM for variable haptic intensity.
- This is a prototype layout; the final wearable may use a different pin map.
