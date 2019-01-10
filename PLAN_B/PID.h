#define sampleTime 100

typedef struct pidController {
    float *Input;
    float *Output;
    float *Setpoint;
    float kp;
    float ki;
    float kd;
    int outMin;
    int outMax;
    float lastInput;
    float ITerm;
    int lastTime;
    bool inAuto;
}pidController;


/*   PID CONTROLLER DEFINITION SYNTAX
 *   (Input, Output, and Setpoint Variables need to be defined externally)
 *
 *   pidController <name> = {&<Input>, &<Output>, &<Setpoint>, Kp, Ki, Kd, outMin, outMax}
 */
#define MANUAL 0
#define AUTOMATIC 1

void Compute(pidController PID)
{
    if(!PID.inAuto) return;
    unsigned long now = time1[T2];
    int timeChange = (now - PID.lastTime);
    if(timeChange>=sampleTime)
    {
        /*Compute all the working error variables*/
        float error = *PID.Setpoint - *PID.Input;
        PID.ITerm+= (PID.ki * error);
        if(PID.ITerm > PID.outMax) PID.ITerm= PID.outMax;
        else if(PID.ITerm< PID.outMin) PID.ITerm= PID.outMin;
        float dInput = (*PID.Input - PID.lastInput);
        
        /*Compute PID Output*/
        *PID.Output = PID.kp * error + PID.ITerm - PID.kd * dInput;
        if(*PID.Output > PID.outMax) *PID.Output = PID.outMax;
        else if(*PID.Output < PID.outMin) *PID.Output = PID.outMin;
        
        /*Remember some variables for next time*/
        PID.lastInput = *PID.Input;
        PID.lastTime = now;
    }
}

void SetTunings(pidController PID, float Kp, float Ki, float Kd)
{
    double SampleTimeInSec = sampleTime/1000;
    PID.kp = Kp;
    PID.ki = Ki;
    PID.kd = Kd;
}

void SetOutputLimits(pidController PID, float Min, float Max)
{
    if(Min > Max) return;
    PID.outMin = PID.Min;
    PID.outMax = PID.Max;
    
    if(*PID.Output > PID.outMax) *PID.Output = PID.outMax;
    else if(*PID.Output < PID.outMin) *PID.Output = PID.outMin;
    
    if(PID.ITerm> PID.outMax) PID.ITerm= PID.outMax;
    else if(PID.ITerm< PID.outMin) PID.ITerm= PID.outMin;
}

void SetMode(pidController PID, int Mode)
{
    bool newAuto = (Mode == AUTOMATIC);
    if(newAuto && !PID.inAuto)
    {  /*we just went from manual to auto*/
        Initialize(PID);
    }
    PID.inAuto = newAuto;
}

void Initialize(pidController PID)
{
    PID.lastInput = *PID.Input;
    PID.ITerm = *PID.Output;
    if(PID.ITerm > PID.outMax) PID.ITerm= PID.outMax;
    else if(PID.ITerm< PID.outMin) PID.ITerm= PID.outMin;
}
