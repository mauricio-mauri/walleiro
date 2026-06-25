#pragma once

#include <Arduino.h>

/**
 * @file LineFollower.h
 * @brief Leitura de 2 sensores IR (HW-201) via resistor ladder no ADC.
 *
 * Converte a tensão do pino A0 em 4 estados:
 *   0 = FORA DA LINHA  (ambos no branco)
 *   1 = ESQUERDA       (sensor direito na linha)
 *   2 = DIREITA         (sensor esquerdo na linha)
 *   3 = MEIO DA LINHA  (ambos na linha)
 *
 * Circuito:
 *   IR_L -- 10kΩ --+-- A0
 *   IR_R -- 20kΩ --+
 *   GND  -- 10kΩ --+
 */

class LineFollower {
public:
  /**
   * @param adcPin Pino ADC conectado ao resistor ladder (padrão A0)
   */
  LineFollower(uint8_t adcPin = A0);

  /// Configura o pino ADC como entrada.
  void begin();

  /**
   * @brief Lê o valor do ADC e retorna o estado.
   * @return 0 (FORA), 1 (ESQUERDA), 2 (DIREITA) ou 3 (MEIO)
   */
  int read();

private:
  uint8_t _pin;

  // Thresholds entre os níveis de tensão (10-bit ADC, 0-1023)
  static constexpr int TH_1 = 170;   // entre 0 e 1 (~341 analogRead)
  static constexpr int TH_2 = 420;   // entre 1 e 2 (~512 analogRead)
  static constexpr int TH_3 = 560;   // entre 2 e 3 (~614 analogRead)
};
