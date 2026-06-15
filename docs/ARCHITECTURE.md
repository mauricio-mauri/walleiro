# Arquitetura do Firmware

## Visão Geral

O firmware do Walleiro segue uma arquitetura em **camadas**. Cada camada
abstrai um nível de hardware ou lógica, e depende apenas da camada
imediatamente abaixo.

```
┌─────────────────────────────────────────────┐
│                 main.cpp                    │
│   instancia os objetos e chama update()     │
├─────────────────────────────────────────────┤
│                  Robot                      │
│   lógica de navegação (trajetória + desvio) │
├──────────────────────┬──────────────────────┤
│   MotorController    │   UltrasonicSensor   │
│   (L298N)            │   (HC-SR04 + NewPing)│
├──────────────────────┴──────────────────────┤
│               pins.h                        │
│    #ifdef ESP32 / #else (ESP8266)           │
├─────────────────────────────────────────────┤
│          Arduino Framework                  │
│   digitalWrite, analogWrite, Serial, etc.   │
└─────────────────────────────────────────────┘
```

**Regra de dependência:** as setas apontam **para cima** (quem usa quem):

- `Robot` usa `MotorController` e `UltrasonicSensor`
- `MotorController` e `UltrasonicSensor` usam `pins.h` e o framework Arduino
- `main.cpp` usa todos os módulos

Nenhum módulo da camada superior é conhecido pelos módulos inferiores.

---

## Por que #ifdef ESP32?

O projeto compila para duas placas diferentes:

| Placa | Env PlatformIO | Uso |
|-------|----------------|-----|
| ESP32 DevKit v4 | `esp32_sim` | Simulação Wokwi |
| NodeMCU v3 (ESP8266) | `nodemcuv2` | Placa física |

Os **GPIOs são diferentes** entre as duas, e as bibliotecas de WiFi também
(`WiFi.h` no ESP32, `ESP8266WiFi.h` no ESP8266).

A estratégia adotada foi:

1. **Centralizar as diferenças em `pins.h`** — os `#define` de cada GPIO
   ficam num único arquivo, com `#ifdef ESP32`.
2. **O resto do código não sabe qual placa é.** `MotorController`,
   `UltrasonicSensor` e `Robot` recebem os valores de `pins.h` e ponto final.
3. **`main.cpp` inclui a WiFi certa** antes dos módulos, pro caso de
   precisarmos de conectividade no futuro.

Se amanhã quisermos suportar ESP32-C3 ou outra placa, é só adicionar
mais um `#elif` em `pins.h`.

---

## Módulos

### `lib/MotorController`

Encapsula o driver L298N. Responsabilidades:

- Configurar os 6 pinos como OUTPUT
- Escrever os níveis lógicos em IN1–IN4 para cada estado do motor
- Aplicar PWM em ENA/ENB para controle de velocidade
- Manter o estado de velocidade atual

**Não sabe da existência de sensor, obstáculo ou trajetória.**

### `lib/UltrasonicSensor`

Encapsula o sensor HC-SR04 via biblioteca NewPing. Responsabilidades:

- Inicializar o sensor (nenhum setup necessário, o construtor faz tudo)
- Medir distância em centímetros
- Decidir se há obstáculo dentro de um limiar

**Não sabe da existência de motores ou controle.**

### `lib/Robot'

Orquestra os dois módulos anteriores. Responsabilidades:

- Inicializar o robô (serial, motores parados)
- A cada ciclo: ler o sensor, decidir ação (frente/curva/parar), executar
- Manter as constantes de navegação (velocidade, distância mínima, etc.)

**É a única camada que contém lógica de negócio.** Se a regra de desvio
mudar, mexe-se só aqui.

### `src/main.cpp`

Ponto de entrada. Cria as instâncias globais e delega o trabalho:

```cpp
MotorController motors(IN1, IN2, ENA, IN3, IN4, ENB);
UltrasonicSensor sensor(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
Robot robot(motors, sensor);

void setup() { robot.setup(); }
void loop()  { robot.update(); }
```

Mantido propositalmente enxuto — qualquer leitor entende o que o robô
faz em segundos.

---

## Fluxo de Execução

```
[Power ON]
    │
    ▼
  setup()
    ├── Serial.begin(115200)
    ├── motors.begin()       ← pinMode + stop()
    ├── motors.setSpeed(180)
    └── motors.stop()
    │
    ▼ (loop infinito)
  update()
    ├── sensor.readDistance()
    ├── Serial.print(distancia)
    ├── decisão:
    │   ├── 0 ou > 35cm  → motors.forward()
    │   ├── < 15cm       → motors.stop() + delay + motors.turnLeft() + delay
    │   └── entre 15-35  → motors.forward()
    └── delay(50)
```

---

## Ganhos da Arquitetura

| Antes (monolítico) | Depois (modular) |
|---|---|
| 96 linhas em `main.cpp` | 22 linhas em `main.cpp` |
| Lógica e hardware misturados | Cada módulo tem uma responsabilidade |
| Trocar de motor driver = reescrever tudo | Trocar de motor driver = só `MotorController` |
| Difícil testar isoladamente | Cada módulo pode ser testado separado |
| Time pisava no código do outro | Cada membro pode trabalhar num módulo |
