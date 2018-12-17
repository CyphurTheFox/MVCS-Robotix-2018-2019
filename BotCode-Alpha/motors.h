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


void apply(driveMotor mo){
    motor[mo.motorm] = mo.speed;
    return;
}
