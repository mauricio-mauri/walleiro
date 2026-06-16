# Hardware — Montagem e Ligações

## Componentes

### Ligados

| Componente | Quantidade | Função |
|------------|:----------:|--------|
| NodeMCU v3 (ESP8266) | 1 | Microcontrolador |
| TB6612FNG (HW-166) | 1 | Driver de motor (ponte H MOSFET) |
| Motor DC 3-6V 200 RPM | 2 | Tração diferencial |
| HC-SR04 | 1 | Sensor ultrassônico |
| HW-201 | 1 | Sensor infravermelho (logicamente desconectado) |
| Power Bank GoPro 5V | 1 | Alimentação |

### Desconectados (disponíveis)

- Potenciômetro
- Display 7 segmentos
- Servo motor
- Capacitor
- Motor DC 3-6V 200 RPM (reserva)
- HW-201 - Sensor infravermelho (4 disponíveis)

---

## Alimentação

```
Power Bank 5V
  │
  └── Cabo USB cortado
        ├── VERMELHO ── 5V (breadboard +)
        │                ├── NodeMCU VIN
        │                └── TB6612 VM
        │
        └── PRETO    ── GND comum  (breadboard -)
```

| Fonte | Tensão | Alimenta |
|-------|--------|----------|
| NodeMCU VIN | 5V (power bank) | NodeMCU (regulador 3.3V interno) |
| NodeMCU 3V3 | 3.3V | HC-SR04 VCC, HW-201 VCC |
| NodeMCU VU | ~4.7V | TB6612 VCC (lógica) |
| Power Bank (cabo cortado) | 5V | Breadboard → NodeMCU VIN, TB6612 VM (motores) |

### Por que 3.3V nos sensores?

HC-SR04 e HW-201 alimentados com **3.3V** em vez de 5V:

| Com 5V | Com 3.3V |
|--------|----------|
| ECHO e OUT = 5V → precisa de divisor de tensão | ECHO e OUT = 3.3V → direto no GPIO ✅ |
| Alcance total do HC-SR04 (~4m) | Alcance reduzido (~2m) |

**Ganho:** sem resistores extras, sem risco de queimar o pino do ESP8266.

---

## Ligações

### ESP8266 (NodeMCU v3)

| Pino | GPIO | Destino | Componente | Fio |
|------|------|---------|------------|-----|
| VIN | — | 5V | Power Bank | Vermelho |
| GND | — | GND | Power Bank | Preto |
| 3V3 | — | VCC, VCC | HC-SR04, HW-201 | Marrom |
| VU | — | VCC, STBY | TB6612FNG | Branco |
| D0 | 16 | OUT | HW-201 | Cinza |
| D1 | 5 | TRIG | HC-SR04 | Laranja |
| D2 | 4 | ECHO | HC-SR04 | Cinza |
| D3 | 0 | BIN1 | TB6612FNG | Amarelo |
| D4 | 2 | BIN2 | TB6612FNG | Amarelo |
| D5 | 14 | AIN1 | TB6612FNG | Amarelo |
| D6 | 12 | AIN2 | TB6612FNG | Amarelo |
| D7 | 13 | PWMA | TB6612FNG | Roxo |
| D8 | 15 | PWMB | TB6612FNG | Roxo |

### TB6612FNG (HW-166)

| Pino TB6612 | Conecta em | Componente | Fio |
|-------------|------------|------------|-----|
| VM | 5V | Power Bank | Vermelho |
| VCC | VU (NodeMCU) | ESP8266 | Branco |
| GND | GND | Power Bank | Preto |
| STBY | VU (NodeMCU) | ESP8266 | Branco |
| A01 | Motor A fio 1 | Motor DC 1 | Azul |
| A02 | Motor A fio 2 | Motor DC 1 | Azul |
| B01 | Motor B fio 1 | Motor DC 2 | Verde |
| B02 | Motor B fio 2 | Motor DC 2 | Verde |
| AIN1 | D5 | ESP8266 | Amarelo |
| AIN2 | D6 | ESP8266 | Amarelo |
| PWMA | D7 | ESP8266 | Roxo |
| BIN1 | D3 | ESP8266 | Amarelo |
| BIN2 | D4 | ESP8266 | Amarelo |
| PWMB | D8 | ESP8266 | Roxo |

### HC-SR04

| Pino | Conecta em | Fio |
|------|------------|-----|
| VCC | NodeMCU 3V3 | Marrom |
| GND | GND | Preto |
| TRIG | NodeMCU D1 | Laranja |
| ECHO | NodeMCU D2 | Cinza |

### HW-201 (desconectado logicamente)

| Pino | Conecta em |
|------|------------|
| VCC | NodeMCU 3V3 |
| GND | GND |
| OUT | NodeMCU D0 |

---

## Segurança no Boot

Durante o boot do ESP8266, alguns GPIOs têm comportamento especial:

| GPIO | Pino | Estado boot | Efeito no TB6612 |
|------|------|-------------|-------------------|
| 0 | D3 | Pullup HIGH | BIN1=HIGH + BIN2=HIGH = freio |
| 2 | D4 | Pullup HIGH | BIN1=HIGH + BIN2=HIGH = freio |
| 15 | D8 | Pulldown LOW | PWMB=0 → motor B desligado ✓ |

**Resultado:** os dois motores ficam desligados durante o boot.
Nenhum movimento involuntário.

---

## Segurança Elétrica

| Verificação | OK? |
|-------------|:---:|
| HC-SR04 VCC no **3V3**, não no 5V | ⚠️ |
| HW-201 VCC no **3V3**, não no 5V | ⚠️ |
| STBY jumper no **VCC** (TB6612 funcionar) | ⚠️ |
| GND power bank = GND NodeMCU = GND TB6612 (comum) | ⚠️ |
| VM no cabo cortado, **não** no VU | ⚠️ |

Marque com ✅ antes de ligar a energia.

---

## Esquema Visual

![Esquema de montagem](sources/walleiro_bb.jpg)

> Esquema visual dos componentes — feito no Fritzing.
