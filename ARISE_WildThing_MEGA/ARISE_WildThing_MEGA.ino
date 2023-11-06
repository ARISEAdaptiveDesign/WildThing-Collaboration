// ARISE WildThing code for VNH5019 Motor Driver Shield for Arduino
/*
  ARISE - Syracuse, NY
  Main Contact: Connor McGough
  Coders: Bill Smith 2023/11/05
*/

  #include "pinouts_vnh5019.h"; // Use for VNH5019 MotorShield
  #include "Config.h";
  #include "Init.h";
  #include "DualVNH5019MotorShield.h";
  DualVNH5019MotorShield md;

void setup()
{
  Serial.begin(9600); // set communication between computer & Arduino
  md.init(); // initialize VNH5019
  pinMode(JoySwitch_Main,INPUT);

  // Generate mix tables for Left & Right mix based on joystick angle and scale based on radius
    createJoystickTables();
}

void loop()
{
  selectJoystick(); // Check if we use occupant or tether joystick & Interrupt to Autocenter if it changes

  if (joyPassed == true) {
  readPot(); // Read speed potentiometer and calc speedMultiplier
  // << Read the Joystick X and Y positions >>
  joyPosX = analogRead(joyX);
  joyPosY = analogRead(joyY);

  // Check for broken wires or bad connection. Stop motors if bad <ELSE> Calc Desired Motor Velocities if good
  if ( joyPosX < joyFaultBand || joyPosX > 1023-joyFaultBand || joyPosY < joyFaultBand || joyPosY > 1023-joyFaultBand) {
      // If we got here, we probably have a bad connection in the Joystick Circuit so stop (ramp down motors) and throw error
      Serial.println(""); Serial.print("ERROR: Joystick out of Range. Check Joystick Pin and Connector Connections."); Serial.println("");
      // Need to set motor speeds to 0 (motorLVel_Filt & motorRVel_Filt will let rate limiter take care of decel rate limits)
      joyAngle = 0;
      joyRadius = 0;
    } else {
      // Joystick wires seem OK, so go ahead...
      deltaX = joyPosX - xRest; // A2D cartesian position from "at Rest"
      deltaY = joyPosY - yRest; // A2D cartesian position from "at Rest"
      getPolarCoordinates(); // convert cartesian deltaX and deltaY of joystick into polar coordinates joyAngle & joyRadius
    }

    // figure out scale from radius
    for (int i = 0; i < 5; i+=1) {
      x1 = radTable[i][0];
      y1 = radTable[i][1];
      x2 = radTable[i+1][0];
      y2 = radTable[i+1][1];
      if ( joyRadius<=x2 ) {
        if (x2==x1) {scale = y1;} else {  scale = y1 + (joyRadius - x1) * (y2 - y1) / (x2 - x1);}
        if (usingTether) {scale = scale*TetherDownrate;} else { scale = scale*OccupantDownrate;}
        break; // jump out of loop when find value in lookup table
      }
    }

    // figure out Lmix from angle
    for (int i = 0; i < 14; i+=1) {
      x1 = mixTableL[i][0];
      y1 = mixTableL[i][1];
      x2 = mixTableL[i+1][0];
      y2 = mixTableL[i+1][1];
      if ( joyAngle<=x2 ) {
        if (x2==x1) {Lmix = y1;} else {  Lmix = y1 + (joyAngle - x1) * (y2 - y1) / (x2 - x1);}
        break; // jump out of loop when find value in lookup table
      }
    }

    // figure out Rmix from angle
    for (int i = 0; i < 14; i+=1) {
      x1 = mixTableR[i][0];
      y1 = mixTableR[i][1];
      x2 = mixTableR[i+1][0];
      y2 = mixTableR[i+1][1];
      if ( joyAngle<=x2 ) {
        if (x2==x1) {Rmix = y1;} else {  Rmix = y1 + (joyAngle - x1) * (y2 - y1) / (x2 - x1);}
        break; // jump out of loop when find value in lookup table
      }
    }

    // Set desired motor velocity as product of Mix (joystick angle), Scale (joystick radius), speedMultiplier (Speed Pot) and trimFactor (left vs right motor strength)
    motorLVel = min( 1, max( Lmix * scale * speedMultiplier * (1 + trimFactor), -1)); // limited between +/-1
    motorRVel = min( 1, max( Rmix * scale * speedMultiplier * (1 - trimFactor), -1)); // limited between +/-1

    // need to make sure that when turning on we start at motor min value
    // Dropout is put here instead of in scale lookup table because motor dropout cannot be modified by LMix, speedMultiplier, etc
      if( motorLVel > 0 ) {
        motorLVel_Dropout = rescale(motorLVel, 0, 1, motorDropout, 1);
      } else if ( motorLVel < 0 ) {
        motorLVel_Dropout = rescale(motorLVel, -1, 0, -1, -motorDropout);
      } else {
        motorLVel_Dropout = 0;
      }
      if( motorRVel > 0 ) {
        motorRVel_Dropout = rescale(motorRVel, 0, 1, motorDropout, 1);
      } else if ( motorRVel < 0 ) {
        motorRVel_Dropout = rescale(motorRVel, -1, 0, -1, -motorDropout);
      } else {
        motorRVel_Dropout = 0;
      }

    // Rate Limit Motor Velocity & Delay in case of zero crossing
    rateLimitMotors(); // rate limit motors and handle zerocrossing

    // OUTPUT MOTOR SIGNALS
    setMotorOutputs_VNH5019(motorLVel_Filt, motorRVel_Filt);

    // Print info to Serial screen
    debug();
    
    // wait x milliseconds before the next loop
    delay(10);
  
  } else {
    // If Joystick Fails, You end up here.
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
}
