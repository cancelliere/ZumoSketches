/*
 * This is a modified version by Politecnico di Torino of the  
 * Demo line-following code for the Pololu Zumo Robot
 */

#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <ZumoMotors.h>
#include <ZumoBuzzer.h>
#include <Pushbutton.h>


ZumoBuzzer buzzer;
ZumoReflectanceSensorArray reflectanceSensors;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);

// Quanto ti senti coraggioso?
// Decidi di quanto aumentare la velocità del tuo zumo
// Valori consentiti: 0 - 100
// Valori negativi rallenteranno il tuo zumo
const int SPEED_INCREASE = 50;

// Numero di giri
const int MAX_LAPS = 3;

// This is the maximum speed the motors will be allowed to turn.
const int MAX_SPEED = 350 + SPEED_INCREASE / 2;

unsigned int max_error;
byte max_nerrors;
byte laps;

int lastError = 0;
const int THRESH = 500;

void setup()
{
  // Play a little welcome song
  buzzer.play(">g32>>c32");

  // Set the maximum value of error allowed according to the speed
  // (only if SPEED_INCREASE is positive
  max_error = 2501;
  if(SPEED_INCREASE > 0)
    max_error = 2500 - (SPEED_INCREASE*sqrt(SPEED_INCREASE));

  // Set the maximum number of 'errors' allowed according to the speed
  // (only if SPEED_INCREASE is greater than 50)
  max_nerrors = 25;
  if(SPEED_INCREASE >= 40)
    max_nerrors -= (SPEED_INCREASE-40)/4;

  // Reset the number of laps
  laps = 0;

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
  static unsigned int nerrors;
  static unsigned long lastLap;

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
  int speedDifference = error / 5.5 + 6 * (error - lastError);

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

  // Reset the number of errors if there is no error
  if(abs(error) < max_error)
    nerrors = 0;
  // If there is an error
  else
  {
    nerrors++;
    
    // If the number of errors equals the maximum allowed
    if(nerrors == max_nerrors)
    {
      buzzer.playNote(NOTE_A(4), 125, 15);
      
      // Go straight on and then stop
      m1Speed = m2Speed = MAX_SPEED;
      motors.setSpeeds(m1Speed, m2Speed);
      delay(200);
      motors.setSpeeds(0, 0);

      buzzer.playNote(NOTE_A(3), 125, 15);

      // Wait for the button before restart driving
      button.waitForButton();
    }
  }

  motors.setSpeeds(m1Speed, m2Speed);

  // Has the robot reached the finish line?
  if(sensors[0] > THRESH && sensors[5] > THRESH)
  { 
    if(millis() - lastLap > 2500)
    {
      // Reset the number of laps
      laps++;
      lastLap = millis();

      // End of the race
      if(laps > MAX_LAPS)
      {
        laps = 0;
      
        motors.setSpeeds(0, 0); 
        buzzer.play("L16 cdegreg4"); 
    
        // Wait for the button to be pressed and restart driving 
        button.waitForButton();
      }
    
      // Play a bip when the finish line is overpassed
      else
        buzzer.play(">g32>>c32");
    }
  } 

}
