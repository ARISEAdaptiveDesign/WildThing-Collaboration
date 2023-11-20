// ARISE WildThing code for VNH5019 Motor Driver Shield for Arduino
/*
  ARISE - Syracuse, NY
  Main Contact: Connor McGough
  Coders: Bill Smith 2023/11/19
  >> SPECIAL CODE Joystick will always be Tether, Occupant joyX and joyY inputs will be used for switches
      In occupant mode:(Joystick select in occupant position): 
        If parent override switch is ON then use tether joystick, else if occupant switch is ON then go forward.
          (This means if no switch is on, then no movement and parent switch overrides occupant)
      In parent mode: (Joystick select in parent position)
        normal tether joystick control
*/

  #include "src/Debounce.h"
  #include "src/DualVNH5019MotorShield.h";
  #include "pinouts.h"; // Use for VNH5019 MotorShield
  #include "Config.h";
  #include "Init.h";
  DualVNH5019MotorShield md;

void setup() {
  Serial.begin(9600); // set communication between computer & Arduino
  md.init(); // initialize VNH5019
  pinMode(JoySwitch_Main,INPUT);
  createJoystickTables(); // Generate mix tables for Left & Right mix based on joystick angle and scale based on radius
}

void loop() {
  selectJoystick(); // Check if we use occupant or tether joystick & Interrupt to switch & Autocenter if it changes

  if (joyPassed == false) {
    setJoystickLearnError();
    
  } else {
    getInputs(); // read all inputs (speed pot & selected joystick)
    
    bool outOfRange = ( joyPosX < joyFaultBand || joyPosX > 1023-joyFaultBand || joyPosY < joyFaultBand || joyPosY > 1023-joyFaultBand ); // Check for broken wires or bad connection
    if (outOfRange) { // Stop motors if bad 
      setJoystickOutOfRangeError(); 
    } else { // Calc joyAngle & joyRadius if good
      deltaX = joyPosX - xRest; // calc A2D cartesian position from "at Rest"
      deltaY = joyPosY - yRest; // calc A2D cartesian position from "at Rest"
      getPolarCoordinates(); // convert cartesian deltaX and deltaY of joystick into polar coordinates joyAngle & joyRadius
    }

    //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    // SPECIAL CODE Joystick will always be Tether, GoRequest_Onboard will move straight forward if in occupant mode
      if ( analogRead(JoySwitch_Main) > 512 || analogRead(GoRequest_Tether) > 512 ) {
        // if in Parent mode or parent button is pushed then use Tether joystick
      } else if ( analogRead(GoRequest_Onboard) > 512) { // if in occupant mode, go straight forward if GoRequest_Onboard button is pushed
        joyAngle = 0;
        joyRadius = OccupantDownrate;
      } else { // if in occupant mode and GoRequest_Onboard button is not pushed, then stop
        joyAngle = 0;
        joyRadius = 0;
      }
    // <<< END SPECIAL CODE <<<<
    //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


    // Convert Polar Coordinates to Desired Speed & Mix using 2D Joystick Table Lookups 
      int numRows = sizeof(radTable)/sizeof(radTable[0]);
      scale = interpolate(joyRadius, radTable, numRows); // calc scale from joyRadius
      if (usingTether) {scale = scale*TetherDownrate;} else { scale = scale*OccupantDownrate;} // modify scale per customer desired speeds on tether vs onboard joysticks
      numRows = sizeof(mixTableL)/sizeof(mixTableL[0]);
      Lmix = interpolate(joyAngle, mixTableL, numRows); // calc Lmix from joyAngle
      numRows = sizeof(mixTableR)/sizeof(mixTableR[0]);
      Rmix = interpolate(joyAngle, mixTableR, numRows); // calc Rmix from joyAngle
      
    // Calc desired motor velocity as product of Mix (joystick angle), Scale (joystick radius), speedMultiplier (Speed Pot) and trimFactor (left vs right motor strength)
      motorLVel = min( 1, max( Lmix * scale * speedMultiplier * (1 + trimFactor), -1)); // limited between +/-1
      motorRVel = min( 1, max( Rmix * scale * speedMultiplier * (1 - trimFactor), -1)); // limited between +/-1
      
    // Rescale to set lowest motor on point to motorDropout <NOTE> Dropout is here instead of in scale lookup table because motor dropout cannot be modified by LMix, speedMultiplier, etc
      motorLVel_plusDropout = rescaleForDropout(motorLVel, motorDropout);
      motorRVel_plusDropout = rescaleForDropout(motorRVel, motorDropout);
      
    // Rate Limit Motor Velocity & Delay in case of zero crossing
      rateLimitMotors();
      
    // OUTPUT MOTOR SIGNALS
      setMotorOutputs_VNH5019(motorLVel_Filt, motorRVel_Filt);

    // Print info to Serial screen
      debug();
    // wait x milliseconds before the next loop
      delay(10);
  }
}
