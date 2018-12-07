bool moved;
struct driveMotor {
    tMotor motorm;
    tSensors encoder;
    int speed;
    int target;
    int topspeed;
    bool inPos;
}
int sign(int input) {
	return input < 0 ? -1 : 1;
}
struct driveMotor FR;
struct driveMotor BR;
struct driveMotor BL;
struct driveMotor FL;

void calc(driveMotor the){
    the.speed = the.target-SensorValue[the.encoder];
    the.speed = abs(the.speed) > the.topspeed ? the.topspeed*sign(the.speed) : the.speed;
    if (abs(the.speed) < 10){
        the.speed = 0;
        the.inPos = true;
    } else {
        the.inPos = false;
    }
    return;
}

void apply(driveMotor mo){
    motor[mo.motorm] = mo.speed;
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
    }while(!FR.inPos || !BR.inPos || !BL.inPos || !FL.inPos);
    moved = true;
}

void translate(int x,int y){  //translate robot relative to robot head
    x *= 360;
    y *= 360;
    FR.target += (-y + x);
    BR.target += (-y - x);
    BL.target += (y - x);
    FL.target += (y + x);
    startTask(moveMotors);
    while(!moved){wait1Msec(10);}
    return;
}
void rotate(int t){
    t *= -360;
    FR.target += t;
    BR.target += t;
    BL.target += t;
    BR.target += t;
    startTask(moveMotors);
    while(!moved){wait1Msec(10);}
    return;
}
