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
MotorController motors(IN1, IN2, ENA, IN3, IN4, ENB);
UltrasonicSensor sensor(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
Robot robot(motors, sensor);

// ─── Setup ──────────────────────────────────────────────
void setup() {
  robot.setup();
}

// ─── Loop Principal ─────────────────────────────────────
void loop() {
  robot.update();
}
