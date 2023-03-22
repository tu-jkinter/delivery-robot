
// This class abstracts the control of the robot's motors and exposed public APIs
// to directly control the robot's motion.

#ifndef Movement_h
#define Movement_h

#include <Arduino.h>
#include "Motor.h"
#include "Accelerometer.h"

class Movement {

  private:

    Motor motor;

    //Rotate the front-right wheel
    //True = forward, false = reverse.
    void FrontRightRotate(int speed, bool forward) {
        if (forward) {
            digitalWrite(motor.RightMotorDirPin1, HIGH);
            digitalWrite(motor.RightMotorDirPin2, LOW);
        } else {
            digitalWrite(motor.RightMotorDirPin1, LOW);
            digitalWrite(motor.RightMotorDirPin2, HIGH);
        }

        analogWrite(motor.SpeedPinRightFront, speed);
    }

    //Rotate the front-left wheel
    //True = forward, false = reverse.
    void FrontLeftRotate(int speed, bool forward) {
        if (forward) {
            digitalWrite(motor.LeftMotorDirPin1, HIGH);
            digitalWrite(motor.LeftMotorDirPin2, LOW);
        } else {
            digitalWrite(motor.LeftMotorDirPin1, LOW);
            digitalWrite(motor.LeftMotorDirPin2, HIGH);
        }

        analogWrite(motor.SpeedPinLeftFront, speed);
    }

    //Rotate the back-right wheel
    //True = forward, false = reverse.
    void BackRightRotate(int speed, bool forward) {
        if (forward) {
            digitalWrite(motor.RightMotorDirPin1B, HIGH);
            digitalWrite(motor.RightMotorDirPin2B, LOW);
        } else {
            digitalWrite(motor.RightMotorDirPin1B, LOW);
            digitalWrite(motor.RightMotorDirPin2B, HIGH);
        }

        analogWrite(motor.SpeedPinRightBack, speed);
    }

    //Rotate the back-left wheel
    //True = forward, false = reverse.
    void BackLeftRotate(int speed, bool forward) {
        if (forward) {
            digitalWrite(motor.LeftMotorDirPin1B, HIGH);
            digitalWrite(motor.LeftMotorDirPin2B, LOW);
        } else {
            digitalWrite(motor.LeftMotorDirPin1B, LOW);
            digitalWrite(motor.LeftMotorDirPin2B, HIGH);
        }

        analogWrite(motor.SpeedPinLeftBack, speed);
    }

    //Turn the robot to the left
    void CounterClockwise(int speed) {
        FrontRightRotate(speed, true);
        BackRightRotate(speed, true);
        FrontLeftRotate(speed, false);
        BackLeftRotate(speed, false);
    }

    //turn the robot to the right
    void Clockwise(int speed) {
        FrontRightRotate(speed, false);
        BackRightRotate(speed, false);
        FrontLeftRotate(speed, true);
        BackLeftRotate(speed, true);
    }

    //Shift the robot to the right
    void ShiftRight(int speed) {
        FrontRightRotate(speed, false);
        FrontLeftRotate(speed, true);
        BackRightRotate(speed, true);
        BackLeftRotate(speed, false);
    }

    //Shift the robot to the left
    void ShiftLeft(int speed) {
        FrontRightRotate(speed, true);
        FrontLeftRotate(speed, false);
        BackRightRotate(speed, false);
        BackLeftRotate(speed, true);
    }

    ///Shift the robot right-forward
    void ShiftRightForward(int speed) {
        FrontRightRotate(0, true);
        FrontLeftRotate(speed, true);
        BackRightRotate(speed, true);
        BackLeftRotate(0, true);
    }

    ///Shift the robot right-reverse
    void ShiftRightReverse(int speed) {
        FrontRightRotate(speed, false);
        FrontLeftRotate(0, false);
        BackRightRotate(0, false);
        BackLeftRotate(speed, false);
    }

    //Shift the robot left-forward
    void ShiftLeftForward(int speed) {
        FrontRightRotate(speed, true);
        FrontLeftRotate(0, true);
        BackRightRotate(0, true);
        BackLeftRotate(speed, true);
    }

    //Shift the robot left-reverse
    void ShiftLeftReverse(int speed) {
        FrontRightRotate(0, false);
        FrontLeftRotate(speed, false);
        BackRightRotate(speed, false);
        BackLeftRotate(0, false);
    }

    //Rotate the robot right in an arc with its back as the axis of rotation
    void ArcRight(int speed) {
        FrontRightRotate(speed, false);
        FrontLeftRotate(speed, true);
        BackRightRotate(0, true);
        BackLeftRotate(0, true);
    }

