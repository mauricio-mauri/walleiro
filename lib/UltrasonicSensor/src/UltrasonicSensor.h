#pragma once

#include <Arduino.h>
#include <NewPing.h>

/**
 * @file UltrasonicSensor.h
 * @brief Abstração do sensor ultrassônico HC-SR04.
 *
 * Encapsula a biblioteca NewPing e oferece métodos de leitura
 * de distância e detecção de obstáculos.
 */

class UltrasonicSensor {
public:
  /**
   * @param trig        Pino TRIG do HC-SR04
   * @param echo        Pino ECHO do HC-SR04
   * @param maxDistance Distância máxima em cm (padrão: 200)
   */
  UltrasonicSensor(uint8_t trig, uint8_t echo, unsigned int maxDistance = 200);

  /// Retorna a distância medida em centímetros (0 = sem eco).
  unsigned int readDistance();

  /**
   * @brief Verifica se há obstáculo dentro do limiar.
   * @param threshold Distância em cm (padrão: 15)
   * @return true se a distância lida for > 0 e <= threshold
   */
  bool obstacleDetected(unsigned int threshold = 15);

private:
  NewPing _sonar;
};
