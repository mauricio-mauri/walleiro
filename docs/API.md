# Referência da API

## `include/pins.h`

Define os pinos físicos para cada placa. Inclua com:

```cpp
#include <pins.h>
```

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
| `LINE_SENSOR` | A0 | ADC — HW-201 via resistor ladder |

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

Ambos os motores giram para frente (`AIN1=HIGH, AIN2=LOW, BIN1=HIGH,
BIN2=LOW`). Cada motor usa sua velocidade individual (`_speedA` e `_speedB`).

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

### `setSpeedA()` / `setSpeedB()`

```cpp
void setSpeedA(uint8_t speed);
void setSpeedB(uint8_t speed);
```

Ajusta a velocidade de cada motor **individualmente**. Útil para
correções suaves de trajetória (diferencial de velocidade).

**Exemplo:**
```cpp
motors.setSpeedA(200);   // motor esquerdo mais rápido
motors.setSpeedB(150);   // motor direito mais lento
motors.forward();         // curva para a direita
```

### `getSpeedA()` / `getSpeedB()`

```cpp
uint8_t getSpeedA() const;
uint8_t getSpeedB() const;
```

Retorna a velocidade individual de cada motor (0–255).

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

## `LineFollower`

Leitura de 2 sensores IR (HW-201) conectados a um **resistor ladder**
no pino ADC do ESP8266. Converte a tensão analógica em 4 estados
discretos.

```
IR_L ── 10kΩ ──┐
                ├── A0
IR_R ── 20kΩ ──┘
GND  ── 10kΩ
```

### includes

```cpp
#include <LineFollower.h>
```

### Construtor

```cpp
LineFollower(uint8_t adcPin = A0);
```

| Parâmetro | Padrão | Descrição |
|-----------|--------|-----------|
| `adcPin` | A0 | Pino ADC conectado ao resistor ladder |

### `begin()`

```cpp
void begin();
```

Inicializa o sensor (consistência com os demais módulos).

### `read()`

```cpp
int read();
```

Lê o ADC e retorna o estado da linha:

| Retorno | Estado | Tensão A0 | analogRead |
|:-------:|--------|:---------:|:----------:|
| 0 | FORA DA LINHA | 0 V | ~0 |
| 1 | ESQUERDA | 1,1 V | ~341 |
| 2 | DIREITA | 1,65 V | ~512 |
| 3 | MEIO DA LINHA | 1,98 V | ~614 |

**Exemplo:**
```cpp
int estado = lineSensor.read();
if (estado == 0) {
  Debug.println("Perdeu a linha!");
}
```

---

## `Robot`

Coordena os módulos de motor, sensor ultrassônico e seguidor de linha
para executar o comportamento do robô seguidor de trajetória
com desvio de obstáculos.

### includes

```cpp
#include <Robot.h>
```

### Construtor

```cpp
Robot(MotorController& motors, UltrasonicSensor& sensor,
      LineFollower& lineSensor);
```

Recebe **referências** para o controlador de motores, sensor
ultrassônico e seguidor de linha.

**Exemplo:**

```cpp
MotorController motors(AIN1, AIN2, PWMA, BIN1, BIN2, PWMB);
UltrasonicSensor sensor(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
LineFollower lineSensor(LINE_SENSOR);
Robot robot(motors, sensor, lineSensor);
```

### `setup()`

```cpp
void setup();
```

Inicializa o robô:

1. `_motors.begin()`
2. `_motors.setSpeed(VEL_BASE)` — padrão 180
3. `_motors.stop()`
4. Imprime `Robô iniciado.` no log

### `update()`

```cpp
void update();
```

Executa um ciclo completo do comportamento. Deve ser chamado dentro
de `loop()`. A cada chamada:

1. Lê a distância do sensor ultrassônico
2. **Se obstáculo a < 15 cm**: `stop()`, pausa 300 ms, `turnLeft()`,
   espera 400 ms, retorna
3. **Senão**: lê o `LineFollower` e corrige trajetória com
   **diferencial de velocidade** (curva suave):
   - **FORA (0)**: `forward()` com `VEL_BASE` (tenta reencontrar)
   - **ESQUERDA (1)**: motor A mais rápido, motor B mais lento →
     corrige para a esquerda
   - **DIREITA (2)**: motor A mais lento, motor B mais rápido →
     corrige para a direita
   - **MEIO (3)**: `forward()` com `VEL_BASE` (centrado)
4. Aguarda 50 ms

### Constantes internas (ajustáveis)

| Constante | Valor | Efeito |
|-----------|-------|--------|
| `VEL_BASE` | 180 | Velocidade base dos motores (0–255) |
| `VEL_CURVA` | 60 | Diferencial aplicado nas correções |
| `DIST_MINIMA` | 15 cm | Distância que dispara o desvio |

Para alterar, edite `lib/Robot/src/Robot.h`.

---

## `DebugLog`

Substitui o `Serial.print()` convencional por uma saída que vai
simultaneamente para **Serial USB** e para uma **página web**
servida via WiFi (Access Point).

Exclusiva para ESP8266 — depende de `ESP8266WiFi.h` e `ESP8266WebServer.h`.

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
#include <LineFollower.h>
#include <Robot.h>

MotorController motors(AIN1, AIN2, PWMA, BIN1, BIN2, PWMB);
UltrasonicSensor sensor(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
LineFollower lineSensor(LINE_SENSOR);
Robot robot(motors, sensor, lineSensor);

void setup() {
  Debug.begin();
  lineSensor.begin();
  robot.setup();
}

void loop() {
  Debug.handleClient();
  robot.update();
}
```
