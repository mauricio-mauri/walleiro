#include <Arduino.h>
#include <NewPing.h>

// Diretivas de compilação para incluir a biblioteca WiFi correta dependendo do ambiente
#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

// ─── Sensor Ultrassônico ───────────────────────────────
#define TRIG_PIN     18
#define ECHO_PIN     19
#define MAX_DISTANCE 200

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

// ─── Driver L298N - Motor A (Esquerdo) ────────────────
#define IN1  25
#define IN2  26
#define ENA  32   // PWM

// ─── Driver L298N - Motor B (Direito) ─────────────────
#define IN3  27
#define IN4  14
#define ENB  33   // PWM

// ─── Parâmetros ───────────────────────────────────────
#define VELOCIDADE      180   // 0-255
#define DIST_MINIMA     15    // cm — obstáculo à frente
#define DIST_REFERENCIA 25    // cm — distância alvo da trajetória

// ─── Funções dos Motores ──────────────────────────────
void moverFrente() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, VELOCIDADE);
  analogWrite(ENB, VELOCIDADE);
}

void virarEsquerda() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, VELOCIDADE);
  analogWrite(ENB, VELOCIDADE);
}

void virarDireita() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
  analogWrite(ENA, VELOCIDADE);
  analogWrite(ENB, VELOCIDADE);
}

void parar() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

// ─── Setup ────────────────────────────────────────────
void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT); pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT); pinMode(ENB, OUTPUT);

  parar();
  Serial.println("Robô iniciado.");
}

// ─── Loop Principal ───────────────────────────────────
void loop() {
  unsigned int distancia = sonar.ping_cm();

  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");

  if (distancia == 0 || distancia > DIST_REFERENCIA + 10) {
    // Sem leitura ou muito longe: avança
    moverFrente();
  } else if (distancia < DIST_MINIMA) {
    // Obstáculo próximo: para e vira
    parar();
    delay(300);
    virarEsquerda();
    delay(400);
  } else {
    // Na faixa ideal: segue em frente
    moverFrente();
  }

  delay(50);
}