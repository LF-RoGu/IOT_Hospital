void task_Max30105_id ()
{
  long irValue = particleSensor.getIR();

  if (checkForBeat(irValue) == true)
  {
    //We sensed a beat!
    int64_t delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
      rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
      rateSpot %= RATE_SIZE; //Wrap variable

      //Take average of readings
      beatAvg = 0;
      for (uint8_t x = 0 ; x < RATE_SIZE ; x++)
      {
        beatAvg += rates[x];
      }
      beatAvg /= RATE_SIZE;
    }
  }

  Serial.print("IR=");
  Serial.print(irValue);
  Serial.print(", BPM=");
  Serial.print(beatsPerMinute);
  Serial.print(", Avg BPM=");
  Serial.print(beatAvg);


  Serial.print(" ");
  oxymeter_var = irValue / irOffset;
  Serial.print("Oxygen=");
  Serial.print(oxymeter_var);
  Serial.print("%");

  float temperatureF = particleSensor.readTemperatureF(); //Because I am a bad global citizen
  temperatureF = temperatureF + degOffset;
  
  Serial.print(" Temp(F)=");
  Serial.print(temperatureF, 2);
  Serial.print("°");

  Serial.print(" IR=");
  Serial.print(irValue);

  if (irValue < 50000) 
  {
    Serial.print(" No finger?");
    noFinger = noFinger+1;
  } else 
  {
    //only count and grab the reading if there's something there.
    count = count+1;
    avgIr = avgIr + irValue;
    avgTemp = avgTemp + temperatureF;
    Serial.print(" ... ");
  }

  //Get an average IR value over 100 loops
  if (count == 100) 
  {
    avgIr = avgIr / count;
    avgTemp = avgTemp / count;
    Serial.print(" avgO=");
    Serial.print(avgIr);
    Serial.print(" avgF=");
    Serial.print(avgTemp);

    Serial.print(" count=");
    Serial.print(count);
    //reset for the next 100
    count = 0; 
    avgIr = 0;
    avgTemp = 0;
  }
  Serial.println();
}
