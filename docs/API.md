# Referência da API

## `include/pins.h`

Define os pinos físicos para cada placa. Inclua com:

```cpp
#include <pins.h>
```

### Constantes (ESP32)

| Constante | GPIO | Função |
|-----------|------|--------|
| `TRIG_PIN` | 18 | Trigger do HC-SR04 |
| `ECHO_PIN` | 19 | Echo do HC-SR04 |
| `MAX_DISTANCE` | — | Alcance máximo do sensor (200 cm) |
| `IN1` | 25 | Motor A — Entrada 1 do L298N |
| `IN2` | 26 | Motor A — Entrada 2 do L298N |
| `ENA` | 32 | Motor A — Enable/PWM |
| `IN3` | 27 | Motor B — Entrada 3 do L298N |
| `IN4` | 14 | Motor B — Entrada 4 do L298N |
| `ENB` | 33 | Motor B — Enable/PWM |

### Constantes (ESP8266 — provisório)

Os valores abaixo são **provisórios** e devem ser ajustados ao montar
o circuito físico. Veja [`HARDWARE.md`](HARDWARE.md) quando disponível.

| Constante | GPIO | Silkscreen |
|-----------|------|------------|
| `TRIG_PIN` | 5 | D1 |
| `ECHO_PIN` | 4 | D2 |
| `MAX_DISTANCE` | — | 200 cm |
| `IN1` | 14 | D5 |
| `IN2` | 12 | D6 |
| `ENA` | 13 | D7 |
| `IN3` | 2 | D4 |
| `IN4` | 15 | D8 |
| `ENB` | 0 | D3 |

---

## `MotorController`

Abstração do driver de motor L298N para dois motores DC.

### includes

```cpp
#include <MotorController.h>
```

### Construtor

```cpp
MotorController(uint8_t in1, uint8_t in2, uint8_t ena,
                uint8_t in3, uint8_t in4, uint8_t enb);
```

Cria o controlador. **Não mexe no hardware** — chame `begin()` depois.

**Parâmetros:**

| Parâmetro | Pino no L298N | Função |
|-----------|---------------|--------|
| `in1` | IN1 | Controle de direção do motor A |
| `in2` | IN2 | Controle de direção do motor A |
| `ena` | ENA | PWM de velocidade do motor A |
| `in3` | IN3 | Controle de direção do motor B |
| `in4` | IN4 | Controle de direção do motor B |
| `enb` | ENB | PWM de velocidade do motor B |

**Exemplo:**

```cpp
MotorController motors(IN1, IN2, ENA, IN3, IN4, ENB);
```

### `begin()`

```cpp
void begin();
```

Configura os 6 pinos como `OUTPUT` e para os motores (`stop()`).
Deve ser chamado dentro de `setup()`.

### `forward()`

```cpp
void forward();
```

Ambos os motores giram para frente na velocidade atual (`IN1=HIGH,
IN2=LOW, IN3=HIGH, IN4=LOW`).

### `backward()`

```cpp
void backward();
```

Ambos os motores giram para trás na velocidade atual (`IN1=LOW,
IN2=HIGH, IN3=LOW, IN4=HIGH`).

### `turnLeft()`

```cpp
void turnLeft();
```

Gira no lugar para a esquerda: motor A (esquerdo) vai para trás,
motor B (direito) vai para frente.

### `turnRight()`

```cpp
void turnRight();
```

Gira no lugar para a direita: motor A (esquerdo) vai para frente,
motor B (direito) vai para trás.

### `stop()`

```cpp
void stop();
```

Para ambos os motores: todos os INx em LOW, PWM em 0.

### `setSpeed()`

```cpp
void setSpeed(uint8_t speed);
```

Ajusta a velocidade de ambos os motores. O valor deve estar entre
0 (parado) e 255 (máximo).

**Valor padrão:** 180 (definido no construtor).

**Exemplo:**

```cpp
motors.setSpeed(200);  // mais rápido
motors.forward();      // aplica a nova velocidade
```

### `getSpeed()`

```cpp
uint8_t getSpeed() const;
```

