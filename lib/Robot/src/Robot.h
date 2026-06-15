#pragma once

#include <Arduino.h>
#include "MotorController.h"
#include "UltrasonicSensor.h"

/**
 * @file Robot.h
 * @brief Lógica de alto nível do robô seguidor de trajetória.
 *
 * Coordena os módulos de motor e sensor para executar a
 * trajetória e desviar de obstáculos.
 */

class Robot {
public:
  /**
   * @param motors Referência para o controlador de motores
   * @param sensor Referência para o sensor ultrassônico
   */
  Robot(MotorController& motors, UltrasonicSensor& sensor);

  /// Inicializa robô (serial, delay inicial).
  void setup();

  /**
   * @brief Executa um ciclo do comportamento do robô.
   *
   * Lê o sensor, decide entre avançar, desviar ou parar.
   * Deve ser chamado dentro de loop().
   */
  void update();

private:
  MotorController& _motors;
  UltrasonicSensor& _sensor;

  // Parâmetros de navegação
  static constexpr uint8_t VELOCIDADE      = 180;
  static constexpr uint8_t DIST_MINIMA      = 15;   // cm
  static constexpr uint8_t DIST_REFERENCIA  = 25;   // cm
  static constexpr uint16_t TEMPO_CURVA     = 400;  // ms
  static constexpr uint16_t TEMPO_PAUSA     = 300;  // ms
};
