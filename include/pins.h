#pragma once

/*
 * Definições dos pinos para a placa ESP8266 (NodeMCU v3)
 *
 * Driver de motor: TB6612FNG (HW-166).
 * Convenção de nomenclatura: AIN1/AIN2/PWMA (motor A),
 * BIN1/BIN2/PWMB (motor B) — compatível com os parâmetros
 * do MotorController (in1, in2, ena, in3, in4, enb nessa ordem).
 */

#define TRIG_PIN      5   // D1
#define ECHO_PIN      4   // D2
#define MAX_DISTANCE 200

// TB6612FNG — Motor A
#define AIN1         14   // D5
#define AIN2         12   // D6
#define PWMA         13   // D7 (PWM)

// TB6612FNG — Motor B
// BIN1 (D3) e BIN2 (D4) têm pullup no boot = HIGH.
// Mas PWMB (D8) tem pulldown = LOW → motor B desligado.
#define BIN1          0   // D3
#define BIN2          2   // D4
#define PWMB         15   // D8 (PWM)

// Sensor infravermelho HW-201 (desativado por enquanto)
// #define IR_PIN    16   // D0

