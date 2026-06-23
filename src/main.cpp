#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DebugLog.h>

#include <pins.h>
#include <MotorController.h>
#include <UltrasonicSensor.h>
#include <LineFollower.h>
#include <Robot.h>

// ─── Instâncias Globais ────────────────────────────────
// MotorController(in1, in2, ena, in3, in4, enb)
MotorController motors(AIN1, AIN2, PWMA, BIN1, BIN2, PWMB);
UltrasonicSensor sensor(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
LineFollower lineSensor(LINE_SENSOR);
Robot robot(motors, sensor, lineSensor);

// ─── Setup ──────────────────────────────────────────────
void setup() {
  Debug.begin();
  lineSensor.begin();
  robot.setup();
}

// ─── Loop Principal ─────────────────────────────────────
void loop() {
  Debug.handleClient();
  robot.update();
}
