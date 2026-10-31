#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_VL53L0X.h>

// Prototype pin mapping.
const uint8_t SENSOR_XSHUT_PINS[5] = {4, 5, 6, 7, 8};
const uint8_t SENSOR_I2C_ADDRS[5] = {0x30, 0x31, 0x32, 0x33, 0x34};
const uint8_t MOTOR_PINS[5] = {25, 26, 27, 32, 33};

Adafruit_VL53L0X sensors[5];
float previousDistance[5] = {0.0f};
float previousTime[5] = {0.0f};

void setMotor(uint8_t zone, uint8_t strength) {
  analogWrite(MOTOR_PINS[zone], strength);
}

void setup() {
  Serial.begin(115200);
  Wire.begin();

  for (uint8_t i = 0; i < 5; i++) {
    pinMode(SENSOR_XSHUT_PINS[i], OUTPUT);
    digitalWrite(SENSOR_XSHUT_PINS[i], LOW);
  }

  delay(50);

  for (uint8_t i = 0; i < 5; i++) {
    digitalWrite(SENSOR_XSHUT_PINS[i], HIGH);
    delay(10);

    sensors[i].setAddress(SENSOR_I2C_ADDRS[i]);
    if (!sensors[i].begin()) {
      Serial.printf("VL53L0X %d init failed\n", i);
      while (1) {
        delay(10);
      }
    }

    sensors[i].startRangeContinuous();
    previousDistance[i] = 0.0f;
    previousTime[i] = millis() / 1000.0f;
    setMotor(i, 0);
  }
}

float computeDangerScore(uint8_t zone, float distanceMm, float now) {
  if (distanceMm <= 0.0f) {
    return 0.0f;
  }

  if (previousDistance[zone] <= 0.0f) {
    previousDistance[zone] = distanceMm;
    previousTime[zone] = now;
    return 0.0f;
  }

  float dt = max(0.05f, now - previousTime[zone]);
  float approachSpeed = (previousDistance[zone] - distanceMm) / dt;

  previousDistance[zone] = distanceMm;
  previousTime[zone] = now;

  float proximityWeight = 1.0f / max(50.0f, distanceMm);
  float speedWeight = max(0.0f, approachSpeed) / 500.0f;

  float danger = (proximityWeight * 1000.0f) + (speedWeight * 1000.0f);
  return constrain(danger, 0.0f, 255.0f);
}

void loop() {
  float now = millis() / 1000.0f;

  for (uint8_t zone = 0; zone < 5; zone++) {
    VL53L0X_RangingMeasurementData_t data;
    sensors[zone].getSingleRangingMeasurement(&data, false);

    uint16_t distanceMm = data.RangeMilliMeter;
    float danger = computeDangerScore(zone, (float)distanceMm, now);
    uint8_t strength = (uint8_t)constrain(danger, 0.0f, 255.0f);

    setMotor(zone, strength);
    Serial.printf("Zone %d distance=%d mm strength=%d\n", zone, distanceMm, strength);
  }

  delay(50);
}
