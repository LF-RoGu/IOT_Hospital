void task_DS18B20_id ()
{
  /** */
  sensors.requestTemperatures();
  /** */
  temperature_var = sensors.getTempC(insideThermometer);
   if(temperature_var == DEVICE_DISCONNECTED_C) 
  {
    Serial.println("Error: Could not read temperature data");
    return;
  }
}
