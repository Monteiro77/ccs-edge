#include <WiFi.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <LiquidCrystal_I2C.h>

// Configurações Wi-Fi e MQTT
#define SSID "Wokwi-GUEST"
#define PASS ""
#define MQTTSERVER "test.mosquitto.org"
#define PORT 1883

// Configurações do LCD
#define COLUNAS 20
#define LINHAS 4
LiquidCrystal_I2C lcd(0x27, COLUNAS, LINHAS);

// Objetos de comunicação
WiFiClient espClient;
PubSubClient client(espClient);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "a.st1.ntp.br", -10800, 60000);

// Variáveis para sensores e cálculo
const int pinVoltage = 36; // Pino do sensor de tensão
const int pinCurrent = 39; // Pino do sensor de corrente
float voltage = 0.0;
float current = 0.0;
float power = 0.0;

unsigned long time_ms = 0;

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();

  // Conexão Wi-Fi
  connectWiFi();

  // Configuração MQTT
  client.setServer(MQTTSERVER, PORT);
  client.setCallback(callback);

  // Configuração do NTP
  timeClient.begin();

  lcd.setCursor(0, 0);
  lcd.print("Monitor Solar");
}

void connectWiFi() {
  Serial.print("Conectando ao Wi-Fi...");
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    lcd.setCursor(0, 1);
    lcd.print("Conectando...");
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Conectado!");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  Serial.println("Conectado!");
}

void mqttReconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (client.connect("ESP32-Solar")) {
      Serial.println("Conectado!");
      client.subscribe("solar/control");
    } else {
      Serial.print("Falha, rc=");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem recebida: ");
  Serial.println(topic);
}

void readSensors() {
  int rawVoltage = analogRead(pinVoltage);
  int rawCurrent = analogRead(pinCurrent);

  // Conversão para valores reais (ajustar conforme o divisor/sensor utilizado)
  voltage = (rawVoltage / 4095.0) * 5.0 * 11.0; // Divisor de tensão 10:1
  current = (rawCurrent / 4095.0) * 30.0 - 15.0; // Exemplo para ACS712 (corrente centrada em 0)

  // Calcular potência
  power = voltage * current;
}

void publishData() {
  char buffer[20];

  // Publicar tensão
  snprintf(buffer, sizeof(buffer), "%.2f", voltage);
  client.publish("solar/voltage", buffer);

  // Publicar corrente
  snprintf(buffer, sizeof(buffer), "%.2f", current);
  client.publish("solar/current", buffer);

  // Publicar potência
  snprintf(buffer, sizeof(buffer), "%.2f", power);
  client.publish("solar/power", buffer);
}

void displayData() {
  lcd.setCursor(0, 0);
  lcd.printf("V: %.2f V", voltage);
  lcd.setCursor(0, 1);
  lcd.printf("I: %.2f A", current);
  lcd.setCursor(0, 2);
  lcd.printf("P: %.2f W", power);
}

void loop() {
  if (!client.connected()) {
    mqttReconnect();
  }
  client.loop();

  if (millis() - time_ms >= 1000) {
    time_ms = millis();
    readSensors();
    publishData();
    displayData();
  }
}