    //Rotate the robot left in an arc with its back as the axis of rotation
    void ArcLeft(int speed) {
        FrontRightRotate(speed, true);
        FrontLeftRotate(speed, false);
        BackRightRotate(0, true);
        BackLeftRotate(0, true);
    }


  
  public:

    Accelerometer Accelerometer;
    
    //Initialize Pins
    void Initialize() {
        //Speed pins
        pinMode(motor.SpeedPinRightFront, OUTPUT);
        pinMode(motor.SpeedPinLeftFront, OUTPUT);
        pinMode(motor.SpeedPinRightBack, OUTPUT);
        pinMode(motor.SpeedPinLeftBack, OUTPUT);

        //Direction pins
        pinMode(motor.RightMotorDirPin1, OUTPUT);
        pinMode(motor.RightMotorDirPin2, OUTPUT);

        pinMode(motor.LeftMotorDirPin1, OUTPUT);
        pinMode(motor.LeftMotorDirPin2, OUTPUT);

        pinMode(motor.RightMotorDirPin1B, OUTPUT);
        pinMode(motor.RightMotorDirPin2B, OUTPUT);

        pinMode(motor.LeftMotorDirPin1B, OUTPUT);
        pinMode(motor.LeftMotorDirPin2B, OUTPUT);

        Accelerometer.Initialize();
        Accelerometer.UpdateValues();
    }

    //Stop all motors
    void Stop() {
        analogWrite(motor.SpeedPinRightFront, 0);
        analogWrite(motor.SpeedPinLeftFront, 0);
        analogWrite(motor.SpeedPinRightBack, 0);
        analogWrite(motor.SpeedPinLeftBack, 0);
    }

    //Move robot forward
    void Forward(int speed) {
        FrontRightRotate(speed, true);
        FrontLeftRotate(speed, true);
        BackRightRotate(speed, true);
        BackLeftRotate(speed, true);
    }

    //Move robot in reverse
    void Reverse(int speed) {
        FrontRightRotate(speed, false);
        FrontLeftRotate(speed, false);
        BackRightRotate(speed, false);
        BackLeftRotate(speed, false);
    }

    //Rotate the robot in the designated direction
    //true = clockwise, false = counterclockwise
    void Rotate(int speed, bool clockwise) {
        if (clockwise) {
            Clockwise(speed);
        } else {
            CounterClockwise(speed);
        }
    }

    void Rotate(int speed, bool clockwise, float angle) {
        
        //Limit rotation to 180% turns
        if (angle >= 180)
        {
          angle = 179;
        }

        Accelerometer.Flush();
        float currentAngle = Accelerometer.GetAngle().Z;
        float targetAngle = CalculateTargetZAngle(currentAngle, angle, clockwise);
        bool isComplete = false;

        Rotate(speed, clockwise);

        while (!isComplete)
        {
          Accelerometer.UpdateValues();
          currentAngle = Accelerometer.GetAngle().Z;

          if (abs(targetAngle - currentAngle) < 2)
          {
            isComplete = true;
            Stop();
          }
        }
    }
    
    //Calculate the angle to stop at 
    float CalculateTargetZAngle(float currentAngle, float moveAngle, bool clockwise) {
        
        float targetAngle = (clockwise) ? currentAngle + moveAngle : currentAngle - moveAngle;

        if (targetAngle >= 360) {
          targetAngle -= 360;
        }

        if (targetAngle < 0) {
          targetAngle += 360;
        }
        
        return targetAngle;
    }

    //Shift the robot in a direction relative to its forward-facing position.
    // 0 (default): Forward, 1: ForwardRight, 2: Right, 3: ReverseRight, 4: Reverse, 5: ReverseLeft, 6: Left, 7: ForwardLeft
    void Shift(int speed, int direction) {
        switch (direction) {
            case 1:
                ShiftRightForward(speed);
                break;
            case 2:
                ShiftRight(speed);
                break;
            case 3:
                ShiftRightReverse(speed);
                break;
            case 4:
                Reverse(speed);
                break;
            case 5:
                ShiftLeftReverse(speed);
                break;
            case 6:
                ShiftLeft(speed);
                break;
            case 7:
                ShiftLeftForward(speed);
                break;
            default:
                Forward(speed);
                break;
        }
    }

    //Arc the robot in the designated direction using its back as the axis of rotation.
    void Arc(int speed, bool clockwise) {
        if (clockwise) {
            ArcRight(speed);
        } else {
            ArcLeft(speed);
        }
    }
};

#endif