void task_DS18B20_id ()
{
  /** */
  sensors.requestTemperatures();
  /** */
  temperature_var = sensors.getTempC(insideThermometer);
  Serial.print("Temp C: ");
  Serial.print(temperature_var);
}
