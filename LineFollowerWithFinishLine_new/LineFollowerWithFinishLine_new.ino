/*
 * Demo line-following code for the Pololu Zumo Robot
 *
 * This code will follow a black line on a white background, using a
 * PID-based algorithm.  It works decently on courses with smooth, 6"
 * radius curves and has been tested with Zumos using 30:1 HP and
 * 75:1 HP motors.  Modifications might be required for it to work
 * well on different courses or with different motors.
 *
 * http://www.pololu.com/catalog/product/2506
 * http://www.pololu.com
 * http://forum.pololu.com
 */

#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <ZumoMotors.h>
#include <ZumoBuzzer.h>
#include <Pushbutton.h>
#define QTR_THRESHOLD 400
#define TRUE 1
#define FALSE 0
#define MAX_SPEED 339




ZumoBuzzer buzzer;
ZumoReflectanceSensorArray reflectanceSensors;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);
int lastError = 0;
bool flag = TRUE;
int error_divider;



// This is the maximum speed the motors will be allowed to turn.
// (400 lets the motors go at top speed; decrease to impose a speed limit)
//const int MAX_SPEED = 340;


void setup()
{

  if(MAX_SPEED <= 300){
    error_divider=4;
    flag=FALSE;
  }else if(MAX_SPEED >300 && MAX_SPEED<340){
    error_divider=7;
    flag=TRUE;
  }else{
    error_divider=9;
    flag=TRUE;
  }
  // Play a little welcome song
  buzzer.play(">g32>>c32");

  // Initialize the reflectance sensors module
  reflectanceSensors.init();

  // Wait for the user button to be pressed and released
  button.waitForButton();

  // Turn on LED to indicate we are in calibration mode
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  // Wait 1 second and then begin automatic sensor calibration
  // by rotating in place to sweep the sensors over the line
  delay(1000);
  int i;
  for(i = 0; i < 80; i++)
  {
    if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
      motors.setSpeeds(-200, 200);
    else
      motors.setSpeeds(200, -200);
    reflectanceSensors.calibrate();

    // Since our counter runs to 80, the total delay will be
    // 80*20 = 1600 ms.
    delay(20);
  }
  motors.setSpeeds(0,0);

  // Turn off LED to indicate we are through with calibration
  digitalWrite(13, LOW);
  buzzer.play(">g32>>c32");

  // Wait for the user button to be pressed and released
  button.waitForButton();

  // Play music and wait for it to finish before we start driving.
  buzzer.play("L16 cdegreg4");
  while(buzzer.isPlaying());
}

void loop()
{
  unsigned int sensors[6];

  // Get the position of the line.  Note that we *must* provide the "sensors"
  // argument to readLine() here, even though we are not interested in the
  // individual sensor readings
  int position = reflectanceSensors.readLine(sensors);

  // Our "error" is how far we are away from the center of the line, which
  // corresponds to position 2500.
  int error = position - 2500;

  // Get motor speed difference using proportional and derivative PID terms
  // (the integral term is generally not very useful for line following).
  // Here we are using a proportional constant of 1/4 and a derivative
  // constant of 6, which should work decently for many Zumo motor choices.
  // You probably want to use trial and error to tune these constants for
  // your particular Zumo and line course.
  int speedDifference = error / error_divider  + 6 * (error - lastError);

  lastError = error;

  // Get individual motor speeds.  The sign of speedDifference
  // determines if the robot turns left or right.
  int m1Speed = MAX_SPEED + speedDifference;
  int m2Speed = MAX_SPEED - speedDifference;

  // Here we constrain our motor speeds to be between 0 and MAX_SPEED.
  // Generally speaking, one motor will always be turning at MAX_SPEED
  // and the other will be at MAX_SPEED-|speedDifference| if that is positive,
  // else it will be stationary.  For some applications, you might want to
  // allow the motor speed to go negative so that it can spin in reverse.
    if (m1Speed < 0)
      m1Speed = 0;
    if (m2Speed < 0)
      m2Speed = 0;
    if (m1Speed > MAX_SPEED)
      m1Speed = MAX_SPEED;
    if (m2Speed > MAX_SPEED)
      m2Speed = MAX_SPEED;

    motors.setSpeeds(m1Speed, m2Speed);
  
  //checks if the zumo has reached the finish line
  if(sensors[0] > QTR_THRESHOLD && sensors[5] > QTR_THRESHOLD){
    motors.setSpeeds(0, 0);
    buzzer.play("L16 cdegreg4");
    //wait for the button to be pressed and restart driving
    button.waitForButton();
    // Play music and wait for it to finish before we start driving.
    buzzer.play(">g32>>c32");
    while(buzzer.isPlaying());

    //gives time to the zumo to surpass the finish line and restart the line.
    motors.setSpeeds(MAX_SPEED, MAX_SPEED);
    delay(100);
  }
  if(sensors[0] < QTR_THRESHOLD && sensors[1]< QTR_THRESHOLD && sensors[2] < QTR_THRESHOLD && sensors[3] < QTR_THRESHOLD && sensors[4] < QTR_THRESHOLD && sensors[5] < QTR_THRESHOLD){
    while(flag){
      motors.setSpeeds(0, 0);
      button.waitForButton();
    }
    /*while(flag){
      button.waitForButton();
      
      motors.setSpeeds(MAX_SPEED, MAX_SPEED);
      delay(500);
      motors.setSpeeds(-MAX_SPEED, MAX_SPEED);
      delay(200);
      motors.setSpeeds(-MAX_SPEED, -MAX_SPEED);
      delay(500);
    }*/
  }
  
}
