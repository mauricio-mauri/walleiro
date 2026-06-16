# Walleiro — Robô Seguidor de Trajetória

**Disciplina:** Robótica Móvel  
**Universidade:** Universidade do Estado do Pará (UEPA)

---

## 📋 Sobre o Projeto

O **Walleiro** é um robô móvel que segue uma trajetória predefinida
enquanto desvia de obstáculos com sensor ultrassônico HC-SR04, usando
o driver **TB6612FNG** para controle dos motores DC.

```
        ┌──────────────────┐
        │   NodeMCU v3     │
        │   (ESP8266)      │
        │   ┌──────────┐   │
        │   │  Cérebro │   │
        │   └──────────┘   │
        └──┬──────┬──────┬─┘
           │      │      │
     ┌─────┘      │      └──────┐
     ▼            ▼             ▼
 ┌────────┐ ┌─────────┐ ┌────────────┐
 │TB6612  │ │HC-SR04  │ │ Power Bank │
 │FNG     │ │Ultrassom│ │   5V       │
 │Driver  │ │         │ │            │
 └───┬────┘ └─────────┘ └────────────┘
     │
     ├──── Motor DC A (esquerdo)
     └──── Motor DC B (direito)
```

**Componentes principais:**

- **NodeMCU v3 (ESP8266)** — processa sensores, controla motores,
  serve página web de debug via WiFi
- **TB6612FNG** — ponte H MOSFET com queda de ~0,2V (eficiência
  muito superior ao L298N, que perdia ~2V)
- **HC-SR04** — sensor ultrassônico (alimentado em 3.3V, ECHO
  compatível com GPIO direto)
- **Motores DC** — tração diferencial
- **Power Bank 5V** — alimentação portátil

**Duas formas de trabalhar:**

| Ambiente | Placa | Finalidade |
|----------|-------|------------|
| Simulação (Wokwi) | ESP32 | Testar lógica sem hardware |
| Físico | ESP8266 NodeMCU v3 | Rodar no robô real |

O código é **único** (`src/main.cpp`) e usa `#ifdef ESP32` para compilar
o trecho certo para cada placa.

---

## 📡 Debug sem fio (WiFi AP)

O robô cria a própria rede WiFi no boot:

| Parâmetro | Valor |
|-----------|-------|
| SSID | `Walleiro` |
| Senha | `12345678` |
| IP | `192.168.4.1` |
| Porta | 80 (HTTP) |

**Uso:**

1. Alimente o robô pela power bank
2. Conecte o celular/PC na rede `Walleiro`
3. Abra o navegador em `http://192.168.4.1/`
4. Veja as distâncias e eventos atualizando a cada 1 segundo

> Quando conectado ao PC via USB, o log também aparece no Serial Monitor
> (115200 baud).

---

## 🔌 Pinagem

### NodeMCU v3 (placa física)

| GPIO | Pino | Sinal | Componente |
|------|------|-------|------------|
| 5 | D1 | TRIG | HC-SR04 |
| 4 | D2 | ECHO | HC-SR04 |
| 14 | D5 | AIN1 | TB6612FNG |
| 12 | D6 | AIN2 | TB6612FNG |
| 13 | D7 | PWMA | TB6612FNG |
| 0 | D3 | BIN1 | TB6612FNG |
| 2 | D4 | BIN2 | TB6612FNG |
| 15 | D8 | PWMB | TB6612FNG |
| 16 | D0 | IR_PIN | HW-201 (futuro) |

**Alimentação:**

| Pino NodeMCU | Tensão | Alimenta |
|-------------|--------|----------|
| VIN | 5V | NodeMCU (via power bank USB) |
| VU | ~4,7V | TB6612 VCC + STBY |
| 3V3 | 3,3V | HC-SR04 VCC, HW-201 VCC |
| GND | 0V | Comum a todos os módulos |

### ESP32 (simulação Wokwi)

