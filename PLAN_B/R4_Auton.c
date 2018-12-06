#pragma config(Sensor, in2,    potClaw,        sensorPotentiometer)
#pragma config(Sensor, dgtl1,  encLift,        sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  LED,            sensorLEDtoVCC)
#pragma config(Sensor, dgtl5,  encFR,          sensorQuadEncoder)
#pragma config(Sensor, dgtl7,  encBL,          sensorQuadEncoder)
#pragma config(Sensor, dgtl9,  encBR,          sensorQuadEncoder)
#pragma config(Sensor, dgtl11, encFL,          sensorQuadEncoder)
#pragma config(Motor,  port1,           mFL,           tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           mBL,           tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           mBR,           tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           mIntake,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           mLift,         tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           mFR,           tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           mClaw,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           mFlyWheelR,    tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port9,           mFlyWheelL,    tmotorVex393HighSpeed_MC29, openLoop)

/*
goFoward function doc
	direction can be one of the four: 12, 3, 6, 9
		let flyWheel be 12 o'clock
		lift facing 6 o'clock, etc

		int mListFL[13] = {0, 0, 0, 1, 0, 0, 1, 0, 0, -1, 0, 0, -1};
int mListFR[13] = {0, 0, 0, 1, 0, 0, -1, 0, 0, -1, 0, 0, 1};
int mListBL[13] = {0, 0, 0, -1, 0, 0, 1, 0, 0, 1, 0, 0, -1};
int mListBR[13] = {0, 0, 0, -1, 0, 0, -1, 0, 0, 1, 0, 0, 1};
*/
int mList[4][13] = // 0: FL, 1: FR, 2: BL, 3: BR
{{0, 0, 0, 1, 0, 0, 1, 0, 0, -1, 0, 0, -1},
{0, 0, 0, 1, 0, 0, -1, 0, 0, -1, 0, 0, 1},
{0, 0, 0, -1, 0, 0, 1, 0, 0, 1, 0, 0, -1},
{0, 0, 0, -1, 0, 0, -1, 0, 0, 1, 0, 0, 1}};

void resetEncoder () {
	SensorValue[encFL] = SensorValue[encFR] = SensorValue[encBL] = SensorValue[encBR] = 0;
}
void goFoward (int direction, int distance) {
	if (mList[0][direction] == 0) {
		return;
	}
	while (true) {
		motor[mFL] = mList[0][direction] * 127;
		motor[mFR] = mList[1][direction] * 127;
		motor[mBL] = mList[2][direction] * 127;
		motor[mBR] = mList[3][direction] * 127;
	}
}

task main()
{

}
