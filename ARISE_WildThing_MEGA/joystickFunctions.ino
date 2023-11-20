// custom functions for x-y joysticks

void AutoCenterJoystick() {
///////////////////////////////////////////////////////////////////////////////////////////////////
// << AutoCenterJoystick >>
///////////////////////////////////////////////////////////////////////////////////////////////////
  // Check joystick is in rest position (noone is moving it) and find nominal X & Y resting positions before starting loop
  /*
    The following section of code checks the X & Y positions of the joystick before starting the Loop 
    This ensures the joystick is not pushed at startup (aka should be hands-off, so machine does not take off immediately when turned on)
    Also monitoring the X & Y positions for a certain amount of time allows us to automatically find the REST Position (Center) of X & Y
    We will record a Window of X & Y positions ensure they are within the expected "resting" range.
    Calibrations allow to define the number of samples to check in a single window and how many windows to check (aka. # of attempts is basically a timeout)
  */

  // JOYSTICK STARTUP TEST CALIBRATIONS
  int joyPosRestingMin = (512 - 60); // min assumed "resting" position (0-1023 A2D)
  int joyPosRestingMax = (512 + 60); // max assumed 'resting" position (0-1023 A2D)
  int joyPosRestingTol = 16; // max allowable noise at rest (total range max-min recorded during check) (0-1023 A2D)

  Serial.print("<<< START Joystick Check >>> "); Serial.print(joySelect);  
  Serial.print("\n");
  
  bool joyCheckSuccessful = false; // flag to keep track if joystick passes startup test
  // Variables for Joystick min & max measured during window
    int minX;
    int maxX;
    int minY;
    int maxY;
  // Check joystick is in center and not moving (if it's out of center or moving, then try again up to numWindows)
  for (int window = 1; window <= numWindows; window++) {
    // initialize min & max at first measurment in window
    minX = analogRead(joyX);
    maxX = analogRead(joyX);
    minY = analogRead(joyY);
    maxY = analogRead(joyY);
    // sample the joystick the requested number of times
    for (int sample = 0; sample < numSamples; sample++) {
      //sample joystick
      joyPosX = analogRead(joyX);
      joyPosY = analogRead(joyY);
      // keep track of min & max for X & Y
      minX = min(joyPosX,minX);
      maxX = max(joyPosX,maxX);
      minY = min(joyPosY,minY);
      maxY = max(joyPosY,maxY);
    }
    // check min & max within allowed range and the difference between them is within resting tolerance (aka not moving or noisy)
    if ( minX >= joyPosRestingMin && maxX <= joyPosRestingMax && minY >= joyPosRestingMin && maxY <= joyPosRestingMax && (maxX - minX) <= joyPosRestingTol && (maxY - minY) <= joyPosRestingTol ) {
      joyCheckSuccessful = true;
      Serial.print("<<< Joystick Passed Startup Check >>>");
      Serial.print("\t");
      Serial.print("Number of attempts = ");
      Serial.print(window);
      Serial.print("\n");
      break;
    } else {
      // Print out results
      Serial.print("Attempt # "); Serial.print(window); Serial.print(" FAILED. Joystick "); Serial.print(joySelect); Serial.print("\n");
      Serial.print("minX = "); Serial.print(minX); Serial.print("\t");
      Serial.print("maxX = "); Serial.print(maxX); Serial.print("\t");
      Serial.print("minY = "); Serial.print(minY); Serial.print("\t");
      Serial.print("maxY = "); Serial.print(maxY); Serial.print("\t");
      Serial.print("\n");
    }
  }

  if (joyCheckSuccessful) {
      joyPassed = true;
      joyInit = true;
      
      xRest = (minX + maxX)/2; //set to mid-range of recorded signal
      yRest = (minY + maxY)/2; //set to mid-range of recorded signal
      
      // Print out results
      Serial.print("joyX at Rest = "); Serial.print(xRest); Serial.print("\t");
      Serial.print("minX = "); Serial.print(minX); Serial.print("\t");
      Serial.print("maxX = "); Serial.print(maxX); Serial.print("\t");
      
      Serial.print("joyY at Rest = "); Serial.print(yRest); Serial.print("\t");
      Serial.print("minY = "); Serial.print(minY); Serial.print("\t");
      Serial.print("maxY = "); Serial.print(maxY); Serial.print("\t");
      
      Serial.print("\n");
      
  } else {
      joyPassed = false;
      
      Serial.print("<<< Joystick FAILED Startup Check >>>");
      Serial.print("\n");
      
      // Print out results
      Serial.print("minX = "); Serial.print(minX); Serial.print("\t");
      Serial.print("maxX = "); Serial.print(maxX); Serial.print("\t");
      
      Serial.print("minY = "); Serial.print(minY); Serial.print("\t");
      Serial.print("maxY = "); Serial.print(maxY); Serial.print("\t");
      
      Serial.print("\n");
      /*
      bool hold = true; // set system to hold here forever & Blink LEDs
      while (hold == true) {
        hold = true;
      }
      */
  }

  Serial.print("<<< END Joystick Check >>>");
  Serial.print("\n");
  
}

