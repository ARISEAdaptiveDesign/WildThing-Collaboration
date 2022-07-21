 ///////////////////////////////////////////////////////////////////////////////////////////////////
 // << Function to blink the LEDs a given number of times >>
 ///////////////////////////////////////////////////////////////////////////////////////////////////
void blinkLED(int pin, int timeOn, int timeOff, int numberOfBlinks)
{
  for (int i = 0; i < numberOfBlinks; i++)
  {
    digitalWrite(pin, HIGH);
    delay(timeOn);
    digitalWrite(pin, LOW);
    delay(timeOff);
  }
}