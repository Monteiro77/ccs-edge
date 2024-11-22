
# **Monitor Solar com ESP32 e MQTT**

Este projeto utiliza o microcontrolador ESP32 para monitorar e exibir em tempo real as informações de um sistema solar fotovoltaico, como tensão, corrente e potência. Os dados são enviados via protocolo MQTT, permitindo sua análise em dispositivos conectados.

---

## **Objetivo do Projeto**

O objetivo principal é criar um sistema simples e eficiente para monitorar a geração de energia solar, fornecendo informações essenciais para:
- Identificar problemas de desempenho.
- Garantir o uso eficiente da energia gerada.
- Monitorar a geração em tempo real, localmente e remotamente.

---

## **Como Funciona?**

1. **Hardware**:
   - **ESP32**: Faz a leitura dos dados e gerencia a comunicação.
   - **Sensores de Tensão e Corrente**:
     - Um divisor de tensão mede a tensão gerada pelo painel solar.
     - Um sensor de corrente (como ACS712) mede a corrente fornecida.
   - **Display LCD**: Exibe as leituras localmente.

2. **Software**:
   - **MQTT**: Protocolo de comunicação para enviar dados ao servidor.
   - **Servidor MQTT**: O broker (`test.mosquitto.org`) recebe os dados do ESP32 e os disponibiliza para dispositivos conectados.
   - **Monitoramento**:
     - Ferramentas como **MQTT Explorer** ou scripts Python podem visualizar os dados.

---

## **Componentes Utilizados**

### **Hardware**
- ESP32
- Sensor de corrente (ex.: ACS712)
- Divisor de tensão (resistores para adequar os valores)
- Display LCD (I2C)
- Jumpers e protoboard

### **Software**
- Biblioteca Arduino:
  - `WiFi.h`
  - `PubSubClient.h` (MQTT)
  - `NTPClient.h` (horário NTP)
  - `LiquidCrystal_I2C.h` (controle do LCD)
- Broker MQTT: `test.mosquitto.org`

---

## **Configuração do Projeto**

1. **Conexão do Hardware**:
   - Conecte o sensor de tensão ao pino 36 (ADC1_CH0).
   - Conecte o sensor de corrente ao pino 39 (ADC1_CH3).
   - Ligue o display LCD ao barramento I2C do ESP32.

2. **Configuração do Código**:
   - No arquivo Arduino, atualize o nome e senha da sua rede Wi-Fi:
     ```cpp
     #define SSID "Seu_SSID"
     #define PASS "Sua_Senha"
     ```
   - Carregue o código no ESP32 usando a IDE Arduino.

3. **Monitoramento no PC**:
   - Use o **MQTT Explorer** ou crie um script Python para subscrever os tópicos:
     - `solar/voltage` (tensão)
     - `solar/current` (corrente)
     - `solar/power` (potência)

---

## **Como Rodar o Projeto?**

1. Conecte o ESP32 à rede Wi-Fi.
2. O ESP32 começará a:
   - Ler os dados dos sensores.
   - Exibir os valores no display LCD.
   - Publicar os dados via MQTT.
3. No PC, abra o **MQTT Explorer** ou um cliente MQTT para visualizar os dados.

---

## **Exemplo de Dados**

- **Tópicos MQTT**:
  - `solar/voltage`: 18.45 V
  - `solar/current`: 2.10 A
  - `solar/power`: 38.74 W

- **No Display**:
  ```
  V: 18.45 V
  I: 2.10 A
  P: 38.74 W
  ```

---

## **Possíveis Melhorias**

- Adicionar armazenamento dos dados em um banco de dados local ou na nuvem.
- Implementar alarmes para condições específicas (ex.: baixa geração).
- Integrar com plataformas como **ThingSpeak** ou **Node-RED**.

---