| GPIO | Sinal | Componente |
|------|-------|------------|
| 18 | TRIG | HC-SR04 |
| 19 | ECHO | HC-SR04 |
| 25 | AIN1 | TB6612FNG (LED no diagrama) |
| 26 | AIN2 | TB6612FNG |
| 32 | PWMA | TB6612FNG (LED indicador) |
| 27 | BIN1 | TB6612FNG |
| 14 | BIN2 | TB6612FNG |
| 33 | PWMB | TB6612FNG (LED indicador) |

> Na simulação, os motores DC são representados por LEDs nos pinos
> PWM (32 e 33). O Wokwi não tem modelos de motor DC.

---

## ⚙️ Configuração do Ambiente

### 1. Instalar o VS Code

Baixe em [code.visualstudio.com](https://code.visualstudio.com) e instale.

### 2. Instalar a extensão PlatformIO

Abra o VS Code, vá em **Extensões** (Ctrl+Shift+X) e procure por
`platformio.platformio-ide`. Instale.

### 3. Clonar o repositório

```bash
git clone https://github.com/mauricio-mauri/walleiro.git
cd walleiro
```

### 4. Abrir no PlatformIO

No VS Code: **File > Open Folder** → selecione a pasta `walleiro`.

O PlatformIO reconhece automaticamente o projeto e baixa as toolchains
necessárias na primeira compilação.

### 5. Compilar

```bash
# Simulação (ESP32)
pio run

# Placa física (ESP8266)
pio run -e nodemcuv2
```

> Se `pio run` não funcionar, use o caminho completo:
> `~/.platformio/penv/bin/pio run`

---

## 🚀 Comandos Úteis

| Ação | Comando |
|------|---------|
| Compilar (simulação) | `pio run` |
| Compilar (placa física) | `pio run -e nodemcuv2` |
| Upload ESP8266 | `pio run -e nodemcuv2 --target upload` |
| Upload com porta específica | `pio run -e nodemcuv2 --target upload --upload-port /dev/ttyUSB0` |
| Monitor serial | `pio device monitor -b 115200` |
| Limpar build | `pio run --target clean` |
| Simular Wokwi | `wokwi` |

---

## 🔄 Fluxo de Trabalho com Git

Cada membro trabalha em seu próprio fork ou branch. O fluxo sugerido:

```
main  (protegida — não commitar direto)
  └── <nome-do-membro>/<funcionalidade>
```

**Passo a passo:**

```bash
# 1. Criar branch para sua tarefa
git checkout -b joao/teste-motor

# 2. Fazer alterações
git add .
git commit -m ":sparkles: feat: teste de motor DC com TB6612FNG"

# 3. Enviar para o GitHub
git push -u origin joao/teste-motor

# 4. Abrir Pull Request no GitHub
```

**Padrão de commits (iuricode/padroes-de-commits):**

| Tipo | Emoji | Quando usar |
|------|-------|-------------|
| `feat` | ✨ `:sparkles:` | Novo recurso |
| `fix` | 🐛 `:bug:` | Correção de bug |
| `docs` | 📚 `:books:` | Documentação |
| `refactor` | ♻️ `:recycle:` | Refatoração |
| `chore` | 🔧 `:wrench:` | Configuração / tarefas |
| `cleanup` | 🧹 `:broom:` | Limpeza de código |
| `remove` | 🗑️ `:wastebasket:` | Remoção de arquivos |
| `test` | ✅ `:white_check_mark:` | Testes |
| `perf` | ⚡ `:zap:` | Performance |
| `raw` | 🗃️ `:card_file_box:` | Dados / parâmetros |
| `init` | 🎉 `:tada:` | Commit inicial |

**Exemplos:**

```bash
git commit -m ":sparkles: feat: controle PWM dos motores"
git commit -m ":books: docs: atualização do README"
git commit -m ":bug: fix: correção na leitura do sensor"
```

> A pasta `.pio/` está no `.gitignore` e **nunca** deve ser commitada.

---

## 🧪 Simulação Wokwi

Wokwi é um simulador de circuitos que roda dentro do VS Code.
Ele permite testar o código **sem precisar do hardware físico**.

**Limitação:** o Wokwi só simula ESP32, não ESP8266.
Por isso o projeto tem dois ambientes de compilação.

**Como usar:**

```bash
# 1. Compile o firmware para ESP32
pio run

# 2. Inicie a simulação (VS Code → aba Wokwi → Start)
```

O arquivo `diagram.json` define os componentes e conexões do circuito
simulado.

**Fluxo recomendado:**

```
Edita código → Compila (pio run) → Simula (Wokwi) → Testa
→ Se funcionar: compila pra ESP8266 → upload na placa real
```

---

## 🛠️ Solução de Problemas

### Erro de permissão na porta USB

```bash
sudo usermod -a -G dialout $USER
# (Reinicie a sessão depois)
```

### Upload falha mesmo com o ESP conectado

O ESP8266 pode estar em modo de boot incorreto. Tente:

1. Segure o botão **FLASH** (ou **IO0**) no NodeMCU
2. Aperte **RST** (reset) rapidamente
3. Solte o **FLASH**
4. Execute o upload novamente

### Wokwi não abre

- Verifique se a extensão "Wokwi for VS Code" está instalada
- Verifique se `pio/build/esp32_sim/firmware.bin` existe (rode `pio run`)

### Monitor serial não mostra nada

- Confirme a porta: `ls /dev/ttyUSB*`
- Confirme o baud rate: `-b 115200`
- O ESP pode estar em reset loop — aperte RST

---

## 📁 Estrutura do Projeto

```
walleiro/
├── src/
│   └── main.cpp              # Código-fonte único
├── lib/
│   ├── MotorController/      # Driver TB6612FNG
│   ├── UltrasonicSensor/     # Sensor HC-SR04
│   ├── Robot/                # Lógica de navegação
│   └── DebugLog/             # Debug USB + WiFi AP
├── include/
│   └── pins.h                # Definição de pinos
├── docs/
│   ├── ARCHITECTURE.md       # Arquitetura do firmware
│   ├── API.md                # Referência das classes
│   └── HARDWARE.md           # Montagem e ligações
├── test/                     # (vazio — sem testes ainda)
├── diagram.json              # Circuito Wokwi
├── wokwi.toml                # Configuração Wokwi
├── platformio.ini            # Build (2 envs)
└── README.md                 # Este arquivo
```

---

## 🧰 Componentes

| Componente | Quantidade | Função |
|------------|:----------:|--------|
| NodeMCU v3 (ESP8266) | 1 | Microcontrolador |
| TB6612FNG | 1 | Driver ponte H MOSFET |
| Motor DC 3-6V | 2 | Tração diferencial |
| HC-SR04 | 1 | Sensor ultrassônico |
| HW-201 | 1 | Sensor infravermelho |
| Power Bank 5V | 1 | Alimentação |

---

## 👥 Equipe

| Nome | Função |
|------|--------|
| mauricio-mauri | Arquitetura geral, documentação e API |
| <!-- (preencher) --> | |
| <!-- (preencher) --> | |
| <!-- (preencher) --> | |
| <!-- (preencher) --> | |
| <!-- (preencher) --> | |

---

## 📚 Referências

- [TB6612FNG Datasheet](https://www.sparkfun.com/datasheets/Robotics/TB6612FNG.pdf)
- [HC-SR04 Datasheet](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf)
- [ESP8266 NodeMCU v3 Pinout](https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/)
- [ESP32 DevKit v4 Pinout](https://raw.githubusercontent.com/espressif/ESP8266_MP3_Decoder/master/pinout/ESP32-pinout.png)
- [Wokwi — Simulação de IoT](https://wokwi.com)
- [PlatformIO Docs](https://docs.platformio.org)
- [iuricode/padroes-de-commits](https://github.com/iuricode/padroes-de-commits)
