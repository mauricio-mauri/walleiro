#include "LineFollower.h"

LineFollower::LineFollower(uint8_t adcPin)
  : _pin(adcPin) {}

void LineFollower::begin() {
  // O ADC do ESP8266 não precisa de pinMode; é sempre entrada.
  // Mantido por consistência com a interface dos outros módulos.
}

int LineFollower::read() {
  int v = analogRead(_pin);

  if (v < TH_1) return 0;   // FORA DA LINHA
  if (v < TH_2) return 1;   // ESQUERDA
  if (v < TH_3) return 2;   // DIREITA
  return 3;                  // MEIO DA LINHA
}
