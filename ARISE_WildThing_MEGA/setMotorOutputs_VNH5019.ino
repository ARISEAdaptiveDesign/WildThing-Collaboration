void setMotorOutputs_VNH5019(float motorOutL, float motorOutR){
        if (motorOutL==0){
        // Braking function (time based - open loop)
        BrakeL = min(BrakeL+BrakeRamp,maxBrake); //increment brake at defined rate to avoid skids ;), Limit at maxBrake
        md.setM1Brake(BrakeL); // set brake pwm
      } else {
        // Drive Motor
        BrakeL = 0; //reset brake level if driving motor
        motorLSpeed = rescale(motorOutL, -1, 1, -400, 400); //scaling for vnh5019
        md.setM1Speed(motorLSpeed); // set motor drive pwm
      }
      if (motorOutR==0){
        // Braking function (time based - open loop)
        BrakeR = min(BrakeR+BrakeRamp,maxBrake); //increment brake at defined rate to avoid skids ;), limit at maxBrake
        md.setM2Brake(BrakeR); // set brake pwm
      } else {
        // Drive Motor
        BrakeR = 0; //reset brake level if driving motor
        motorRSpeed = rescale(motorOutR, -1, 1, -400, 400); //scaling for vnh5019
        md.setM2Speed(motorRSpeed); // set motor drive pwm
      }
    //stopIfFault();

}
