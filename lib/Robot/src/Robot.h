#pragma once

#include <Arduino.h>
#include <Servo.h>
#include "MotorController.h"
#include "UltrasonicSensor.h"
#include "LineFollower.h"

/**
 * @file Robot.h
 * @brief Lógica de alto nível do robô seguidor de trajetória.
 *
 * Coordena os módulos de motor, sensor ultrassônico, seguidor de linha
 * e servo para executar a trajetória e desviar de obstáculos.
 */

class Robot {
public:
  /**
   * @param motors      Referência para o controlador de motores
   * @param sensor      Referência para o sensor ultrassônico
   * @param lineSensor  Referência para o seguidor de linha (HW-201 via ADC)
   * @param servoPin    Pino do servo motor, ou -1 se não usado
   */
  Robot(MotorController& motors, UltrasonicSensor& sensor,
        LineFollower& lineSensor, int servoPin = -1);

  /// Inicializa robô (motores parados, servo centrado).
  void setup();

  /**
   * @brief Executa um ciclo do comportamento do robô.
   *
   * 1. Se obstáculo a < 15cm → servo escaneia L/R, desvia
   * 2. Senão → lê o sensor de linha e corrige trajetória
   */
  void update();

private:
  MotorController& _motors;
  UltrasonicSensor& _sensor;
  LineFollower& _line;
  Servo _servo;
  int _servoPin;

  // Parâmetros de navegação
  static constexpr uint8_t VEL_BASE      = 180;
  static constexpr uint8_t VEL_CURVA     = 60;    // diferencial p/ correção
  static constexpr uint8_t DIST_MINIMA   = 15;    // cm
  static constexpr unsigned int TEMPO_PAUSA  = 300;  // ms
  static constexpr unsigned int TEMPO_CURVA  = 400;  // ms
  static constexpr int ANGLE_CENTER  = 90;
  static constexpr int ANGLE_LEFT    = 0;
  static constexpr int ANGLE_RIGHT   = 180;
};
