//RoboCode-Alpha v2.5.0

#include "controllerDefinitions.h"      //definitions for controller buttons
#include "motors.h"                     //define motor structure and motor related commands

//Internal Use Variables
bool flywheelRunning;
int flySpeed = 500;
signed int flyModifier = 0;
int blinker = 0;
int armTarget;
int headFlip = 1;
int sens;
bool armOff;
bool secondary = false;

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
                switch (a[1]) {
                    case 'R': ++z;
                    case 'U': ++z;
                    case 'L': ++z;
                }
                if (j == 5)
                    return 0;
                z += (b == 8 ? 1 : 2)*4;
            }
            break;
    }
    return vexRT[z];
}
#include "RoboCode-Lift.h"      //include lift driver code
#include "RoboCode-Shooter.h"   //include Shooter code

task controllerSwitch(){        //switch controllers in dual controller layout
    while(true){
        if (vexRT[Btn8RXmtr2]){     //if switch button
            while(vexRT[Btn8RXmtr2]){wait1Msec(1);}
            secondary  = !secondary;    //switch controller
        }
    }
}

task drivetrain(){ //Drivetrain Task. Joshua's code.
    while(true){
        FR.speed = (headFlip * (-vexRT[Ch4] + vexRT[Ch3])) - vexRT[Ch1]; //Determines motor speeds. Joshua's Code.
        FL.speed = (headFlip * (-vexRT[Ch4] - vexRT[Ch3])) - vexRT[Ch1];
        BL.speed = (headFlip * (vexRT[Ch4] - vexRT[Ch3])) - vexRT[Ch1];
        BR.speed = (headFlip * (vexRT[Ch4] + vexRT[Ch3])) - vexRT[Ch1];
        if (abs(vexRT[Ch4]) < 20 && abs(vexRT[Ch3]) < 20 && abs(vexRT[Ch1]) < 30){ //Checks if joystick is in deadzone
            FR.speed = FL.speed = BR.speed = BL.speed = 0;                                                                                                    //if so, sets motor speeds to 0
        }
        if(sqrt((vexRT[Ch4]*vexRT[Ch4]) + (vexRT[Ch3]*vexRT[Ch3])) < 120 && sqrt((vexRT[Ch1]*vexRT[Ch1]) + (vexRT[Ch2]*vexRT[Ch2])) < 120){    //if Joystick not at extremes
            FR.speed /= 2;      //halfSpeed
            FL.speed /= 2;
            BR.speed /= 2;
            BL.speed /= 2;
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

void Start(){
    startTask(drivetrain);
    startTask(ballGrabber);
    flywheelRunning = false;
    startTask(flywheelToggle);
    startTask(flySpeedAdjuster);
    startTask(liftClawInterfaceSimple);
    startTask(cascadeClawManual);
	startTask(autoLift);
	startTask(autoClaw);
	startTask(LEDControl);
    if( nVexRCReceiveState & 0x02 ){
        startTask(controllerSwitch);
    }
}
