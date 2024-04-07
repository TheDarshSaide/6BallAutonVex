/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       thedarshsaide                                             */
/*    Created:      3/28/2024, 5:35:32 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;
\
// define your global instances of motors and other devices here
motor LeftMotor1{PORT1,gearSetting::ratio6_1,false};
motor LeftMotor2{PORT2,gearSetting::ratio6_1,false};
motor LeftMotor3{PORT3,gearSetting::ratio6_1,false};
motor RightMotor1{PORT4,gearSetting::ratio6_1,true};
motor RightMotor2{PORT5,gearSetting::ratio6_1,true};
motor RightMotor3{PORT6,gearSetting::ratio6_1,true};



motor motor1(PORT7,gearSetting::ratio6_1);

motor_group LeftMotors {LeftMotor1,LeftMotor2,LeftMotor3};
motor_group RightMotors {RightMotor1,RightMotor2,RightMotor3};

drivetrain Drive{LeftMotors,RightMotors};

controller Controller = controller(primary);
/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {

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

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}

void motorTest(void){
  motor1.spin(forward);
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
  // User control code here, inside the loop
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................
    Drive.arcade(Controller.Axis3.value(), Controller.Axis1.value());
    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
    Controller.ButtonL1.pressed(motorTest);
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
