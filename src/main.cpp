#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DebugLog.h>

#include <pins.h>
#include <MotorController.h>
#include <UltrasonicSensor.h>
#include <Robot.h>

// ─── Instâncias Globais ────────────────────────────────
// Ordem do construtor MotorController: (in1, in2, ena, in3, in4, enb)
MotorController motors(AIN1, AIN2, PWMA, BIN1, BIN2, PWMB);
UltrasonicSensor sensor(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

// IR sensor desativado — passar -1 como irPin
Robot robot(motors, sensor, -1);

// ─── Setup ──────────────────────────────────────────────
void setup() {
  Debug.begin();
  robot.setup();
}

// ─── Loop Principal ─────────────────────────────────────
void loop() {
  Debug.handleClient();
  robot.update();
}
