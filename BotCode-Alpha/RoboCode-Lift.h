
/* ------------------------IMPORTANT-----------------------------*/
/* USE SEPARATE TASKS FOR EVERYTHING,                            */
/* IT ACCOMPLISHES THE SAME THING AS PUTTING EVERYTHING IN MAIN, */
/* BUT IT'S MORE ORGANIZED AND HELPS DEBUG                       */
/*-------------------------IMPORTANT-----------------------------*/



task cascadeClawManual(){  //control cascade lift and claw manually
	while(true){
		if(cascadeU){
            liftTarget = liftTop;
            while (cascadeU) {wait1Msec(10);}
            liftTarget = SensorValue[quadLift];
		}
        if(cascadeD){
            liftTarget = liftBottom;
            while (cascadeD) {wait1Msec(10);}
            liftTarget = SensorValue[quadLift];
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
    int diff;
    int movMod;
    armTarget = SensorValue[Claw]/10;  //set target to current position to prevent early movement
    while (true) {
        sens = SensorValue[Claw]/10;    //determine current claw position
        armOff = mabs(sens-armTarget) > 10;
        motor[mCLW]= 1.5*(armTarget-sens);
        EndTimeSlice(); //tell task handler done
    }
}


task autoLift(){ //Task to move Lift into postition
    int sensLift;
    SensorValue[quadLift] = 0;                  //initialize the quad encoder to 0;
    liftTarget = SensorValue[quadLift];      //set target to current position to prevent unwanted movement
    while (true) {
        sensLift = SensorValue[quadLift];    // determine current lift position
        liftOff = mabs(sensLift-liftTarget) > 1;
        motor[mLFT]= 2.5*(liftTarget-sensLift);
        EndTimeSlice();            //tell task handler done
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
                    wait1Msec(500);
                    liftTarget = liftBottom;
                    break;
            }
        }
        if(cascadeDown){
            liftTarget = liftBottom;
        }
        if(groundFlip){
            while(groundFlip){wait1Msec(1);}
            armTarget = 2*(Lifted/3);
            while(armOff){wait1Msec(1);}
            armTarget = Down;
        }
        EndTimeSlice();
    }
}
/*

task liftClawControllerInterface(){
    startTask(autoLift);  //ensure position monitors already running
    startTask(autoClaw);
    while (true) {
        if (vexRT[Btn7D]) {                         //if button for lift pressed
            while (vexRT[Btn7D]) {wait1Msec(1);}     //wait until button un-pressed
            switch (liftTarget) {                   //switch case:
                case liftBottom:                    //if target is set to bottom
                    liftTarget = liftMiddle;        //set target to middle
                    break;

                case liftMiddle:                    //if target set to middle
                    liftTarget = liftTop;           //set target to top
                    break;

                default:                            //else
                    liftTarget = liftBottom;        //set target to bottom
                    break;
            }
        }
        if (vexRT[Btn7L]) {                         //if button for claw pressed
            while (vexRT[Btn7L]) {wait1Msec(1);}     //wait until un-pressed
            switch (armTarget) {                    //switch case:
                case Down:                          //if target is set to down position
                    armTarget = Lifted;             //set target to lifted position
                    break;

                default:                            //else
                    armTarget = Down;               //set target to down position
                    wait1Msec(500);
                    liftTarget = liftBottom;
                    break;
            }
        }
        if(vexRT[Btn7R] && liftTarget < 190 && liftTarget > 50 && clawTarget == Down){    //if cap flip activated and lift is in position
            while(vexRT[Btn7R]){wait1Msec(1);}
            liftTarget += removeMod;                //move lift up my predetermined value
            do{wait1Msec(2);} while (liftOff);      //wait until in position
            armTarget = Lifted;                     //set move arm to up position
            liftTarget += 10;                       //move lift up a little more
            do{wait1Msec(2);}while(liftOff);        //wait until in position
            liftTarget += -(removeMod+10);          //move lift back down
            do{wait1Msec(2);}while(liftOff);        //wait unti in position
            armTarget = Down;                       //move arm down
            wait1Msec(500);                         //wait 500 ms
            liftTarget = liftBottom;                //move lift down
        }
        EndTimeSlice();
    }
}*/
