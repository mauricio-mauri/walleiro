#include "Robot.h"

Robot::Robot(MotorController& motors, UltrasonicSensor& sensor)
  : _motors(motors), _sensor(sensor) {}

void Robot::setup() {
  Serial.begin(115200);
  _motors.begin();
  _motors.setSpeed(VELOCIDADE);
  _motors.stop();
  Serial.println("Robô iniciado.");
}

void Robot::update() {
  unsigned int distancia = _sensor.readDistance();

  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");

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
