#pragma once

/*
 * Definições dos pinos para cada placa.
 * ESP32 → usado na simulação Wokwi
 * ESP8266 → placa física (NodeMCU v3)
 *
 * ATENÇÃO: os pinos do ESP8266 são provisórios — ajuste ao montar o
 * circuito real e verificar problemas de boot (GPIO0, GPIO2, GPIO15).
 */

#ifdef ESP32

  // ─── HC-SR04 ─────────────────────────────────────────
  #define TRIG_PIN     18
  #define ECHO_PIN     19
  #define MAX_DISTANCE 200    // cm

  // ─── L298N — Motor A (esquerdo) ──────────────────────
  #define IN1          25
  #define IN2          26
  #define ENA          32     // PWM

  // ─── L298N — Motor B (direito) ───────────────────────
  #define IN3          27
  #define IN4          14
  #define ENB          33     // PWM

#else
  // ─── ESP8266 (NodeMCU v3) ────────────────────────────
  // TODO: verificar pinagem ao montar o circuito físico

  #define TRIG_PIN     5      // D1
  #define ECHO_PIN     4      // D2
  #define MAX_DISTANCE 200    // cm

  #define IN1          14     // D5
  #define IN2          12     // D6
  #define ENA          13     // D7 (PWM)

  #define IN3          2      // D4
  #define IN4          15     // D8
  #define ENB          0      // D3 (PWM — verificar boot)

#endif
