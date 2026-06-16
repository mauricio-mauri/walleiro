#pragma once

#include <Arduino.h>

/**
 * @file MotorController.h
 * @brief Abstração do driver de motor TB6612FNG / L298N (ponte H).
 *
 * Controla dois motores DC com direção e velocidade independentes
 * via 4 pinos de controle (AIN1–BIN2) e 2 pinos PWM (PWMA, PWMB).
 *
 * Compatível com TB6612FNG e L298N — a lógica de controle é idêntica.
 */

class MotorController {
public:
  /**
   * @param in1  Pino AIN1 / IN1 (motor A, direção)
   * @param in2  Pino AIN2 / IN2 (motor A, direção)
   * @param ena  Pino PWMA / ENA (motor A, PWM)
   * @param in3  Pino BIN1 / IN3 (motor B, direção)
   * @param in4  Pino BIN2 / IN4 (motor B, direção)
   * @param enb  Pino PWMB / ENB (motor B, PWM)
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
