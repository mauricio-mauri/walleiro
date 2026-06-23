#pragma once

#include <Arduino.h>
#include "MotorController.h"
#include "UltrasonicSensor.h"
#include "LineFollower.h"

/**
 * @file Robot.h
 * @brief Lógica de alto nível do robô seguidor de trajetória.
 *
 * Coordena os módulos de motor, sensor ultrassônico e sensor IR
 * para executar a trajetória e desviar de obstáculos.
 */

class Robot {
public:
  /**
   * @param motors      Referência para o controlador de motores
   * @param sensor      Referência para o sensor ultrassônico
   * @param lineSensor  Referência para o seguidor de linha (HW-201 via ADC)
   */
  Robot(MotorController& motors, UltrasonicSensor& sensor,
        LineFollower& lineSensor);

  /// Inicializa robô (motores parados).
  void setup();

  /**
   * @brief Executa um ciclo do comportamento do robô.
   *
   * 1. Se obstáculo a < 15cm → desvia
   * 2. Senão → lê o sensor de linha e corrige trajetória
   */
  void update();

private:
  MotorController& _motors;
  UltrasonicSensor& _sensor;
  LineFollower& _line;

  // Parâmetros de navegação
  static constexpr uint8_t VEL_BASE    = 180;
  static constexpr uint8_t VEL_CURVA   = 60;    // diferencial p/ correção
  static constexpr uint8_t DIST_MINIMA = 15;    // cm
};
