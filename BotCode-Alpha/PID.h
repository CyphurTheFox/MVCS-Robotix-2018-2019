struct pidController {
    float input*;
    float output*;
    float setpoint*;
    float kp;
    float ki;
    float kd;
    bool frozen;
    bool lastState;
    float lastError
    float errSum;
    int lastRun;
}

void compute(pidController PID){

}
