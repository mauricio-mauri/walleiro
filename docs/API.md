# Referência da API

## `include/pins.h`

Define os pinos físicos para cada placa. Inclua com:

```cpp
#include <pins.h>
```

### Constantes (ESP32 — Wokwi)

| Constante | GPIO | Função |
|-----------|------|--------|
| `TRIG_PIN` | 18 | Trigger do HC-SR04 |
| `ECHO_PIN` | 19 | Echo do HC-SR04 |
| `MAX_DISTANCE` | — | Alcance máximo do sensor (200 cm) |
| `AIN1` | 25 | Motor A — direção (TB6612) |
| `AIN2` | 26 | Motor A — direção (TB6612) |
| `PWMA` | 32 | Motor A — PWM (TB6612) |
| `BIN1` | 27 | Motor B — direção (TB6612) |
| `BIN2` | 14 | Motor B — direção (TB6612) |
| `PWMB` | 33 | Motor B — PWM (TB6612) |

### Constantes (ESP8266 — NodeMCU v3)

| Constante | GPIO | Silkscreen |
|-----------|------|------------|
| `TRIG_PIN` | 5 | D1 |
| `ECHO_PIN` | 4 | D2 |
| `MAX_DISTANCE` | — | 200 cm |
| `AIN1` | 14 | D5 |
| `AIN2` | 12 | D6 |
| `PWMA` | 13 | D7 (PWM) |
| `BIN1` | 0 | D3 |
| `BIN2` | 2 | D4 |
| `PWMB` | 15 | D8 (PWM) |
| `IR_PIN` | 16 | D0 (comentado — ativar quando ligar) |

> `IR_PIN` está comentado por padrão. Descomente em `pins.h` e
> passe `IR_PIN` em vez de `-1` no construtor do `Robot` quando
> o sensor HW-201 estiver conectado.

---

## `MotorController`

Abstração do driver de motor **TB6612FNG** (compatível com L298N)
para dois motores DC.

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

| Parâmetro | Pino TB6612 | Função |
|-----------|-------------|--------|
| `in1` | AIN1 | Controle de direção do motor A |
| `in2` | AIN2 | Controle de direção do motor A |
| `ena` | PWMA | PWM de velocidade do motor A |
| `in3` | BIN1 | Controle de direção do motor B |
| `in4` | BIN2 | Controle de direção do motor B |
| `enb` | PWMB | PWM de velocidade do motor B |

**Exemplo:**

```cpp
MotorController motors(AIN1, AIN2, PWMA, BIN1, BIN2, PWMB);
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

Ambos os motores giram para frente na velocidade atual (`AIN1=HIGH,
AIN2=LOW, BIN1=HIGH, BIN2=LOW`).

### `backward()`

```cpp
void backward();
```

Ambos os motores giram para trás na velocidade atual (`AIN1=LOW,
AIN2=HIGH, BIN1=LOW, BIN2=HIGH`).

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

Coordena os módulos de motor, sensor ultrassônico e sensor IR
para executar o comportamento do robô seguidor de trajetória
com desvio de obstáculos.

### includes

```cpp
#include <Robot.h>
```

### Construtor

```cpp
Robot(MotorController& motors, UltrasonicSensor& sensor, int irPin = -1);
```

Recebe **referências** para o controlador de motores e o sensor.
O parâmetro `irPin` indica o pino do sensor HW-201; passe `-1`
para desativar (padrão).

**Exemplo:**

```cpp
MotorController motors(AIN1, AIN2, PWMA, BIN1, BIN2, PWMB);
UltrasonicSensor sensor(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
Robot robot(motors, sensor, -1);          // sem IR
// Robot robot(motors, sensor, IR_PIN);   // com IR
```

### `setup()`

```cpp
void setup();
```

Inicializa o robô:

1. `_motors.begin()`
2. `_motors.setSpeed(VELOCIDADE)` — padrão 180
3. `_motors.stop()`
4. Se `_irPin >= 0`: `pinMode(_irPin, INPUT)`
5. Imprime `Robô iniciado.` no log

> O `Serial.begin()` é chamado pelo `DebugLog::begin()` em `main.cpp`,
> não pelo `Robot::setup()`.

### `update()`

```cpp
void update();
```

Executa um ciclo completo do comportamento. Deve ser chamado dentro
de `loop()`. A cada chamada:

1. Se IR ativado e obstáculo detectado → `stop()`, pausa 300 ms,
   `turnLeft()`, espera 400 ms, retorna
2. Lê a distância do sensor ultrassônico
3. Imprime no log
4. Decide a ação:
   - **distância == 0 ou > 35 cm** → `forward()`
   - **distância < 15 cm** → `stop()`, pausa 300 ms, `turnLeft()`, espera 400 ms
   - **15–35 cm** → `forward()`
5. Aguarda 50 ms

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

## `DebugLog`

Substitui o `Serial.print()` convencional por uma saída que vai
simultaneamente para **Serial USB** e para uma **página web**
servida via WiFi (Access Point).

Só compilada para ESP8266. No ESP32 (Wokwi) usa `Serial` direto.

### includes

```cpp
#include <DebugLog.h>
```

### Instância global

```cpp
extern DebugLog Debug;
```

Declarada em `DebugLog.cpp` — use `Debug` diretamente em qualquer
arquivo que inclua o header.

### `begin()`

```cpp
void begin(const char* ssid = "Walleiro", const char* pass = "12345678");
```

Inicializa:

1. `Serial.begin(115200)`
2. WiFi em modo **Access Point** com o SSID e senha fornecidos
3. Servidor HTTP na porta 80

Chame dentro de `setup()`.

**Parâmetros:**

| Parâmetro | Padrão | Descrição |
|-----------|--------|-----------|
| `ssid` | `"Walleiro"` | Nome da rede WiFi |
| `pass` | `"12345678"` | Senha da rede (mín. 8 caracteres) |

### `handleClient()`

```cpp
void handleClient();
```

Atende requisições HTTP. Deve ser chamado dentro de `loop()`.

### `write()`, `print()`, `println()`

```cpp
size_t write(uint8_t c);
```

Implementa `Print::write()` — toda chamada escreve no Serial USB
e no buffer circular de 50 linhas. Use `Debug.print()` e
`Debug.println()` exatamente como usaria `Serial.print()`.

### Página web

Com o robô alimentado pela power bank, conecte o celular/PC na
rede `Walleiro` (senha `12345678`) e acesse:

```
http://192.168.4.1/
```

A página exibe as últimas 50 linhas do log, atualizando a cada
1 segundo.

---

## Exemplo Completo

```cpp
#include <Arduino.h>
#include <pins.h>
#include <MotorController.h>
#include <UltrasonicSensor.h>
#include <Robot.h>

MotorController motors(AIN1, AIN2, PWMA, BIN1, BIN2, PWMB);
UltrasonicSensor sensor(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
Robot robot(motors, sensor, -1);

void setup() {
  Debug.begin();
  robot.setup();
}

void loop() {
  Debug.handleClient();
  robot.update();
}
```
