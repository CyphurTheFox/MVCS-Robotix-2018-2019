bool liftToTop = false;
bool liftToBottom = true;
task cascadeClawManual(){  //control cascade lift and claw manually
    while(true){
        if(cascadeU){
            liftToTop = false;
            liftToBottom = false;
            while (cascadeU) {wait1Msec(10);}
        }
        if(cascadeD){
            liftToTop = false;
            liftToBottom = false;
            while (cascadeD) {wait1Msec(10);}
        }
        if(clawU){
            armTarget = Lifted;
            while (clawU) {wait1Msec(10);}
            armTarget = SensorValue[Claw]/10;
        }
        if(clawD){
            armTarget = Down;
            while (clawD) {wait1Msec(10);}
            armTarget = SensorValue[Claw]/10;
        }
        EndTimeSlice();
    }
}

task autoClaw(){  //handle Claw Movement
    armTarget = SensorValue[Claw]/10;  //set target to current position to prevent early movement
    while (true) {
        sens = SensorValue[Claw]/10;    //determine current claw position
        armOff = abs(sens-armTarget) > 50;
        if (!armOff){
            clearTimer(T2);
        } else if (time1[T2] > 3000){
            armTarget = sens;
        }
        motor[mCLW]= (armTarget-sens);
        EndTimeSlice(); //tell task handler done
    }
}

/*
 The function works in four steps, then passes the identifier, herin known as "var" to the switch statement.

 1: if the lift is in top position, then set var to 6, else set it to three;
 2: if lift is in bottom position, then set var to 0 (subtract 3), else do nothing.
 effect: if lift is at top, var will be 6, if bottom: 0, if middle: 3

 3: if lift is set to go to top position or upwards, then add one to var
 4: if lift is set to go down, subtract one from var

 effect:    if lift is not set to move, var will be 0, 3, or 6
            if lift is set to move upwards and can, var will be 1 or 4
            if lift is set to move down and can, var will be 2 or 5
            if lift is being told to go to both up and down at the same time, it will do neither. (var = 0, 3, or 6)
switch statement will then act on this math.
*/

task autoLift(){ //Task to move Lift into postition
    while (true) {
        switch (
        /*1:*/  (SensorValue[limitLiftTop] == 1 ? 6 : 3)+
        /*2:*/  (SensorValue[limitLiftBottom] == 1 ? -3 : 0)+
        /*3:*/  (cascadeU || liftToTop ? 1 : 0)+
        /*4:*/  (liftToBottom || cascadeD ? -1 : 0)
                )
        {  //see above for how this math works
            case 1:
            case 4:
                motor[mLFT] = 127;
                break;
            case 2:
            case 5:
                motor[mLFT] = -127;
                break;
            default:
                motor[mLFT] = 0;
                clearTimer(T1);
                break;
        }
        if(time1[T1] > 10500 && (liftToTop || liftToBottom)) //Motor overextention catch
            liftToTop = liftToBottom = false;
    }
}

task liftClawInterfaceSimple(){
    while (true) {
        if (clawToggle) {                         //if button for claw pressed
            while (clawToggle) {wait1Msec(1);}     //wait until un-pressed
            switch (armTarget) {                    //switch case:
                case Down:                          //if target is set to down position
                    armTarget = Lifted;             //set target to lifted position
                    break;

                default:                            //else
                    armTarget = Down;               //set target to down position
                    break;
            }
        }
        if(cascadeBottom){
            while(cascadeBottom || cascadeD){wait1Msec(10);}
            liftToBottom = true;
            liftToTop = false;
        }
        if(groundFlip){
            while(groundFlip){wait1Msec(10);}
            armTarget = gndFlip;
            do{wait1Msec(5);} while(armOff);
            armTarget = Down;
        }
        if(cascadeTop){
            while(cascadeTop || cascadeU){wait1Msec(10);}
            liftToBottom = false;
            liftToTop = true;
        }
        EndTimeSlice();
    }
}
