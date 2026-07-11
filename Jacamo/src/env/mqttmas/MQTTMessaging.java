// CArtAgO artifact code for project ag2frontJacamo

package mqttmas;

import cartago.*;

import java.nio.charset.StandardCharsets;
import java.util.*;
import org.eclipse.paho.client.mqttv3.*;

import com.fasterxml.jackson.databind.ObjectMapper;



public class MQTTMessaging extends Artifact {
	private MqttClient client;
	public void init() throws Exception {
        
        String broker = "tcp://localhost:1883";

            client = new MqttClient(broker, "Jacamo");

            client.setCallback(new MqttCallback() {

                @Override //here is where i forward to the router component
                public void messageArrived(String topic, MqttMessage message) {
                    System.out.println("MQTT received: " + topic + " -> " + message);
                }

                @Override
                public void connectionLost(Throwable cause) {
                    System.out.println("MQTT connection lost");
                }

                @Override
                public void deliveryComplete(IMqttDeliveryToken token) {}
            });

            client.connect();
            client.subscribe("ui/pub");
            client.subscribe("mcu/pub");
            ObjectMapper mapper = new ObjectMapper();
            Map<String, Object> msg = new HashMap<>();
            msg.put("id", "jc1");
            msg.put("sender", "agent");
            msg.put("text","Agent Connected to Broker.");

            String json = mapper.writeValueAsString(msg);
            System.out.println(json);
            client.publish("agent/pub", new MqttMessage(
                    json.getBytes(StandardCharsets.UTF_8)
                ));

            System.out.println("MQTT initialized");
    }
}

