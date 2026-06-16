#include "Robot.h"

#ifdef ESP8266
  #include <DebugLog.h>
  #define LOG Debug
#else
  #define LOG Serial
#endif

Robot::Robot(MotorController& motors, UltrasonicSensor& sensor, int irPin)
  : _motors(motors), _sensor(sensor), _irPin(irPin) {}

void Robot::setup() {
  _motors.begin();
  _motors.setSpeed(VELOCIDADE);
  _motors.stop();
  if (_irPin >= 0) {
    pinMode(_irPin, INPUT);
  }
  LOG.println("Robô iniciado.");
}

void Robot::update() {
  // ─── Sensor IR (obstáculo imediato) ───────────────
  if (_irPin >= 0 && digitalRead(_irPin) == LOW) {
    LOG.println("IR: obstáculo detectado!");
    _motors.stop();
    delay(TEMPO_PAUSA);
    _motors.turnLeft();
    delay(TEMPO_CURVA);
    return;
  }

  // ─── Sensor ultrassônico ─────────────────────────
  unsigned int distancia = _sensor.readDistance();

  LOG.print("Distancia: ");
  LOG.print(distancia);
  LOG.println(" cm");

  if (distancia == 0 || distancia > DIST_REFERENCIA + 10) {
    // Sem leitura ou muito longe: avança
    _motors.forward();
  } else if (distancia < DIST_MINIMA) {
    // Obstáculo próximo: para e vira
    _motors.stop();
    delay(TEMPO_PAUSA);
    _motors.turnLeft();
    delay(TEMPO_CURVA);
  } else {
    // Na faixa ideal: segue em frente
    _motors.forward();
  }

  delay(50);
}
