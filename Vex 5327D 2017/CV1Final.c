#pragma config(Sensor, in1,    potentiometer,  sensorPotentiometer)
#pragma config(Motor,  port2,           baseLeftFront, tmotorVex393HighSpeed_MC29, openLoop, driveLeft)
#pragma config(Motor,  port3,           baseRightFront, tmotorVex393HighSpeed_MC29, openLoop, driveRight)
#pragma config(Motor,  port4,           baseLeftBack,  tmotorVex393HighSpeed_MC29, openLoop, driveLeft)
#pragma config(Motor,  port5,           baseRightBack, tmotorVex393HighSpeed_MC29, openLoop, driveRight)
#pragma config(Motor,  port6,           rightchain,    tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           leftchain,     tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port8,           coneintake,    tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port9,           mogointake,    tmotorVex393HighSpeed_MC29, openLoop)
// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"
#define MOTOR_NUM               kNumbOfTotalMotors  //number of total motors (applied to all)
#define MOTOR_MAX_VALUE         127
#define MOTOR_MIN_VALUE         (-127)
#define MOTOR_DEFAULT_SLEW_RATE 10      // Default will cause 375mS from full fwd to rev (increase 10 every time looped through)
#define MOTOR_FAST_SLEW_RATE    256     // essentially off (need to be changed to between -127 to 127)
#define MOTOR_TASK_DELAY        15      // task 1/frequency in mS (about 66Hz)
#define MOTOR_DEADBAND          10

// Array to hold requested speed for the motors
// This array will have the number of objects equivalent to MOTOR_NUM
// The value in this array will need to be modified in order for the motors to move
int motorReq[ MOTOR_NUM ];

// Array to hold "slew rate" for the motors, the maximum change every time the task
// runs checking current mootor speed.
int motorSlew[ MOTOR_NUM ];
/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the cortex has been powered on and    */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton()
{
  // Set bStopTasksBetweenModes to false if you want to keep user created tasks
  // running between Autonomous and Driver controlled modes. You will need to
  // manage all user created tasks if set to false.
  bStopTasksBetweenModes = true;

	// Set bDisplayCompetitionStatusOnLcd to false if you don't want the LCD
	// used by the competition include file, for example, you might want
	// to display your team name on the LCD in this function.
	// bDisplayCompetitionStatusOnLcd = false;

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

task autonomous()
{
  // ..........................................................................
  // Insert user code here.
  // ..........................................................................

  // Remove this function call once you have "real" code.
  AutonomousCodePlaceholderForTesting();
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

task MotorSlewRateTask()
{
    int unsigned motorIndex; //The index for each motor
    int motorTmp;  //The current motor value for each motor, to be changed to change motor speed

    // Initialize stuff
    for(motorIndex=0;motorIndex<MOTOR_NUM;motorIndex++)
        {
        motorReq[motorIndex] = 0;
        motorSlew[motorIndex] = MOTOR_DEFAULT_SLEW_RATE;
        }

    // run task until stopped
    while( true )
        {
        // run loop for every motor
        for( motorIndex=0; motorIndex<MOTOR_NUM; motorIndex++)
            {
            // So we don't keep accessing the internal storage
            // current motor value
            motorTmp = motor[ motorIndex ];

            // Do we need to change the motor value ?
            if( motorTmp != motorReq[motorIndex] )
                {
                // increasing motor value
                //if motor value in the list is greater than motor value
                if( motorReq[motorIndex] > motorTmp )
                    {
                    motorTmp += motorSlew[motorIndex];
                    // limit
                    //if motor value is greater than motor value in the list
                    if( motorTmp > motorReq[motorIndex] )
                        motorTmp = motorReq[motorIndex];
                    }

                // decreasing motor value
                if( motorReq[motorIndex] < motorTmp )
                    {
                    motorTmp -= motorSlew[motorIndex];
                    // limit
                    if( motorTmp < motorReq[motorIndex] )
                        motorTmp = motorReq[motorIndex];
                    }

                // finally set motor
                motor[motorIndex] = motorTmp;
                }
            }

        // Wait approx the speed of motor update over the spi bus
        wait1Msec( MOTOR_TASK_DELAY );
        }
}

task slewBase ()
{
	/*this task is to move the corresponding value of the motors for the base in motorReq array
	in order to work in unison with the MotorSlewRateTask task*/
  while (true)
  {
  	//Index 1 & 3 are right base motors
		motorReq[ 1 ] = motorReq[ 3 ] = vexRT[Ch2];
  	//Index 0 & 2 are left base motors
		motorReq[ 0 ] = motorReq[ 2 ] = vexRT[Ch3];
		wait1Msec (25); //40Hz
	}
}


task slewChainbar()
{
	while (true)
	{
		if (vexRT[Btn5U] == 1)
		{
			motorReq[ 6 ] = motor[MOTOR_DEFAULT_SLEW_RATE ];
			motorReq[ 7 ] = motor[MOTOR_DEFAULT_SLEW_RATE ];
		}
		if (vexRT[Btn5D] == 1)
		{
			motorReq[ 6 ] = motor[-(MOTOR_DEFAULT_SLEW_RATE)];
			motorReq[ 7 ] = motor[-(MOTOR_DEFAULT_SLEW_RATE)];
		}
	}
}

task slewCone()
{
	while (true)
	{
		if (vexRT[Btn6U] == 1)
		{
			motorReq[ 8 ] = motor[MOTOR_DEFAULT_SLEW_RATE ];
		}
		if (vexRT[Btn6D] == 1)
		{
			motorReq[ 8 ] = motor[-(MOTOR_DEFAULT_SLEW_RATE)];
		}
	}
}

task slewMogo()
{
	while (true)
	{
		if (vexRT[Btn8U] == 1)
		{
			motorReq[ 9 ] = motor[MOTOR_DEFAULT_SLEW_RATE ];
		}
		if (vexRT[Btn8D] == 1)
		{
			motorReq[ 9 ] = motor[-(MOTOR_DEFAULT_SLEW_RATE)];
		}
	}
}

task buzzer {
	if (vexRT[Btn7R]==1)
	{
		playSoundFile("1.wav");
	}
}


task usercontrol()
{
  startTask(MotorSlewRateTask);
	startTask(slewBase); //utilizing slew rate on base
	startTask(slewCone);
	startTask(slewMogo);
	startTask(slewChainbar);//utilizing slew rate on chainbar
	startTask(buzzer);

}
