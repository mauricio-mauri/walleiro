#pragma once

#include <Arduino.h>
#include "MotorController.h"
#include "UltrasonicSensor.h"

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
   * @param motors Referência para o controlador de motores
   * @param sensor Referência para o sensor ultrassônico
   * @param irPin  Pino do sensor IR (HW-201), ou -1 se não usado
   */
  Robot(MotorController& motors, UltrasonicSensor& sensor, int irPin = -1);

  /// Inicializa robô (serial, delay inicial, pinMode do IR).
  void setup();

  /**
   * @brief Executa um ciclo do comportamento do robô.
   *
   * Lê o sensor IR primeiro (obstáculo imediato), depois o
   * sensor ultrassônico, e decide entre avançar, desviar ou parar.
   */
  void update();

private:
  MotorController& _motors;
  UltrasonicSensor& _sensor;
  int _irPin;

  // Parâmetros de navegação
  static constexpr uint8_t VELOCIDADE      = 180;
  static constexpr uint8_t DIST_MINIMA      = 15;   // cm
  static constexpr uint8_t DIST_REFERENCIA  = 25;   // cm
  static constexpr uint16_t TEMPO_CURVA     = 400;  // ms
  static constexpr uint16_t TEMPO_PAUSA     = 300;  // ms
};
