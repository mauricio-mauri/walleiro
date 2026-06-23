#include "Robot.h"
#include <DebugLog.h>

Robot::Robot(MotorController& motors, UltrasonicSensor& sensor,
             LineFollower& lineSensor, int servoPin)
  : _motors(motors), _sensor(sensor), _line(lineSensor),
    _servoPin(servoPin) {}

void Robot::setup() {
  _motors.begin();
  _motors.setSpeed(VEL_BASE);
  _motors.stop();

  if (_servoPin >= 0) {
    _servo.attach(_servoPin);
    _servo.write(ANGLE_CENTER);
    delay(500);
  }

  Debug.println("Robô iniciado.");
}

void Robot::update() {
  unsigned int distancia = _sensor.readDistance();

  // ─── Prioridade 1: obstáculo ───────────────────────────
  if (distancia > 0 && distancia < DIST_MINIMA) {
    Debug.println("Obstáculo! Escaneando...");
    _motors.stop();

    if (_servoPin >= 0) {
      // Mede lado esquerdo
      _servo.write(ANGLE_LEFT);
      delay(300);
      unsigned int distEsq = _sensor.readDistance();

      // Mede lado direito
      _servo.write(ANGLE_RIGHT);
      delay(300);
      unsigned int distDir = _sensor.readDistance();

      // Escolhe o lado com mais espaço
      Debug.print("Esq: ");
      Debug.print(distEsq);
      Debug.print("  Dir: ");
      Debug.println(distDir);

      if (distDir > distEsq) {
        _motors.setSpeed(VEL_BASE);
        _motors.turnRight();
      } else {
        _motors.setSpeed(VEL_BASE);
        _motors.turnLeft();
      }
      delay(TEMPO_CURVA);

      // Volta ao centro
      _servo.write(ANGLE_CENTER);
      delay(300);
    } else {
      // Sem servo: desvia no tempo fixo
      delay(TEMPO_PAUSA);
      _motors.setSpeed(VEL_BASE);
      _motors.turnLeft();
      delay(TEMPO_CURVA);
    }

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
