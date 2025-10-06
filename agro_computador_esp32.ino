// ARQUIVO: agro_computador_esp32.ino

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h> // Usar para construir o JSON de forma eficiente

// --- CONFIGURAÇÕES ---
const char* WIFI_SSID = "SuaRedeWiFi";
const char* WIFI_PASS = "SuaSenha";
const char* MQTT_BROKER = "seu.broker.mqtt.com"; // Ex: IP do Mosquitto ou HiveMQ
const int MQTT_PORT = 1883;
const char* DEVICE_ID = "M01_NIASSA";
const char* TOPIC_TELEMETRY = "agro/telemetry/"; // Tópico para dados de sensores

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() { /* Lógica de conexão Wi-Fi aqui */ }
void reconnect_mqtt() { /* Lógica de reconexão MQTT aqui */ }

// Função para simular a leitura dos seus pinos GPIO
void read_sensors(float &temp_ar, int &umid_solo, int &lux) {
    // Implemente a leitura dos pinos GPIO/I2C/ADC aqui, conforme Tabela 9.
    // Exemplo Simulado:
    temp_ar = random(200, 300) / 10.0; // 20.0 a 30.0 C
    umid_solo = random(10, 80);        // 10 a 80 %
    lux = random(500, 1000);           // 500 a 1000 LUX
}

void loop() {
    if (!client.connected()) {
        reconnect_mqtt();
    }
    client.loop();

    float temp_ar, umid_solo;
    int lux;
    read_sensors(temp_ar, umid_solo, lux);

    // Criação do JSON (ArduinoJson usa menos memória)
    StaticJsonDocument<256> doc;
    doc["device_id"] = DEVICE_ID;
    doc["temp_ar"] = temp_ar;
    doc["umid_solo"] = umid_solo;
    doc["lux"] = lux;
    
    char output[256];
    serializeJson(doc, output);
    
    // Publica no tópico específico do dispositivo
    String topic = String(TOPIC_TELEMETRY) + DEVICE_ID;

    if (client.publish(topic.c_str(), output)) {
        Serial.print("Dados enviados: ");
        Serial.println(output);
    }

    // Intervalo de 30 minutos (Tabela 6)
    delay(1800000); 
    // delay(5000); // Para teste
}

void setup() {
    Serial.begin(115200);
    setup_wifi();
    client.setServer(MQTT_BROKER, MQTT_PORT);
}