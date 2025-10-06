// ARQUIVO: MqttDataReceiver.java (Cliente MQTT no Spring Boot)

@Component
public class MqttDataReceiver {

    // Injetar serviços de Armazenamento e IA
    @Autowired
    private SensorDataService sensorDataService;
    @Autowired
    private IaService iaService;

    @ServiceActivator(inputChannel = "mqttInputChannel") // Configurado via Spring Integration
    public void processaTelemetria(String payload) {
        
        // 1. DESSERIALIZAÇÃO: Converte o payload JSON (do ESP32) em um objeto Java
        SensorData data = JsonParser.parse(payload, SensorData.class); // Classe SensorData

        // 2. ARMAZENAMENTO: Salva os dados brutos no InfluxDB (Séries Temporais)
        sensorDataService.saveTelemetry(data); // InfluxDB

        // 3. LÓGICA DE IA: Envia os dados para a análise (que pode chamar o Python)
        String alerta = iaService.runPredictiveLogic(data);
        
        if (alerta != null) {
            // 4. ARMAZENAMENTO E ALERTA: Salva o alerta no MySQL e envia notificação
            Alert alert = new Alert(data.getDeviceId(), alerta, LocalDateTime.now());
            sensorDataService.saveAlert(alert); // MySQL
            // Envia Notificação Push/SMS (Tabela 6)
        }
    }
}