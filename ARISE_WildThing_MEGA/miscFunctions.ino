// misc functions

void getInputs() {
    readPot(); // Read speed potentiometer and calc speedMultiplier
    // << Read the Joystick X and Y positions >>
    joyPosX = analogRead(joyX);
    joyPosY = analogRead(joyY);
}

float interpolate(float x, const float table[][2], int numRows) {
  // Function to perform linear interpolation on a 2D lookup table

  // Check if x is outside the range of the table
  if (x <= table[0][0]) {
    return table[0][1];
  } else if (x >= table[numRows - 1][0]) {
    return table[numRows - 1][1];
  }

  // Find the index of the two nearest x values in the table
  for (int i = 0; i < numRows; i+=1) {
    // Perform linear interpolation
    float x1 = table[i][0];
    float x2 = table[i + 1][0];
    float y1 = table[i][1];
    float y2 = table[i + 1][1];
    if ( x<=x2 ) { 
      if (x2==x1) {// protect divByZero
        return y1;
      } else {
        return y1 + (x - x1) * (y2 - y1) / (x2 - x1);
      }
    }
  }
  // next i
  
  return table[numRows - 1][1]; // if not found return last y
}

void rateLimitMotors() {
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // Rate Limit Motor Velocity & Delay in case of zero crossing
  ///////////////////////////////////////////////////////////////////////////////////////////////////

    // check if xxx_Filt is 0; ok2ChangeDir_L = true if xxx_Filt is 0 for debounced time
    // because if xxx_Filt changes direction then we want to wait until ok2ChangeDir_x is true so we don't spike current or cause clunky behavior
    bool Loff = motorLVel_Filt==0;
    bool Roff = motorRVel_Filt==0;
    int ok2ChangeDir_L = zeroCrossDebL.debounceHigh(Loff); // debounce if motor desired velocity is 0 (in ms)
    int ok2ChangeDir_R = zeroCrossDebR.debounceHigh(Loff); // debounce if motor desired velocity is 0 (in ms)
    
    // rate limit LEFT motor
    if (motorLForward) { // we are in Forward direction
      motorLVel_next = motorLVel_Filt + min( maxAccel, max( -maxDecel, motorLVel_plusDropout - motorLVel_Filt ) ); // Rate Limit on Positive direction
      if (motorLVel_next > 0 || ok2ChangeDir_L) { // not changing direction OR ok2ChangeDir
        motorLVel_Filt = motorLVel_next ; // no wait
      } else { // changing direction and waiting for debounce
        motorLVel_Filt = 0 ;
      }
    } else { // we are in Reverse direction
      motorLVel_next = motorLVel_Filt + max( -maxAccel, min( maxDecel, motorLVel_plusDropout - motorLVel_Filt ) );  // Rate Limit on Negative direction
      if (motorLVel_next < 0 || ok2ChangeDir_L) { // not changing direction OR ok2ChangeDir
        motorLVel_Filt = motorLVel_next ; // no wait
      } else { // changing direction and waiting for debounce
        motorLVel_Filt = 0 ;
      }
    }

    // rate limit RIGHT motor
    if (motorRForward) { // we are in Forward direction
      motorRVel_next = motorRVel_Filt + min( maxAccel, max( -maxDecel, motorRVel_plusDropout - motorRVel_Filt ) ); // Rate Limit on Positive direction
      if (motorRVel_next > 0 || ok2ChangeDir_R) { // not changing direction OR ok2ChangeDir
        motorRVel_Filt = motorRVel_next ; // no wait
      } else { // changing direction and waiting for debounce
        motorRVel_Filt = 0 ;
      }
    } else { // we are in Reverse direction
      motorRVel_next = motorRVel_Filt + max( -maxAccel, min( maxDecel, motorRVel_plusDropout - motorRVel_Filt ) );  // Rate Limit on Negative direction
      if (motorRVel_next < 0 || ok2ChangeDir_R) { // not changing direction OR ok2ChangeDir
        motorRVel_Filt = motorRVel_next ; // no wait
      } else { // changing direction and waiting for debounce
        motorRVel_Filt = 0 ;
      }
    }

    // keep track of motor direction (Note velocity of 0 does not change direction)
    if (motorLVel_Filt > 0) { // last direction is forward (motorLForward initialized as true)
      motorLForward = true;
    }
    if (motorLVel_Filt < 0) { // last direction is reverse
      motorLForward = false;
    }
    if (motorRVel_Filt > 0) { // last direction is forward (motorRForward initialized as true)
      motorRForward = true;
    }
    if (motorRVel_Filt < 0) { // last direction is reverse
      motorRForward = false;
    }

}

void readPot() {
  // reads potentiometer output and rescales
  double potValue = analogRead(PowerLevelPotInput);
  speedMultiplier = rescale(potValue, potValueRescale[0], potValueRescale[1], potValueRescale[2], potValueRescale[3]); // linear conversion of speed dial to max speed
  speedMultiplier = speedMultiplier * speedMultiplier; // exponential factor for better resolution down low
}

float rescale(float x, float x1, float x2, float y1, float y2) {
  return (x - x1) * (y2 - y1) / (x2 - x1) + y1;
}

float rescaleForDropout(float motorVel, float Dropout) {
  if( motorVel > 0 ) {
    return rescale(motorVel, 0, 1, Dropout, 1);
  } else if ( motorVel < 0 ) {
    return rescale(motorVel, -1, 0, -1, -Dropout);
  } else {
    return 0;
  }
}

void setJoystickOutOfRangeError(){
      // If we got here, we probably have a bad connection in the Joystick Circuit so stop (ramp down motors) and throw error
      Serial.println(""); Serial.print("ERROR: Joystick out of Range. Check Joystick Pin and Connector Connections."); Serial.println("");
      // Need to set motor speeds to 0 (motorLVel_Filt & motorRVel_Filt will let rate limiter take care of decel rate limits)
      joyAngle = 0;
      joyRadius = 0;
}

void setJoystickLearnError(){
  // If Joystick Fails to learn center, You end up here.
  Serial.print("Failed Joystick Checks. Stopping Run. "); Serial.print("\n");
  joySelect = 0; // <1> Tethered (default) or <2> Occupant <0> Error
  // Print Joystick Readings;
  Serial.print("Occupant X "); Serial.print("\t"); Serial.print(analogRead(joyX_Occupant)); Serial.print("\t");
  Serial.print("Occupant Y "); Serial.print("\t"); Serial.print(analogRead(joyY_Occupant)); Serial.print("\t");
  Serial.print("Tether X "); Serial.print("\t"); Serial.print(analogRead(joyX_Tether)); Serial.print("\t");
  Serial.print("Tether Y "); Serial.print("\t"); Serial.print(analogRead(joyY_Tether)); Serial.print("\t");
  //Serial.print("JoySwitch_Tether = "); Serial.print(digitalRead(JoySwitch_Tether)); Serial.print("\t");
  //Serial.print("JoySwitch_Onboard = "); Serial.print(digitalRead(JoySwitch_Onboard)); Serial.print("\t");
  Serial.print("JoySwitch_Main = "); Serial.print(analogRead(JoySwitch_Main)); Serial.print("\t");
  Serial.print("PowerLevelPotInput = "); Serial.print(analogRead(PowerLevelPotInput)); Serial.print("\t");
  Serial.print("\n");
}
