void rateLimitMotors() {
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // Rate Limit Motor Velocity & Delay in case of zero crossing
  ///////////////////////////////////////////////////////////////////////////////////////////////////

    // check if xxx_Filt is 0; ok2ChangeDir_L = true if xxx_Filt is 0 for debounced time
    int ok2ChangeDir_L = debounceHigh(motorLVel_Filt==0, zeroCrossingDwell); // debounce if motor desired velocity is 0 (in ms)
    int ok2ChangeDir_R = debounceHigh(motorRVel_Filt==0, zeroCrossingDwell); // debounce if motor desired velocity is 0 (in ms)
    // if xxx_Filt changes direction then wait (aka xxx_Filt = 0) until ok2ChangeDir_x is true
    
    // rate limit LEFT motor
    if (motorLForward) { // we are in Forward direction
      motorLVel_next = motorLVel_Filt + min( maxAccel, max( -maxDecel, motorLVel_Dropout - motorLVel_Filt ) ); // Rate Limit on Positive direction
      if (motorLVel_next > 0 || ok2ChangeDir_L) { // not changing direction OR ok2ChangeDir
        motorLVel_Filt = motorLVel_next ; // no wait
      } else { // changing direction and waiting for debounce
        motorLVel_Filt = 0 ;
      }
    } else { // we are in Reverse direction
      motorLVel_next = motorLVel_Filt + max( -maxAccel, min( maxDecel, motorLVel_Dropout - motorLVel_Filt ) );  // Rate Limit on Negative direction
      if (motorLVel_next < 0 || ok2ChangeDir_L) { // not changing direction OR ok2ChangeDir
        motorLVel_Filt = motorLVel_next ; // no wait
      } else { // changing direction and waiting for debounce
        motorLVel_Filt = 0 ;
      }
    }

    // rate limit RIGHT motor
    if (motorRForward) { // we are in Forward direction
      motorRVel_next = motorRVel_Filt + min( maxAccel, max( -maxDecel, motorRVel_Dropout - motorRVel_Filt ) ); // Rate Limit on Positive direction
      if (motorRVel_next > 0 || ok2ChangeDir_R) { // not changing direction OR ok2ChangeDir
        motorRVel_Filt = motorRVel_next ; // no wait
      } else { // changing direction and waiting for debounce
        motorRVel_Filt = 0 ;
      }
    } else { // we are in Reverse direction
      motorRVel_next = motorRVel_Filt + max( -maxAccel, min( maxDecel, motorRVel_Dropout - motorRVel_Filt ) );  // Rate Limit on Negative direction
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
