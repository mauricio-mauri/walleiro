#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/**
 * @file DebugLog.h
 * @brief Log de depuração com WiFi AP e página web.
 *
 * Substitui Serial.print() por uma saída dupla:
 *  1. Serial USB (fio)
 *  2. Buffer circular de 50 linhas servido via HTTP em 192.168.4.1
 *
 * Uso:
 *   Debug.begin();
 *   Debug.println("Robô iniciado.");
 *   Debug.print("Distancia: ");
 *   Debug.println(42);
 */

class DebugLog : public Print {
public:
  /**
   * @brief Inicializa Serial USB, WiFi AP e servidor web.
   * @param ssid Nome da rede WiFi (padrão "Walleiro")
   * @param pass Senha da rede WiFi (padrão "12345678")
   */
  void begin(const char* ssid = "Walleiro", const char* pass = "12345678");

  /**
   * @brief Atende requisições HTTP.
   * Deve ser chamado dentro de loop().
   */
  void handleClient();

  /**
   * @brief Interface Print::write — envia para Serial e buffer.
   * @param c Caractere a escrever
   * @return 1
   */
  size_t write(uint8_t c) override;

private:
  static constexpr size_t MAX_LINES = 50;
  static constexpr size_t LINE_LEN  = 128;

  char _buffer[MAX_LINES][LINE_LEN];
  size_t _head  = 0;    // índice da próxima linha a escrever
  size_t _count = 0;    // total de linhas acumuladas

  ESP8266WebServer _server;

  /// Acumula caracteres até '\n', depois armazena no buffer circular.
  void push(char c);

  /// Monta o HTML da página de log.
  String renderPage();
};

/// Instância global única (declarada no .cpp)
extern DebugLog Debug;
