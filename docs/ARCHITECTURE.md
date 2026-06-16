# Arquitetura do Firmware

## Visão Geral

O firmware do Walleiro segue uma arquitetura em **camadas**. Cada camada
abstrai um nível de hardware ou lógica, e depende apenas da camada
imediatamente abaixo.

```
┌──────────────────────────────────────────────────┐
│                   main.cpp                       │
│   instancia os objetos e chama update()          │
├──────────────────────────────────────────────────┤
│                    Robot                         │
│   lógica de navegação (trajetória + desvio)      │
├───────────────────────┬──────────────────────────┤
│   MotorController     │   UltrasonicSensor       │
│   (TB6612FNG)         │   (HC-SR04 + NewPing)    │
├───────────────────────┴──────────────────────────┤
│                pins.h / DebugLog                 │
│   #ifdef ESP32 / #else (ESP8266)  +  WiFi AP    │
├──────────────────────────────────────────────────┤
│              Arduino Framework                   │
│   digitalWrite, analogWrite, Print, etc.         │
└──────────────────────────────────────────────────┘
```

**Regra de dependência:** as setas apontam **para cima** (quem usa quem):

- `Robot` usa `MotorController` e `UltrasonicSensor`
- `MotorController` e `UltrasonicSensor` usam `pins.h` e o framework Arduino
- `DebugLog` é usado por `Robot` (via macro `LOG`) e por `main.cpp`
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

### `lib/DebugLog`

Encapsula a saída de depuração com três destinos simultâneos:

1. **Serial USB** (fio, para desenvolvimento)
2. **Buffer circular** (últimas 50 linhas)
3. **Página web** servida via WiFi AP em `192.168.4.1`

Implementa a interface `Print` do Arduino — tem `print()`, `println()`
etc., iguais ao `Serial`. Toda chamada vai pros três destinos.

**Só compilado no ESP8266.** No ESP32 (Wokwi) usa `Serial` direto.

### `lib/MotorController`

Encapsula o driver TB6612FNG (ponte H MOSFET). Responsabilidades:

- Configurar os 6 pinos como OUTPUT
- Escrever os níveis lógicos em AIN1–BIN2 para cada estado do motor
- Aplicar PWM em PWMA/PWMB para controle de velocidade
- Manter o estado de velocidade atual

**Não sabe da existência de sensor, obstáculo ou trajetória.**
Compatível também com L298N (a lógica de controle é idêntica).

### `lib/UltrasonicSensor`

Encapsula o sensor HC-SR04 via biblioteca NewPing. Responsabilidades:

- Inicializar o sensor (nenhum setup necessário, o construtor faz tudo)
- Medir distância em centímetros
- Decidir se há obstáculo dentro de um limiar

**Não sabe da existência de motores ou controle.**

### `lib/Robot`

Orquestra os módulos anteriores. Responsabilidades:

- Inicializar o robô (motores parados, pinMode do IR se ativado)
- A cada ciclo: ler sensor IR (se ativado), ler ultrassom, decidir ação
- Manter as constantes de navegação (velocidade, distância mínima, etc.)

**É a única camada que contém lógica de negócio.** Se a regra de desvio
mudar, mexe-se só aqui.

### `src/main.cpp`

Ponto de entrada. Cria as instâncias globais e delega o trabalho:

```cpp
MotorController motors(AIN1, AIN2, PWMA, BIN1, BIN2, PWMB);
UltrasonicSensor sensor(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
Robot robot(motors, sensor, -1);

void setup() {
  Debug.begin();          // Serial + WiFi AP + servidor web
  robot.setup();
}

void loop() {
  Debug.handleClient();   // atende requisições HTTP
  robot.update();
}
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
    ├── Debug.begin()
    │     ├── Serial.begin(115200)
    │     ├── WiFi.softAP("Walleiro")
    │     └── servidor HTTP start
    ├── motors.begin()       ← pinMode + stop()
    ├── motors.setSpeed(180)
    └── motors.stop()
    │
    ▼ (loop infinito)
  update()
    ├── Debug.handleClient()
    ├── [IR ativado?] digitalRead(IR_PIN) == LOW?
    │     → stop + turnLeft + return
    ├── sensor.readDistance()
    ├── Debug.print(distancia)
    ├── decisão:
    │   ├── 0 ou > 35cm  → motors.forward()
    │   ├── < 15cm       → motors.stop() + delay + turnLeft() + delay
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
| Debug só por USB | Debug por USB + WiFi (web + celular) |
