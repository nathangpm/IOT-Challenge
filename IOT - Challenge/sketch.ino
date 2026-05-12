#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// ==================== CONFIGURAÇÕES ====================
const char* WIFI_SSID     = "Wokwi-GUEST";
const char* WIFI_PASSWORD = "";

const char* MQTT_SERVER   = "broker.hivemq.com";
const int   MQTT_PORT     = 1883;
const char* MQTT_CLIENT   = "pawcare-esp32";

const char* TOPIC_TEMP    = "pawcare/temperatura";
const char* TOPIC_UMID    = "pawcare/umidade";
const char* TOPIC_MOV     = "pawcare/movimento";
const char* TOPIC_STATUS  = "pawcare/status";

#define DHT_PIN  15
#define PIR_PIN  14
#define LED_PIN  2

// =======================================================

#define DHTTYPE DHT22
DHT dht(DHT_PIN, DHTTYPE);
WiFiClient espClient;
PubSubClient mqtt(espClient);

unsigned long ultimaLeitura = 0;
const long INTERVALO = 3000;

// ==================== FUNÇÕES ====================

void conectarWiFi() {
  Serial.println("================================");
  Serial.println("  PawCare Monitor - Iniciando  ");
  Serial.println("================================");
  Serial.print("Conectando ao WiFi");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("[OK] WiFi conectado!");
  Serial.print("     IP: ");
  Serial.println(WiFi.localIP());
}

void conectarMQTT() {
  while (!mqtt.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (mqtt.connect(MQTT_CLIENT)) {
      Serial.println(" [OK] Conectado!");
      mqtt.publish(TOPIC_STATUS, "PawCare Monitor Online");
    } else {
      Serial.print(" [ERRO] Estado: ");
      Serial.print(mqtt.state());
      Serial.println(" | Tentando novamente em 3s...");
      delay(3000);
    }
  }
}

void publicarDados(float temp, float umid, bool movimento) {
  char tempStr[8];
  char umidStr[8];

  dtostrf(temp, 5, 2, tempStr);
  dtostrf(umid, 5, 2, umidStr);

  mqtt.publish(TOPIC_TEMP, tempStr);
  mqtt.publish(TOPIC_UMID, umidStr);
  mqtt.publish(TOPIC_MOV,  movimento ? "1" : "0");
}

void imprimirDados(float temp, float umid, bool movimento) {
  Serial.println("--------------------------------");
  Serial.print("[TEMP] Temperatura : ");
  Serial.print(temp, 1);
  Serial.println(" C");

  Serial.print("[UMID] Umidade     : ");
  Serial.print(umid, 1);
  Serial.println(" %");

  if (movimento) {
    Serial.println("[MOV]  Movimento   : DETECTADO!");
  } else {
    Serial.println("[MOV]  Movimento   : Pet em repouso");
  }
  Serial.println("--------------------------------");
}

// ==================== SETUP & LOOP ====================

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  dht.begin();

  conectarWiFi();

  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  conectarMQTT();
}

void loop() {
  if (!mqtt.connected()) {
    conectarMQTT();
  }
  mqtt.loop();

  unsigned long agora = millis();
  if (agora - ultimaLeitura >= INTERVALO) {
    ultimaLeitura = agora;

    float temperatura = dht.readTemperature();
    float umidade     = dht.readHumidity();
    bool movimento = digitalRead(PIR_PIN) == HIGH;

    if (!isnan(temperatura) && !isnan(umidade)) {
      imprimirDados(temperatura, umidade, movimento);
      publicarDados(temperatura, umidade, movimento);

      // LED pisca ao publicar os dados
      digitalWrite(LED_PIN, HIGH);
      delay(100);
      digitalWrite(LED_PIN, LOW);

    } else {
      Serial.println("[ERRO] Falha na leitura do DHT22");
    }
  }
}
