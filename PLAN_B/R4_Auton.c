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
#pragma config(Motor,  port5,           mLift,         tmotorVex393_MC29k , openLoop, reversed)
#pragma config(Motor,  port6,           mFR,           tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           mClaw,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           mFlyWheelR,    tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port9,           mFlyWheelL,    tmotorVex393HighSpeed_MC29, openLoop)

/*
goFoward function doc
	direction can be one of the four: 12, 3, 6, 9
		let flyWheel be 12 o'clock
		lift facing 6 o'clock, etc
*/
int mListDirection[4][13] = // 0: FL, 1: FR, 2: BL, 3: BR
{{0, 0, 0, 1, 0, 0, 1, 0, 0, -1, 0, 0, -1},
{0, 0, 0, 1, 0, 0, -1, 0, 0, -1, 0, 0, 1},
{0, 0, 0, -1, 0, 0, 1, 0, 0, 1, 0, 0, -1},
{0, 0, 0, -1, 0, 0, -1, 0, 0, 1, 0, 0, 1}};

int mListDrive[13][4] = {{0, 0, 0, 0},
	{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 3, 0, 2},  //  3 o'clock
	{0, 0, 0, 0}, {0, 0, 0, 0}, {3, 2, 1, 0},  //  6 o'clock
	{0, 0, 0, 0}, {0, 0, 0, 0}, {2, 0, 3, 1},  //  9 o'clock
	{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 2, 3}}; // 12 o'clock

int motorPower[4] = {0, 0, 0, 0};
int encoderValues[4];

int encLeft, encRight;
void resetEncoderValues () {
	encLeft = encRight = SensorValue[encFL] = SensorValue[encFR] = SensorValue[encBL] = SensorValue[encBR] = 0;
}
void getEncoderValues (int direction) {
	encoderValues[0] = -SensorValue[encFL] * ;
	encoderValues[1] = -SensorValue[encFR];
	encoderValues[2] =  SensorValue[encBL];
	encoderValues[3] = -SensorValue[encBR];
	encLeft = (encoderValues[mListDrive[direction][0]] + encoderValues[mListDrive[direction][2]]) / 2;
	encRight = (encoderValues[mListDrive[direction][1]] + encoderValues[mListDrive[direction][3]]) / 2;
}

void goFoward (int direction, int distance) {
	if (mListDirection[0][direction] == 0) {
		return;
	}
	resetEncoderValues();
	while ((encLeft + encRight) / 2 < distance) {
		getEncoderValues(direction);
		if (abs(encLeft - encRight) > 20) {
			motorPower[mListDrive[direction][0]] = motorPower[mListDrive[direction][2]] = encLeft > encRight ? 100 : 127;
			motorPower[mListDrive[direction][1]] = motorPower[mListDrive[direction][3]] = encLeft > encRight ? 127 : 100;
		} else {
			motorPower[0] = motorPower[1] = motorPower[2] = motorPower[3] = 127;
		}

		motor[mFL] = mListDirection[0][direction] * motorPower[0];
		motor[mFR] = mListDirection[1][direction] * motorPower[1];
		motor[mBL] = mListDirection[2][direction] * motorPower[2];
		motor[mBR] = mListDirection[3][direction] * motorPower[3];
	}
	motor[mFL] = motor[mFR] = motor[mBL] = motor[mBR] = 0;
}

task main() {
	goFoward (3, 2000);
}