void createJoystickTables() {
// conversion tables to convert joystick angle (0to360 degrees) to Motor Left & Right Mix (-100to100 %)
// convention is joystick angle 0 degrees = straight ahead (North) and 90 degrees = to the right (East)
// also provides lookuptable for non-linear radius control (calculates %power from 0-100 radius)

//#include "joystickCals.h";

// Angle Lookup Table (Mix)

  // Generate LookupTables for L Mix
  float mixTableLTemp[15][2] = {
      { 0                , motorForward_PowerLevels },
      { TrimAngle , motorForward_PowerLevels + TrimPlus },
      { 90 - spinZone , motorTurnFor_PowerLevels    },
      { 90 - spinDwell, motorSpin_PowerLevels    },
      { 90 + spinDwell, motorSpin_PowerLevels    },
      { 90 + spinZone , 0 },
      { 180 - TrimAngle, - motorReverse_PowerLevels + TrimMinus },
      { 180 , - motorReverse_PowerLevels },
      { 180 + TrimAngle, - motorReverse_PowerLevels - TrimPlus },
      { 270 - spinZone, - motorTurnRev_PowerLevels },
      { 270 - spinDwell, - motorSpin_PowerLevels },
      { 270 + spinDwell, - motorSpin_PowerLevels },
      { 270 + spinZone, 0 },
      { 360 - TrimAngle, motorForward_PowerLevels - TrimMinus  },
      { 360              , motorForward_PowerLevels }
  };
  memcpy(mixTableL, mixTableLTemp, sizeof(mixTableLTemp));
    
  // Generate LookupTables for R Mix  
  float mixTableRTemp[15][2] = {
      { 0                , motorForward_PowerLevels },
      { TrimAngle , motorForward_PowerLevels - TrimMinus},
      { 90 - spinZone , 0    },
      { 90 - spinDwell, - motorSpin_PowerLevels    },
      { 90 + spinDwell, - motorSpin_PowerLevels    },
      { 90 + spinZone , - motorTurnRev_PowerLevels },
      { 180 - TrimAngle, - motorReverse_PowerLevels - TrimPlus},
      { 180 , - motorReverse_PowerLevels },
      { 180 + TrimAngle, - motorReverse_PowerLevels + TrimMinus },
      { 270 - spinZone, 0 },
      { 270 - spinDwell, motorSpin_PowerLevels },
      { 270 + spinDwell, motorSpin_PowerLevels },
      { 270 + spinZone, motorTurnFor_PowerLevels },
      { 360 - TrimAngle, motorForward_PowerLevels + TrimPlus },
      { 360              , motorForward_PowerLevels }
    
  };
  memcpy(mixTableR, mixTableRTemp, sizeof(mixTableRTemp));

// Radius Lookup Table (Scale)

  // Generate LookupTable for Scale
  float radTableTemp[6][2] = {
      { 0             , 0 },
      { Deadband   , 0 },
      { Deadband + .01 , 0 },
      { SlowRadius , SlowPower },
      { 1 -Endband , 1 },
      { 1             , 1 }
  };
  memcpy(radTable, radTableTemp, sizeof(radTableTemp));
  
}

