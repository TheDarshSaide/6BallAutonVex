/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       thedarshsaide                                             */
/*    Created:      4/5/2024, 11:22:25 AM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/
motor RightMotor1{PORT1,false};
motor RightMotor2{PORT2,false};
motor RightMotor3{PORT3,false};
motor LeftMotor1{PORT4,true};
motor LeftMotor2{PORT5,true};
motor LeftMotor3{PORT6,true};

motor intake(PORT7,false);



motor_group RightMotors {LeftMotor1,LeftMotor2,LeftMotor3};
motor_group LeftMotors {RightMotor1,RightMotor2,RightMotor3};

drivetrain Drive{LeftMotors,RightMotors};

controller Controller1 = controller(primary);
void pre_auton(void) {
  intake.setVelocity(200,percent);

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
void Intake(void){
  intake.spin(forward);
  while (Controller1.ButtonR1.pressing()){
    wait(10,msec);
  }
  intake.stop();
}

void Outake(void){
  intake.spin(reverse);
  while (Controller1.ButtonR2.pressing()){
    wait(10,msec);
  }
  intake.stop();
}
//Settings
double kP = 0.0005;
double kI = 0.0;
double kD = 0.0003;

double turnkP = 0.6;
double turnkI = 0.0;
double turnkD = 0.6;
/// Autounomous Settings
int desiredValue = 200;
int desiredTurnValue = 200;

int error; //sensor value - Desired Value  : positional value
int prevError = 0;// postion 20 msecs ago
int derivative; //difference beetween error and prev error (calcs sped)
int totalError = 0;

int turnError; //sensor value - Desired Value  : positional value
int turnPrevError = 0;// postion 20 msecs ago
int turnDerivative; //difference beetween error and prev error (calcs sped)
int turnTotalError = 0;


//varibles modified for use
bool enableDrivePID = false;

int drivePID(){
  while(enableDrivePID){
    //get position of both motors
    int LeftMotorPosition = LeftMotors.position(degrees);
    int RightMotorPosition = RightMotors.position(degrees);
    
    //lateral movement pid

    //get average of both motors
    int averagePosition = (LeftMotorPosition + RightMotorPosition)/2;
    
    //Potential
    error = averagePosition - desiredValue;
    
    // Derivative
    derivative = error-prevError;

    totalError += error;
    
    double lateralMotorPower = ((error* kP + derivative * kD)*50)/6;
    
    
    
    // turning movement PID
     //get position of both motors
    int TurnLeftMotorPosition = LeftMotors.position(degrees);
    int TurnRightMotorPosition = RightMotors.position(degrees);

    //get average of both motors
    int averageTurnPosition = (TurnLeftMotorPosition + TurnRightMotorPosition)/2;
    
    //Potential
    turnError = averageTurnPosition - desiredTurnValue;
    
    // Derivative
    turnDerivative = turnError-turnPrevError;

    turnTotalError += turnError;
    
    double turnMotorPower = ((turnError* turnkP + turnDerivative * turnkD)*50)/6;
    
    
    Drive.setDriveVelocity(lateralMotorPower,percent);
    Drive.setDriveVelocity(turnMotorPower,percent);
    //code
    prevError = error;
    turnPrevError = turnError;
    vex::task::sleep(20);
  }
  
  
  return 1;
}
void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
  enableDrivePID = true;
  vex::task zuperman(drivePID);
  Drive.driveFor(forward,25,inches);
  Drive.turnFor(right,20,degrees);
  

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

void usercontrol(void) {
  enableDrivePID = false;
  Drive.setDriveVelocity(100,percent);

  // User control code here, inside the loop
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................
   Drive.arcade(Controller1.Axis3.value(), Controller1.Axis1.value());
    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  Controller1.ButtonR1.pressed(Intake);
  Controller1.ButtonR2.pressed(Outake);
  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
