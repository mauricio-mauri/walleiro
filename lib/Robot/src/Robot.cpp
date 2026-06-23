#include "Robot.h"
#include <DebugLog.h>

Robot::Robot(MotorController& motors, UltrasonicSensor& sensor,
             LineFollower& lineSensor)
  : _motors(motors), _sensor(sensor), _line(lineSensor) {}

void Robot::setup() {
  _motors.begin();
  _motors.setSpeed(VEL_BASE);
  _motors.stop();
  Debug.println("Robô iniciado.");
}

void Robot::update() {
  unsigned int distancia = _sensor.readDistance();

  // ─── Prioridade 1: obstáculo ───────────────────────────
  if (distancia > 0 && distancia < DIST_MINIMA) {
    Debug.println("Obstáculo! Desviando...");
    _motors.stop();
    delay(300);
    _motors.setSpeed(VEL_BASE);
    _motors.turnLeft();
    delay(400);
    return;
  }

  // ─── Prioridade 2: seguir linha ─────────────────────────
  int estado = _line.read();

  Debug.print("Linha: ");
  Debug.println(estado);

  switch (estado) {
    case 0:  // FORA DA LINHA — segue reto tentando reencontrar
      _motors.setSpeed(VEL_BASE);
      _motors.forward();
      break;

    case 1:  // ESQUERDA — corrige para a esquerda (motor direito mais fraco)
      _motors.setSpeedA(VEL_BASE + VEL_CURVA);
      _motors.setSpeedB(VEL_BASE - VEL_CURVA);
      _motors.forward();
      break;

    case 2:  // DIREITA — corrige para a direita (motor esquerdo mais fraco)
      _motors.setSpeedA(VEL_BASE - VEL_CURVA);
      _motors.setSpeedB(VEL_BASE + VEL_CURVA);
      _motors.forward();
      break;

    case 3:  // MEIO DA LINHA — segue reto
      _motors.setSpeed(VEL_BASE);
      _motors.forward();
      break;
  }

  delay(50);
}
