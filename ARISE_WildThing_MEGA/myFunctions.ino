// reusable custom functions

void readPot() {
  // reads potentiometer output and rescales
  double potValue = analogRead(PowerLevelPotInput);
  speedMultiplier = rescale(potValue, potValueRescale[0], potValueRescale[1], potValueRescale[2], potValueRescale[3]); // linear conversion of speed dial to max speed
  speedMultiplier = speedMultiplier * speedMultiplier; // exponential factor for better resolution down low
}

float rescale(float x, float x1, float x2, float y1, float y2)
{
  return (x - x1) * (y2 - y1) / (x2 - x1) + y1;
}
