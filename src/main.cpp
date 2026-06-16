#include <Arduino.h>

// Incluir WiFi antes dos módulos do robô (necessário para compilação)
#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

#include <pins.h>
#include <MotorController.h>
#include <UltrasonicSensor.h>
#include <Robot.h>

// ─── Instâncias Globais ────────────────────────────────
// Ordem do construtor MotorController: (in1, in2, ena, in3, in4, enb)
MotorController motors(AIN1, AIN2, PWMA, BIN1, BIN2, PWMB);
UltrasonicSensor sensor(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

#ifdef ESP32
  Robot robot(motors, sensor);
#else
  Robot robot(motors, sensor, IR_PIN);
#endif

// ─── Setup ──────────────────────────────────────────────
void setup() {
  robot.setup();
}

// ─── Loop Principal ─────────────────────────────────────
void loop() {
  robot.update();
}
