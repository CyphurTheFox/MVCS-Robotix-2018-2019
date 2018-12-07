struct driveMotor {
    tmotor motor;
    tsensor encoder;
    int speed;
    int target;
    int topspeed = 127;
    bool inPos;
}
void apply(driveMotor mo){
    motor[mo.motor] = mo.speed
    return;
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
driveMotor FR = {mFR,encFR, 0, 0};
driveMotor BR = {mBR,encBR, 0, 0};
driveMotor BL = {mBL,encBL, 0, 0};
driveMotor FL = {mFL,encFL, 0, 0};
