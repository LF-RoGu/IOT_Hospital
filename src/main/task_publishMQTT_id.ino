void task_publishMQTT_id ()
{
  client.add("SPO2", spo2);  // Insert your variable Labels and the value to be sent
  client.ubidotsPublish("iot_hospital");
  client.loop();
}
