

#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <ZumoBuzzer.h>

#define NUM_PHOTORES 4

// Number to add to the environment reading in order to get the final threshold
#define BASE_THRESH 50

// Defines the time between the button press and the calibration phase
#define BUTTON_DELAY 2000

// The greater is this value, the softer will be the changes between speeds
#define LOOP_DELAY 50

// Speed range 0 - 400
#define FORWARD_SPEED  200
#define REVERSE_SPEED -200 
#define TURN_SPEED     200

// Define the pins assigned to the photoresistors
#define FW 4
#define SX 1
#define DX 3
#define BW 0
 
ZumoMotors motors;
Pushbutton leftButton(ZUMO_BUTTON);
Pushbutton rightButton(11);
ZumoBuzzer buzzer;

// photores_pins[] contains the pins corresponding to each sensor
const int photores_pins[]= {FW, SX, DX, BW };

// leftSpeedArray[] contains the speeds for the left motor corresponding to each direction (FW - SX - DX - BW)
const int leftSpeedArray[] = { FORWARD_SPEED, -TURN_SPEED, TURN_SPEED, REVERSE_SPEED };

// rightSpeedArray[] contains the speeds for the right motor corresponding to each direction (FW - SX - DX - BW)
const int rightSpeedArray[] = { FORWARD_SPEED, TURN_SPEED, -TURN_SPEED, REVERSE_SPEED };

// thresh[] is used to store the threshold for each sensor
int thresh[NUM_PHOTORES];

void setup()
{ 
  int i;

  // Starting calibration when leftButton is pressed and released
  leftButton.waitForButton();
  buzzer.playNote (NOTE_A(4), 125, 15);

  // Wait in order to avoid covering the sensors while pressing the button
  delay(BUTTON_DELAY);
  
  // For each sensor
  for(i = 0;i < NUM_PHOTORES; i++)
  {
    // Calibration of the sensors
    thresh[i] = analogRead(photores_pins[i]) + BASE_THRESH;
  }

  buzzer.playNote (NOTE_A(5), 125, 15);
  
  // End of calibration phase 
}

void loop()
{
  static int leftSpeed, rightSpeed, prevSpeed, num, i;

  // For each sensor
  num = 0;
  for(i = 0; i < NUM_PHOTORES; i++)
  {
    // Read if the sensor is under the beam of light
    // and in this case save the speeds for the motors
    if( analogRead(photores_pins[i]) > thresh[i])
    {
      leftSpeed = leftSpeedArray[i];
      rightSpeed = rightSpeedArray[i];

      // Count how many sensors are under the light
      num++;
    }
  }

  // If more than one sensor is under the light
  if( num != 1 )
  {
    // If the robot was turning, then stop it
    if(prevSpeed == 0)
      leftSpeed = rightSpeed = 0;
    
    // If the robot was going forward, then slow it
    else if(prevSpeed > 0)
      leftSpeed = rightSpeed = prevSpeed - (FORWARD_SPEED >> 3);
    
    // If the robot was going backward, then slow it
    else
      leftSpeed = rightSpeed = prevSpeed - (REVERSE_SPEED >> 3);
  }

  // Set the speed for the motors
  motors.setSpeeds(leftSpeed, rightSpeed);

  // Save the average of the speeds
  prevSpeed = (leftSpeed + rightSpeed) >> 1;

  // Avoid sudden changes
  delay(LOOP_DELAY);
}