Retorna a velocidade atual (0–255).

---

## `UltrasonicSensor`

Abstração do sensor de distância HC-SR04, usando a biblioteca NewPing.

### includes

```cpp
#include <UltrasonicSensor.h>
```

### Construtor

```cpp
UltrasonicSensor(uint8_t trig, uint8_t echo, unsigned int maxDistance = 200);
```

**Parâmetros:**

| Parâmetro | Descrição | Padrão |
|-----------|-----------|--------|
| `trig` | Pino TRIG do HC-SR04 | — |
| `echo` | Pino ECHO do HC-SR04 | — |
| `maxDistance` | Distância máxima em cm | 200 |

**Exemplo:**

```cpp
UltrasonicSensor sensor(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
```

### `readDistance()`

```cpp
unsigned int readDistance();
```

Dispara um pulso ultrassônico e retorna a distância medida em
centímetros. Retorna `0` se não houver eco (objeto fora do alcance
ou sensor desconectado).

**Tempo de execução:** aproximadamente 30 µs por cm de `maxDistance`
(para 200 cm ≈ 6 ms).

### `obstacleDetected()`

```cpp
bool obstacleDetected(unsigned int threshold = 15);
```

Retorna `true` se a distância lida for maior que 0 e menor ou igual
ao `threshold`. Atalho para:

```cpp
unsigned int d = sensor.readDistance();
return (d > 0 && d <= threshold);
```

**Exemplo:**

```cpp
if (sensor.obstacleDetected(20)) {
  Serial.println("Obstáculo a menos de 20 cm!");
}
```

---

## `Robot`

Coordena os módulos de motor e sensor para executar o comportamento
do robô seguidor de trajetória com desvio de obstáculos.

### includes

```cpp
#include <Robot.h>
```

### Construtor

```cpp
Robot(MotorController& motors, UltrasonicSensor& sensor);
```

Recebe **referências** para o controlador de motores e o sensor.
O `Robot` não é dono desses objetos — eles são criados em `main.cpp`
e passados ao robô.

**Exemplo:**

```cpp
MotorController motors(IN1, IN2, ENA, IN3, IN4, ENB);
UltrasonicSensor sensor(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
Robot robot(motors, sensor);
```

### `setup()`

```cpp
void setup();
```

Inicializa o robô:

1. `Serial.begin(115200)`
2. `_motors.begin()`
3. `_motors.setSpeed(VELOCIDADE)` — padrão 180
4. `_motors.stop()`
5. Imprime `Robô iniciado.` no serial

### `update()`

```cpp
void update();
```

Executa um ciclo completo do comportamento. Deve ser chamado dentro
de `loop()`. A cada chamada:

1. Lê a distância do sensor
2. Imprime no serial
3. Decide a ação:
   - **distância == 0 ou > 35 cm** → `forward()`
   - **distância < 15 cm** → `stop()`, pausa 300 ms, `turnLeft()`, espera 400 ms
   - **15–35 cm** → `forward()`
4. Aguarda 50 ms

### Constantes internas (ajustáveis)

| Constante | Valor | Efeito |
|-----------|-------|--------|
| `VELOCIDADE` | 180 | Velocidade padrão dos motores (0–255) |
| `DIST_MINIMA` | 15 cm | Distância que dispara o desvio |
| `DIST_REFERENCIA` | 25 cm | Distância alvo para seguir em frente |
| `TEMPO_CURVA` | 400 ms | Duração da curva de desvio |
| `TEMPO_PAUSA` | 300 ms | Pausa antes de virar |

Para alterar, edite `lib/Robot/src/Robot.h`.

---

## Exemplo Completo

```cpp
#include <Arduino.h>
#include <pins.h>
#include <MotorController.h>
#include <UltrasonicSensor.h>
#include <Robot.h>

MotorController motors(IN1, IN2, ENA, IN3, IN4, ENB);
UltrasonicSensor sensor(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
Robot robot(motors, sensor);

void setup() {
  robot.setup();
}

void loop() {
  robot.update();
}
```