void getPolarCoordinates() {
  // code adapted from: https://forum.arduino.cc/index.php?topic=609064.0
  float maxRadius = 400; // maximum assumed radius of joystick
  float deadZone = 3; // A2D zone near center to assume no motion

  #include <math.h>

  float deltaXpercent = deltaX/maxRadius;
  float deltaYpercent = deltaY/maxRadius;
  
  // to calculate the distance from the center (and, therefore, resulting force of motors) we use classic Pythagoras equation:
  joyRadius = sqrt(deltaXpercent*deltaXpercent + deltaYpercent*deltaYpercent); // use this silly ^2 multiplication, as Arduino's pow() function is prone to errors
  //joyRadius = (abs(deltaX) + abs(deltaY))/2 / maxRadius; // use this silly ^2 multiplication, as Arduino's pow() function is prone to errors
  joyRadius = min( 1,max(-1,joyRadius));
  
  double rad;
  if (deltaY == 0) {
    if (deltaX >= 0) { rad = 1.570796326795;} else { rad = -1.570796326795;} // Beware of horizontal (deltaY ==0) angles as those may result in division by zero!
  } else {
    rad = atan2 (deltaX, deltaY);
  }
  
  if (joyRadius <= deadZone / maxRadius) { joyAngle = 0; } else {
      
    // For conversion to degrees you use the following formula:
    joyAngle = rad * 57.295779513082320876798154814105; // we assume that a radian is 57.29 (and a bit) degrees.
    
    // adjust joyAngle for joystick installed orientation
    joyAngle = joyAngle + joyOffsetAngle;
    // avoid overrun (joyAngle always should be between 0 and 360)
    if (joyAngle >= 360) {
      joyAngle = joyAngle - 360;
      } else {
        if (joyAngle < 0) {
          joyAngle = joyAngle + 360;
        }
      }
    // swap spin direction if required
    if (flipSpin) { joyAngle = map(joyAngle,0,360,360,0); } // reverse direction if flipSpin = true
  }  
}

void selectJoystick() {
  ///////////////////////////////////////////////////////////////
  // check whether we need to use the occupant or tether joystick
  // Autocenter if it changes
  ///////////////////////////////////////////////////////////////

  // SPECIAL CONDITION FOR THIS CODE Joystick will always be Tether
  if (true) //(analogRead(JoySwitch_Main) > 800 )
  {
    //  Serial.print("Selected tether ");
    // If the onboard switch is on and the tether switch is not pushed, we use the tether joystick
    if (!usingTether || !joyInit)
    {
      // If we weren't using the tether previously, we must set it up
      Serial.print("Switching to the tether ");
      usingTether = true;
      Serial.print("Checking Tether Joystick "); Serial.print(joySelect); Serial.print("\n");
        joySelect = 1;
        joyX = joyX_Tether;
        joyY = joyY_Tether;
        joyOffsetAngle = joyOffsetAngle_Tether;
        flipSpin = flipSpin_Tether;
        numSamples = numSamples_Tether; // number of samples to check in a "window"
        numWindows = numWindows_Tether; // maximum attempts to check joystick

      // Check Joystick and AutoCenter
      AutoCenterJoystick(); // Check Joystick is near center and not moving, return "at Rest" values if pass test
    }
  }
  else
  {
    // Serial.print("Selected Onboard ");
    // We use the occupant joystick
    if (usingTether || !joyInit)
    {
      // If we weren't using the occupant previously, we must set it up
      Serial.print("Switching to the occupant ");
      usingTether = false;
      Serial.print("Checking Occupant Joystick "); Serial.print(joySelect); Serial.print("\n");
        joySelect = 2;
        joyX = joyX_Occupant;
        joyY = joyY_Occupant;
        joyOffsetAngle = joyOffsetAngle_Occupant;
        flipSpin = flipSpin_Occupant;
        numSamples = numSamples_Occupant; // number of samples to check in a "window"
        numWindows = numWindows_Occupant; // maximum attempts to check joystick

      // Check Joystick and AutoCenter
      AutoCenterJoystick(); // Check Joystick is near center and not moving, return "at Rest" values if pass test
    }
  }

}
