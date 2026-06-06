# Walleiro — Robô Seguidor de Trajetória

**Disciplina:** Robótica Móvel  
**Universidade:** Universidade do Estado do Pará (UEPA)

---

## 📋 Sobre o Projeto

O **Walleiro** é um robô móvel que segue uma trajetória predefinida enquanto desvia
de obstáculos com um sensor ultrassônico.

```
        ┌──────────────────┐
        │   NodeMCU v3     │
        │   (ESP8266)      │
        │   ┌──────────┐   │
        │   │  Cérebro  │   │
        │   └──────────┘   │
        └──┬──────┬──────┬─┘
           │      │      │
     ┌─────┘      │      └──────┐
     ▼            ▼             ▼
 ┌────────┐ ┌────────┐ ┌────────────┐
 │ L298N  │ │HC-SR04 │ │ Power Bank │
 │Driver  │ │Ultrassom│ │   5V       │
 │Motores │ │        │ │            │
 └───┬────┘ └────────┘ └────────────┘
     │
     ├──── Motor DC A (esquerdo)
     └──── Motor DC B (direito)
```

**Componentes principais e suas funções:**

- **NodeMCU v3 (ESP8266)** — microcontrolador que processa os dados do sensor e
  decide para onde os motores devem girar
- **L298N** — ponte H que permite controlar a direção e velocidade dos motores DC
- **HC-SR04** — sensor ultrassônico que mede distância até obstáculos (2 cm a 4 m)
- **Motores DC** — tração diferencial: cada roda tem seu próprio motor
- **Power Bank 5V** — alimentação portátil para todo o sistema

**Duas formas de trabalhar:**

| Ambiente | Placa | Finalidade |
|----------|-------|------------|
| Simulação (Wokwi) | ESP32 | Testar lógica e sensores sem hardware |
| Físico | ESP8266 NodeMCU v3 | Rodar no robô real |

O código é **único** (`src/main.cpp`) e usa `#ifdef ESP32` para compilar o trecho
certo para cada placa.

---

## 🔌 Pinagem

### ESP32 (simulação Wokwi)

| Sinal  | GPIO | Componente  | Fio (diagram.json) |
|--------|------|-------------|--------------------|
| TRIG   | 18   | HC-SR04     | Verde              |
| ECHO   | 19   | HC-SR04     | Amarelo            |
| IN1    | 25   | L298N       | Laranja            |
| IN2    | 26   | L298N       | Laranja            |
| IN3    | 27   | L298N       | Laranja            |
| IN4    | 14   | L298N       | Laranja            |
| ENA    | 32   | L298N (PWM) | Roxo               |
| ENB    | 33   | L298N (PWM) | Roxo               |

### NodeMCU v3 (placa física) — Pinagem Inicial

Ainda estamos definindo os pinos do circuito real. Por enquanto,
usamos apenas estes para o **primeiro teste com 1 motor DC**:

| Sinal | GPIO | Silkscreen |
|-------|------|------------|
| IN1   | 14   | D5         |
| IN2   | 12   | D6         |
| ENA   | 13   | D7         |

> ⚠️ Os pinos de TRIG, ECHO, IN3, IN4 e ENB serão definidos quando os
> componentes adicionais forem montados.

**Por que os números mudam?**

O ESP32 e o ESP8266 têm GPIOs diferentes. O `#ifdef ESP32` no código
resolve isso: cada placa usa seus próprios números na compilação.

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

### 5. Primeira compilação

```bash
# Simulação (ESP32)
pio run

# Placa física (ESP8266)
pio run -e nodemcuv2
```

> Se `pio run` não funcionar, use o caminho completo:
> `~/.platformio/penv/bin/pio run`

---

## 🚀 Primeiros Passos — Teste do ESP8266

Antes de montar qualquer circuito, vamos verificar se o NodeMCU v3
está funcionando.

### Materiais necessários

- NodeMCU v3 (ESP8266)
- Cabo USB micro

### Conexão

```
USB do PC
  │
  └── NodeMCU v3 (porta micro USB)
```

**Apenas o ESP conectado ao computador. Nenhum outro componente.**

### Código de teste

```cpp
#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);  // GPIO2 / D4
}

void loop() {
  digitalWrite(LED_BUILTIN, LOW);   // Liga LED (ativo LOW)
  Serial.println("ESP8266 vivo!");
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);  // Desliga LED
  delay(500);
}
```

### Procedimento

1. Substitua o conteúdo de `src/main.cpp` pelo código acima
2. Compile e faça upload:

   ```bash
   pio run -e nodemcuv2 --target upload
   ```

3. Abra o monitor serial:

   ```bash
   pio device monitor -b 115200
   ```

### O que esperar

