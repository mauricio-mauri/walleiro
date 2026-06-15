#pragma once

#include <Arduino.h>

/**
 * @file MotorController.h
 * @brief Abstração do driver de motor L298N (ponte H).
 *
 * Controla dois motores DC com direção e velocidade independentes
 * via 4 pinos de controle (IN1–IN4) e 2 pinos PWM (ENA, ENB).
 */

class MotorController {
public:
  /**
   * @param in1  Pino IN1 do L298N (motor A)
   * @param in2  Pino IN2 do L298N (motor A)
   * @param ena  Pino ENA do L298N (PWM — motor A)
   * @param in3  Pino IN3 do L298N (motor B)
   * @param in4  Pino IN4 do L298N (motor B)
   * @param enb  Pino ENB do L298N (PWM — motor B)
   */
  MotorController(uint8_t in1, uint8_t in2, uint8_t ena,
                  uint8_t in3, uint8_t in4, uint8_t enb);

  /// Configura todos os pinos como OUTPUT e para os motores.
  void begin();

  /// Ambos os motores giram para frente na velocidade atual.
  void forward();

  /// Ambos os motores giram para trás na velocidade atual.
  void backward();

  /// Gira apenas o motor da direita (vira à esquerda).
  void turnLeft();

  /// Gira apenas o motor da esquerda (vira à direita).
  void turnRight();

  /// Para ambos os motores (INx = LOW, PWM = 0).
  void stop();

  /// Ajusta a velocidade de ambos os motores (0–255).
  void setSpeed(uint8_t speed);

  /// Retorna a velocidade atual.
  uint8_t getSpeed() const;

private:
  uint8_t _in1, _in2, _ena;
  uint8_t _in3, _in4, _enb;
  uint8_t _speed;
};
