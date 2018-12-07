//RoboCode-Alpha v1.1.1


//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//


/*
Port1: LiftMotor2 (Can be combined with Prt. 6 in necessity

Port2: Rear Left Drive

Port3: Rear Right Drive

Port4: Intake Motor

Port5: Front Left Drive

Port6: Front Right Drive

Port7: ClawMotor

Port8: Firing Motors (Splitterx2 + Power Expander)

Port9: LiftMotor1

Port10: Platform Boost system (?)

*/
int mabs(int a1) {
    return (a1 < 0 ? -a1 : a1);
}
// ----------------------------------------------IMPORTANT---------------------------------------------
// USE SEPARATE TASKS FOR EVERYTHING. IT ACCOMPLISHES THE SAME THING AS PUTTING EVERYTHING IN MAIN,
// BUT IT'S MORE ORGANIZED AND HELPS DEBUG
//-----------------------------------------------IMPORTANT---------------------------------------------

#include "controllerDefinitions.h"
#include "RoboCode-Lift.h"
#include "RoboCode-Shooter.h"
//Config
#define adjustmentThreshhold 0.04

//Internal Use Variables
bool flywheelRunning;
int flySpeed = 500;
signed int flyModifier = 0;
int blinker = 0;
int revFR;
int revFL;
int revBR;
int revBL;
int armTarget;
int liftTarget;
int headFlip = 1;
int sens;
bool liftOff;
bool armOff;
bool secondary = false;
bool liftClawControl;
bool armOff;
bool moved;
const char assignmentOrder[4] = {'D','L','U','R'};

int rmt(char *a){ //takes advantage of some research on vexRT[]
    int z = 0;
    int b = 0;
    int j;
    switch (a[0]) {
        case 'C':
            if (secondary){
                z += 6;
            }
            z += (a[1] - '0')-1;
            break;
        default:
            b = a[0] - '0';
            if(b > 8)
                return 0;
            z += 14;
            if(secondary)
                z += 12;
            if(b <= 6){
                if (a[1] == 'U')
                    ++z;
                z += ((b-5)*2);
            } else {
                for(j = 0; !( assignmentOrder[j] == a[1]|| j == 5); ++j){
                    ++z;
                }
                if (j == 5)
                    return 0;
                z += (b == 8 ? 1 : 2)*4;
            }
            break;
    }
    return vexRT[z];
}


