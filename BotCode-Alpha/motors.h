struct driveMotor {
    tMotor motorm;
    tSensors encoder;
    int speed;
    int target;
    int topspeed;
    bool inPos;
}
struct driveMotor FR;
struct driveMotor BR;
struct driveMotor BL;
struct driveMotor FL;
void apply(driveMotor mo){
    motor[mo.motorm] = mo.speed;
    return;
}
