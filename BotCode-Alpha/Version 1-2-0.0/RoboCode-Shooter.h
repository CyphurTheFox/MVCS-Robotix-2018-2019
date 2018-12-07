
/* ------------------------IMPORTANT-----------------------------*/
/* USE SEPARATE TASKS FOR EVERYTHING,                            */
/* IT ACCOMPLISHES THE SAME THING AS PUTTING EVERYTHING IN MAIN, */
/* BUT IT'S MORE ORGANIZED AND HELPS DEBUG                       */
/*-------------------------IMPORTANT-----------------------------*/



task ballGrabber(){ //Ball intake system code
    while(true){
        if (intakeI) { //if intake button pressed...
            motor[mGRB] = 127; //run intake motor
            } else if (intakeO) { //if reverse button Pressed
            motor[mGRB] = -127; // reverse
            } else {
            motor[mGRB] = 0; //else stop
        }
        EndTimeSlice(); //tell task handler done
    }
}


task LEDControl(){      //task to turn LED on & off
    while(true){
        if(flySpeed < 127){                 //if flywheel is in secondary state
            while(flySpeed < 127){          //while it's in secondary state
                if(flyModifier > 0){        //if the modifier is positive
                    blinker = 300;          //set blinking delay to 300
                } else if(flyModifier < 0){ //if modifier is negative
                    blinker = 1000;         //set delay to 1000
                } else  {                   //else (it's 0)
                    while(flyModifier == 0 && flySpeed < 127){  //while modifier is 0
                        turnLEDOn(LED);                         //keep LED on
                        EndTimeSlice();                         //tell task handler is done
                    }
                }
                turnLEDOn(LED);     //turn LED on
                wait1Msec(100);     //wait 100 ms
                turnLEDOff(LED);    //turn LED off
                wait1Msec(blinker); //wait for [blinking delay] ms
                EndTimeSlice(); //tell task handler is done
            }
        }else{              //else (flySpeed is in full power)
            turnLEDOff(LED);//turn off LED
            EndTimeSlice(); //tell task handler done
        }
    }
}


task flywheelToggle() { //detects button presses to toggle the flywheel
    while (true) {
        if (toggleFly) { //if toggle button pressed
            flywheelRunning = !flywheelRunning;  //set flywheel running bool to the opposite value
            while (toggleFly) { //wait until button un-pressed
                wait1Msec(1);
            }
        }
        if (flywheelRunning) { // if flywheel set to be on
            motor[mFRE] = flySpeed + (5*flyModifier); //turn on
            motor[mFRE2] = flySpeed + (5*flyModifier);
            } else {
            motor[mFRE] = 0; //else turn off
            motor[mFRE2] = 0;
        }

        EndTimeSlice(); //tell task handler done
    }
}


task flySpeedAdjuster() {       //Adjust the flywheel Speeds
    while(true){
        if (secondarySpeed) {             //if 8L pressed
            while(secondarySpeed){        //wait until not pressed
                wait1Msec(1);
            }
            switch(flySpeed){
            case 500:                   //if flySpeed = 500
                flySpeed = speed1;         //set flySpeed to lv1
                break;
            default:                    //else
                flySpeed = 500;         //set flyspeed to 500
            }
        }
        if (speedReset) {                      //if speed reset buttons
            while(speedReset){wait1Msec(1);}
            flyModifier = 0;                                    //reset modifier
        }else if (speedInc) {                               //if speed increment buttons
            while(speedInc){wait1Msec(1);}
            flyModifier++;                                      //increase modifier
        }else if (speedDec) {                               //if speed decrement buttons
            while(speedDec){wait1Msec(1);}
            flyModifier--;                                      //decrease modifier
        }
    }
}

