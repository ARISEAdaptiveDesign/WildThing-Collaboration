// JOYSTICK SETTINGS //
// See also createJoystickTables

#ifndef CalibrationJoystick_H
#define CalibrationJoystick_H

// Occupant Joystick
const float joyOffsetAngle_Occupant = -90; // polar orientation of joystick (offset # of degrees to get forward to equal 0) <OCCUPANT>
const bool flipSpin_Occupant = false; // use true or false to change SPIN direction (flip if chair left/right is wrong) <OCCUPANT>
const int numSamples_Occupant = 30; // number of samples to check in a "window"
const int numWindows_Occupant = 30; // maximum attempts to check joystick

// Tethered Joystick
const float joyOffsetAngle_Tether = 0; // polar orientation of joystick (offset # of degrees to get forward to equal 0) <TETHER>
const bool flipSpin_Tether = false; // use true or false to change SPIN direction (flip if chair left/right is wrong) <TETHER>
const int numSamples_Tether = 30; // number of samples to check in a "window"
const int numWindows_Tether = 30; // maximum attempts to check joystick

const float motorDropout = 0.08; // motor dropout ( % of motorMaxSpeed ) Set this to the minimum % it takes to drive the motor.


  // Angles
  const float spinZone      = 20; // number of degrees above or below East or West that one motor turning and other is stopped
  const float straightDwell = 0; // degrees travel on either side of forward/backward that is treated as true forward/backward
  const float spinDwell     = 0; // degrees travel on either side of left/right that is treated as true spin
  // Speeds
  const float motorForward_PowerLevels = 1 ; // range: 0.01 to 1 max FORWARD Speed  (% output for Motor pwm)
  const float motorReverse_PowerLevels = 0.30 ; // range: 0.01 to 1 max REVERSE Speed (% output for Motor pwm)
  const float motorTurnFor_PowerLevels = 0.80 ; // range: 0.01 to 1 motor speeds used for FORWARD Turn movement (aka one wheel spins at this speed while other is stopped)  (% output for Motor pwm)
  const float motorTurnRev_PowerLevels = 0.35 ; // range: 0.01 to 1 motor speeds used for REVERSE Turn movement (aka one wheel spins at this speed while other is stopped)  (% output for Motor pwm)
  const float motorSpin_PowerLevels = 0.50 ; // range: 0.01 to 1 motor speeds used for Spin movement (aka each motor spins at this speed in opposite directions)  (% output for Motor pwm)


// Radius Lookup Table (Scale)
  const float Deadband   = .1 ; // 0 to .1 (%) suggested around rest position before moving (NOTE this will be used in + and - directions, so actual deadband is 2x)
  const float Endband    = .01 ; // 0 to .1 (%) suggested at end of range to ignore
  const float SlowRadius = 0.4 ; // 0 to 1 (%) of the Joystick range to use for low speed control (x-axis end of 1st ramp & begin 2nd ramp in lookup table)
  const float SlowPower  = 0.2 ; // 0 to 1 (%) of Power to use for low speed control (y-axis end of 1st ramp & begin 2nd ramp in lookup table)

// Acceleration & Deceleration Limits
  const float maxAccel = 0.05 ; // range: 0.01 to 1 max acceleration of motor per timestep
  const float maxDecel = 0.12 ; // range: 0.01 to 1 max deceleration of motor per timestep
  const float zeroCrossingDwell = 3 ; // number of timesteps (aka loops) to keep motor off between changes in direction (note not "ms", just counting loops)
  const int BrakeRamp = 100 ; // per loop ramp rate to turn on braking for vnh5019. max braking is 400

// zero crossing variables
  bool motorLForward = true;
  float motorLVel_next = 0;
  int zeroCrossingCountL = 0;
  bool motorRForward = true;
  float motorRVel_next = 0;
  int zeroCrossingCountR = 0; //counter how long we are at zero
  int M1Brake = 0 ; // LEFT motor Value to brake for vnh5019, braking is value 0 min and 400 max
  int M2Brake = 0 ; // RIGHT motor Value to brake for vnh5019, braking is value 0 min and 400 max

// keeps track of whether we need to set up the joystick or tether in the loop
  bool usingTether = false;
#endif