void funct_HeartBeat ()
{
  /** Get IR*/
  int64_t ir_var = particleSensor.getIR();
  if(ir_var > 7000)
  {
    if(TRUE == checkForBeat(ir_var))
    {
      int64_t delta = millis() - lastBeat;                   //Measure duration between two beats
      lastBeat = millis();
      beatsPerMinute = 60 / (delta / 1000.0);

      if (beatsPerMinute < 255 && beatsPerMinute > 20)               //To calculate the average we strore some values (4) then do some math to calculate the average
      {
        rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
        rateSpot %= RATE_SIZE; //Wrap variable
        beatAvg = 0;
        for (byte x = 0 ; x < RATE_SIZE ; x++)
        {
          beatAvg += rates[x];
        }
        beatAvg /= RATE_SIZE;
      }
    }
  }
  else
  {
    Serial.print("Place Finger");
  }
}