/*
void applyMotorSpeed(int FRi, int BRi, int BLi, int FLi){
    float ratios [4][2];
    int *lo;
    int *loSens;

    motor[FR] = FRi;
    motor[BR] = BRi;
    motor[BL] = BLi;
    motor[FL] = FLi;
    if (FRi < BRi && FRi < BLi && FRi < FLi){
        lo = &FRi;
        loSens = &revFR;
    } else if (BRi < BLi && BRi < FLi){
        lo = &BRi;
        loSens = &revBR;
    } else if (BLi < FLi){
        lo = &BLi;
        loSens = &revBL;
    } else {
        lo = &FLi;
        loSens = &revFL;
    }
    ratios[0][0] = FRi / *lo;
    ratios[1][0] = BRi / *lo;
    ratios[2][0] = BLi / *lo;
    ratios[3][0] = FLi / *lo;
    wait1Msec(3);
    for(int i = 0; i < 5; ++i){
        ratios[0][1] = ratios[0][0] - (revFR / *loSens);
        ratios[1][1] = ratios[1][0] - (revBR / *loSens);
        ratios[2][1] = ratios[2][0] - (revBL / *loSens);
        ratios[3][1] = ratios[3][0] - (revFL / *loSens);
        if(abs(ratios[0][1]) > adjustmentThreshhold){
            motor[mFR] += 5*sgn(ratios[0][1]);
        }
        if(abs(ratios[1][1]) > adjustmentThreshhold){
            motor[mBR] += 5*sgn(ratios[1][1]);
        }
        if(abs(ratios[2][1]) > adjustmentThreshhold){
            motor[mBL] += 5*sgn(ratios[2][1]);
        }
        if(abs(ratios[3][1]) > adjustmentThreshhold){
            motor[mFL] += 5*sgn(ratios[3][1]);
        }
        wait1Msec(3);
    }
}
*/
task controllerSwitch(){
    while(true){
        if (vexRT[Btn8RXmtr2]){
            while(vexRT[Btn8RXmtr2]){wait1Msec(1);}
            secondary  = !secondary;
        }
    }
}
task drivetrain(){ //Drivetrain Task. Joshua's code.
    while(true){
        FR.speed = (headFlip * (-vexRT[Ch4] + vexRT[Ch3])) - vexRT[Ch1]; //Determines motor speeds. Joshua's Code.
        FL.speed = (headFlip * (-vexRT[Ch4] - vexRT[Ch3])) - vexRT[Ch1];
        BL.speed = (headFlip * (vexRT[Ch4] - vexRT[Ch3])) - vexRT[Ch1];
        BR.speed = (headFlip * (vexRT[Ch4] + vexRT[Ch3])) - vexRT[Ch1];
        if (mabs(vexRT[Ch4]) < 20 && mabs(vexRT[Ch3]) < 20 && mabs(vexRT[Ch1]) < 30){ //Checks if joystick is in deadzone
            FR.speed = FL.speed = BR.speed = BL.speed = 0;                                                                                                    //if so, sets motor speeds to 0
        }
        if(sqrt((vexRT[Ch4]*vexRT[Ch4]) + (vexRT[Ch3]*vexRT[Ch3])) < 120 && sqrt((vexRT[Ch1]*vexRT[Ch1]) + (vexRT[Ch2]*vexRT[Ch2])) < 120){    //if Joystick not at extremes
            FR.speed = FR.speed/2;      //halfSpeed
            FL.speed = FL.speed/2;
            BR.speed = BR.speed/2;
            BL.speed = BL.speed/2;
            }
        apply(FR);  //Applies Motor Speeds
        apply(FL);
        apply(BR);
        apply(BL);
        if(flipHead){                           //tests for button to flip robot head
            while (flipHead) { wait1Msec(1);}
            headFlip *= -1;                         //flips head
        }
        EndTimeSlice(); //tells task handler is done
    }
}
/*
task RPMTrack(){
	int FRi,FLi,BRi,BLi;
	while(true){
		FRi = SensorValue[encFR];       //find initial sensor values
		FLi = SensorValue[encFL];
		BLi = SensorValue[encBL];
		BRi = SensorValue[encBR];
		wait1Msec(2);                   // wait 2 ms
		FR.speed = 500*(FRi - SensorValue[encFR]);  //determine rotational speed (Deg/s)
		FL.speed = 500*(FLi - SensorValue[encFL]);
		BL.speed = 500*(BLi - SensorValue[encBL]);
		BR.speed = 500*(BRi - SensorValue[encBR]);
        EndTimeSlice();
	}
}*/
void calc(driveMotor the){
    the.speed = the.target-SensorValue[the.encoder];
    the.speed = the.speed > the.topspeed ? the.topspeed : the.speed;
    if (the.speed < driveTolerance){
        the.speed = 0;
        the.inPos = true;
    } else {
        the.inPos = false;
    }
    return;
}
task moveMotors(){
    moved = false;
    do{
        calc(FR);
        calc(BR);
        calc(BL);
        calc(FL);
        apply(FR);
        apply(BR);
        apply(BL);
        apply(FL);
        EndTimeSlice();
    }while(!(FR.inPos && BR.inPos && BL.inPos && FL.inPos));
    moved = true;
}


void Start()
{
    startTask(drivetrain);
    startTask(ballGrabber);
    flywheelRunning = false;
    startTask(flywheelToggle);
    startTask(flySpeedAdjuster);
    if(liftClawControl){
        startTask(liftClawControllerInterface);
    } else {
        startTask(liftClawInterfaceSimple);
    }
    startTask(cascadeClawManual);
	startTask(autoLift);
	startTask(autoClaw);
	startTask(LEDControl);
    if( nVexRCReceiveState & 0x02 ){
        startTask(controllerSwitch);
    }
}