- LED azul na placa piscando a cada 1 segundo
- Mensagem `ESP8266 vivo!` aparecendo a cada segundo no monitor serial

> Para restaurar o código original depois: `git checkout src/main.cpp`

---

## 🚀 Comandos Úteis

| Ação | Comando |
|------|---------|
| Compilar (simulação) | `pio run` |
| Compilar (placa física) | `pio run -e nodemcuv2` |
| Upload ESP8266 | `pio run -e nodemcuv2 --target upload` |
| Upload com porta específica | `pio run -e nodemcuv2 --target upload --upload-port /dev/ttyUSB0` |
| Monitor serial | `pio device monitor -b 115200` |
| Monitor serial (porta específica) | `pio device monitor -b 115200 -p /dev/ttyUSB0` |
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
# ...
git add .
git commit -m ":sparkles: feat: teste de motor DC com L298N"

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
git commit -m ":construction: Em progresso"
```

**Importante:** a pasta `.pio/` (build artifacts) está no `.gitignore`
e **nunca** deve ser commitada.

---

## 🧪 Simulação Wokwi

Wokwi é um simulador de circuitos online que roda dentro do VS Code.
Ele permite testar o código **sem precisar do hardware físico**.

**Limitação importante:** o Wokwi só simula ESP32, não ESP8266.
Por isso o projeto tem dois ambientes de compilação.

**Como usar:**

```bash
# 1. Compile o firmware para ESP32
pio run

# 2. Inicie a simulação
wokwi
```

O arquivo `diagram.json` define os componentes e conexões do circuito
simulado. A configuração está em `wokwi.toml`.

**Fluxo recomendado:**

```
1. Edita o código em src/main.cpp
2. Compila (pio run)
3. Simula (wokwi)
4. Testa o comportamento
5. Se funcionar → compila pra ESP8266 e faz upload na placa real
```

---

## 🛠️ Solução de Problemas

### Erro de permissão na porta USB

```bash
# Adicione seu usuário ao grupo dialout
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
- Verifique se o arquivo `.pio/build/esp32_sim/firmware.bin` existe
  (rode `pio run` antes do `wokwi`)

### Monitor serial não mostra nada

- Confirme que a porta está correta: `ls /dev/ttyUSB*`
- Confirme o baud rate: `-b 115200`
- O ESP pode estar em reset loop — aperte RST

---

## 📁 Estrutura do Projeto

```
walleiro/
├── src/
│   └── main.cpp          # Código-fonte único (ifdef pra cada placa)
├── lib/                   # Bibliotecas privadas (opcional)
│   └── README
├── include/               # Headers (opcional)
│   └── README
├── test/                  # Testes (opcional)
│   └── README
├── diagram.json           # Circuito da simulação Wokwi
├── wokwi.toml             # Configuração do Wokwi
├── platformio.ini         # Configuração de build (2 envs)
├── .gitignore             # Arquivos ignorados pelo git
└── README.md              # Este arquivo
```

---

## 🧰 Componentes

| Componente | Quantidade | Função |
|------------|:----------:|--------|
| NodeMCU v3 (ESP8266) | 1 | Microcontrolador — processa sensores e controla motores |
| L298N | 1 | Ponte H — permite inverter direção e controlar velocidade dos motores DC |
| Motor DC | 2 | Tração diferencial (uma roda em cada lado) |
| HC-SR04 | 1 | Sensor ultrassônico — mede distância até obstáculos |
| Power Bank 5V | 1 | Alimentação portátil do sistema |
| Jumpers (macho-macho e macho-fêmea) | vários | Conexões entre componentes |

<!-- FOTO: componentes_individuais.jpg -->

> **Nota:** fotos da montagem passo a passo serão adicionadas assim que
> o circuito físico for montado.

---

## 👥 Equipe

| Nome | Função |
|------|--------|
| <!-- (preencher) --> | |
| <!-- (preencher) --> | |
| <!-- (preencher) --> | |
| <!-- (preencher) --> | |
| <!-- (preencher) --> | |
| <!-- (preencher) --> | |

---

## 📚 Referências

- [L298N Datasheet](https://www.st.com/resource/en/datasheet/l298.pdf)
- [HC-SR04 Datasheet](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf)
- [ESP8266 NodeMCU v3 Pinout](https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/)
- [ESP32 DevKit v4 Pinout](https://raw.githubusercontent.com/espressif/ESP8266_MP3_Decoder/master/pinout/ESP32-pinout.png)
- [Wokwi — Simulação de IoT](https://wokwi.com)
- [PlatformIO Docs](https://docs.platformio.org)
- [Conventional Commits](https://www.conventionalcommits.org/pt-br)
- [iuricode/padroes-de-commits](https://github.com/iuricode/padroes-de-commits)
