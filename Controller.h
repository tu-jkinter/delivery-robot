#include "HardwareSerial.h"

// This class abstracts the control of the robot's motors and exposed public APIs
// to directly control the robot's motion.

#ifndef Controller_h
#define Controller_h

#include <Arduino.h>
#include "Motor.h"
#include "Accelerometer.h"

class Controller {

  private:

    //Turn the robot to the left
    void CounterClockwise(int speed) {
        MotorA.Rotate(speed, false);
        MotorB.Rotate(speed, true);
        MotorC.Rotate(speed, false);
        MotorD.Rotate(speed, true);
    }

    //turn the robot to the right
    void Clockwise(int speed) {
        MotorA.Rotate(speed, true);
        MotorB.Rotate(speed, false);
        MotorC.Rotate(speed, true);
        MotorD.Rotate(speed, false);
    }

    //Shift the robot to the right
    void ShiftRight(int speed) {
        MotorA.Rotate(speed, true);
        MotorB.Rotate(speed, false);
        MotorC.Rotate(speed, false);
        MotorD.Rotate(speed, true);
    }

    //Shift the robot to the left
    void ShiftLeft(int speed) {
        MotorA.Rotate(speed, false);
        MotorB.Rotate(speed, true);
        MotorC.Rotate(speed, true);
        MotorD.Rotate(speed, false);
    }

    ///Shift the robot right-forward
    void ShiftRightForward(int speed) {
        MotorA.Rotate(speed, true);
        MotorB.Rotate(0, true);
        MotorC.Rotate(0, true);
        MotorD.Rotate(speed, true);
    }

    ///Shift the robot right-reverse
    void ShiftRightReverse(int speed) {
        MotorA.Rotate(0, false);
        MotorB.Rotate(speed, false);
        MotorC.Rotate(speed, false);
        MotorD.Rotate(0, false);
    }

    //Shift the robot left-forward
    void ShiftLeftForward(int speed) {
        MotorA.Rotate(0, true);
        MotorB.Rotate(speed, true);
        MotorC.Rotate(speed, true);
        MotorD.Rotate(0, true);
    }

    //Shift the robot left-reverse
    void ShiftLeftReverse(int speed) {
        MotorA.Rotate(speed, false);
        MotorB.Rotate(0, false);
        MotorC.Rotate(0, false);
        MotorD.Rotate(speed, false);
    }

    //Rotate the robot right in an arc with its back as the axis of rotation
    void ArcRight(int speed) {
        MotorA.Rotate(speed, true);
        MotorB.Rotate(speed, false);
        MotorC.Rotate(0, true);
        MotorD.Rotate(0, true);
    }

    //Rotate the robot left in an arc with its back as the axis of rotation
    void ArcLeft(int speed) {
        MotorA.Rotate(speed, false);
        MotorB.Rotate(speed, true);
        MotorC.Rotate(0, true);
        MotorD.Rotate(0, true);
    }

    void UpdateMotorEncoders() {
      MotorA.Encoder.Update();
      MotorB.Encoder.Update();
      MotorC.Encoder.Update();
      MotorD.Encoder.Update();
    }

  public:

    //Instantiate motors and accelerometer
    Accelerometer Accelerometer;
    Motor MotorA;
    Motor MotorB;
    Motor MotorC;
    Motor MotorD;

    //Initialize Pins
    void Initialize() {

        //Initialize motors
        MotorA.Initialize('A');
        MotorB.Initialize('B');
        MotorC.Initialize('C');
        MotorD.Initialize('D');

        //Accelerometer
        Accelerometer.Initialize();
        Accelerometer.UpdateValues();
    }

    void ResetMotorEncoders() {
      //Reset encoders
      MotorA.Encoder.Reset();
      MotorB.Encoder.Reset();
      MotorC.Encoder.Reset();
      MotorD.Encoder.Reset();
    }

    //Stop all motors
    void Stop() {
        analogWrite(MotorA.SpeedPin, 0);
        analogWrite(MotorB.SpeedPin, 0);
        analogWrite(MotorC.SpeedPin, 0);
        analogWrite(MotorD.SpeedPin, 0);
    }

    void Brake(bool forward) {
      
      ResetMotorEncoders();

      if (forward) {
        Reverse(255);
      }
      else
      {
        Forward(255);
      }

      Stop();
    }

    //Move robot forward
    void Forward(int speed) {
        MotorA.Rotate(speed, true);
        MotorB.Rotate(speed, true);
        MotorC.Rotate(speed, true);
        MotorD.Rotate(speed, true);
    }

    //Move robot in reverse
    void Reverse(int speed) {
        MotorA.Rotate(speed, false);
        MotorB.Rotate(speed, false);
        MotorC.Rotate(speed, false);
        MotorD.Rotate(speed, false);
    }

    //Move the robot forward a specified distance (in inches).
    void Forward(int speed, int distance) {
      
      float modifier = (distance >= 4) ? 1.5 * 80 : 0;

      //750 pulses per motor rotation
      //This converts to roughly 80 pulses for every 1 inch of motion
      //ditance is measured in inches in this API
      double pulseTarget = (distance * 80) - modifier;
      int pulseCount = 0;

      bool isCompleted = false;

      ResetMotorEncoders();

      //Begin moving
      Forward(speed);

      while (!isCompleted)
      {
        //Calculate average of all four motors 
        UpdateMotorEncoders();
        
        pulseCount = MotorA.Encoder.Count;

        if (pulseCount >= pulseTarget)
        {
          Brake(true);
          isCompleted = true;
        }
      }

      ResetMotorEncoders();
    }

    void Reverse(int speed, int distance) {

      float modifier = (distance >= 4) ? 1.5 * 80 : 0;

      //750 pulses per motor rotation
      //This converts to roughly 80 pulses for every 1 inch of motion
      //ditance is measured in inches in this API
      double pulseTarget = (distance * 80) - modifier;
      int pulseCount = 0;

      bool isCompleted = false;

      ResetMotorEncoders();

      //Begin moving
      Reverse(speed);

      while (!isCompleted)
      {
        //Calculate average of all four motors 
        UpdateMotorEncoders();
        
        pulseCount = abs(MotorA.Encoder.Count);

        if (pulseCount >= pulseTarget)
        {
          Brake(true);
          isCompleted = true;
        }
      }

      ResetMotorEncoders(); 
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