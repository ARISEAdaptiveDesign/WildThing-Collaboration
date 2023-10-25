// CONFIGURATION PARAMETERS //

#ifndef Config_H
#define Config_H

// Occupant Joystick
const float joyOffsetAngle_Occupant = 0; // polar orientation of joystick (offset # of degrees to get forward to equal 0) <OCCUPANT>
const bool flipSpin_Occupant = false; // use true or false to change SPIN direction (flip if chair left/right is wrong) <OCCUPANT>
const int numSamples_Occupant = 30; // number of samples to check in a "window"
const int numWindows_Occupant = 30; // maximum attempts to check joystick
const float OccupantDownrate = 1.0; // downrate of occupant input for scaling speed (0 to 1)

// Tethered Joystick
const float joyOffsetAngle_Tether = 0; // polar orientation of joystick (offset # of degrees to get forward to equal 0) <TETHER>
const bool flipSpin_Tether = false; // use true or false to change SPIN direction (flip if chair left/right is wrong) <TETHER>
const int numSamples_Tether = 30; // number of samples to check in a "window"
const int numWindows_Tether = 30; // maximum attempts to check joystick
const float TetherDownrate = 1.0; // downrate of thether input for scaling speed (0 to 1)

// Straight Ahead Trim Factors
const float trimFactor = 0.0; // Calibration to compensate if one motor stronger than the other. Typical 0, with range between +/-10% where positive value will strengthen left motor and weaken the right one

float motorDropout = 0.05; // typical range: 0.01 to 0.15 ;motor dropout ( % of motorMaxSpeed ) Set this to the minimum % it takes to drive the motor. "Too high" will cause troubles when turning because the inside motor "will not shut off smoothly"

  // Angles
  const float spinZone      = 15; // number of degrees above or below East or West that one motor turning and other is stopped
  const float TrimAngle = 10; // degrees travel on either side of forward/backward that is treated as true forward/backward
  const float spinDwell     = 3; // degrees travel on either side of left/right that is treated as true spin
  // Speeds
  const float motorForward_PowerLevels = 1.0 ; // range: 0.01 to 1 max FORWARD Speed  (% output for Motor pwm)
  const float motorReverse_PowerLevels = 0.75 ; // range: 0.01 to 1 max REVERSE Speed (% output for Motor pwm)
  const float motorTurnFor_PowerLevels = 0.9 ; // range: 0.01 to 1 motor speeds used for FORWARD Turn movement (aka one wheel spins at this speed while other is stopped)  (% output for Motor pwm)
  const float motorTurnRev_PowerLevels = 0.70 ; // range: 0.01 to 1 motor speeds used for REVERSE Turn movement (aka one wheel spins at this speed while other is stopped)  (% output for Motor pwm)
  const float motorSpin_PowerLevels = 0.65 ; // range: 0.01 to 1 motor speeds used for Spin movement (aka each motor spins at this speed in opposite directions)  (% output for Motor pwm)
  const float TrimPlus = 0.05 ; // range: 0.00 to 0.2 Added Speed for Outside Wheel within TrimAngle (% output for Motor pwm)
  const float TrimMinus = 0.1 ; // range: 0.00 to 0.2 Minus Speed for Inside Wheel within TrimAngle (% output for Motor pwm)

// Radius Lookup Table (Scale)
  const float Deadband   = .1 ; // 0 to .1 (%) suggested around rest position before moving (NOTE this will be used in + and - directions, so actual deadband is 2x)
  const float Endband    = .01 ; // 0 to .1 (%) suggested at end of range to ignore
  const float SlowRadius = 0.4 ; // 0 to 1 (%) of the Joystick range to use for low speed control (x-axis end of 1st ramp & begin 2nd ramp in lookup table)
  const float SlowPower  = 0.2 ; // 0 to 1 (%) of Power to use for low speed control (y-axis end of 1st ramp & begin 2nd ramp in lookup table)

// Acceleration & Deceleration Limits
  const float maxAccel = 0.05 ; // range: 0.01 to 1 max acceleration of motor per timestep
  const float maxDecel = 0.08 ; // range: 0.01 to 1 max deceleration of motor per timestep
  const float zeroCrossingDwell = 10 ; // number of timesteps (aka loops) to keep motor off between changes in direction (note not "ms", just counting loops)
  const int BrakeRamp = 10 ; // per loop ramp rate to turn on braking for vnh5019.
  const int maxBrake = 400 ; // per loop ramp rate to turn on braking for vnh5019. max braking is 400

// rescales the potentiometer output to get a speed multiplier
  float potValueRescale[4] = {30, 990, .35, .85};  //speedMultiplier = rescale(potValue, 540, 950, .35, .85);

#endif
