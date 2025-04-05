import org.eclipse.paho.client.mqttv3.*;
public class MqttSubscribeSample {

    public static void main(String[] args) throws Exception {

        // Broker and Topic Setup
        String broker = "tcp://mqtt.eclipseprojects.io:1883";
        String topic = "VadaPavMan69/#"; // Subscribe to all subtopics
        /*
        UNIQUE ID WAS THE PROBLEMMMMM GODDDDDDDD DAMMMM ITTTTTTTTT
         */
        // Dynamic ID everytime to avoid conflicts on reconnection
        String clientId = "MyJavaClient-" + System.currentTimeMillis();

        // Create MQTT Client
        MqttClient client = new MqttClient(broker, clientId);

        // Connection Settings
        // Some brokers have explicit keep alive times so we don't specify it
        MqttConnectOptions options = new MqttConnectOptions();
        options.setAutomaticReconnect(true); // Reconnect if disconnected
        options.setCleanSession(true);       // Start fresh each time

        // Message Handler
        client.setCallback(new MqttCallback() {
            @Override
            public void messageArrived(String topic, MqttMessage message) {
                System.out.println("Temperature : " + new String(message.getPayload()) + " C");
            }

            @Override
            public void connectionLost(Throwable cause) {
                System.out.println("Connection lost! Will reconnect...");
            }

            @Override
            public void deliveryComplete(IMqttDeliveryToken token) {
                // Not needed for subscriptions
            }
        });

        // 5. Connect and Subscribe
        System.out.println("Connecting to broker...");
        client.connect(options);
        client.subscribe(topic);
        System.out.println("Ready! Waiting for messages...");

        // 6. Keep the program running
        while (true) {
            Thread.sleep(1000); // Prevents exit
        }
    }
}