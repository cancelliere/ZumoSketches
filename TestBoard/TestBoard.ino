#include <Pushbutton.h>
#include <ZumoBuzzer.h>

#define NUM_PHOTORES 4

// Number to add to the environment reading in order to get the final threshold
#define BASE_THRESH 50

#define BUTTON_DELAY 2000

// Define the pins assigned to the photoresistors
#define FW 4
#define SX 1
#define DX 3
#define BW 0
 
Pushbutton leftButton(ZUMO_BUTTON);
Pushbutton rightButton(11);
ZumoBuzzer buzzer;

// photores_pins[] contains the pins corresponding to each sensor
const int photores_pins[]= {FW, SX, DX, BW };

// thresh[] is used to store the threshold for each sensor
int thresh[NUM_PHOTORES];

// array[] contains the value of the sensors
int array[NUM_PHOTORES] = { 0, 0, 0, 0 };

int i;

void setup()
{ 
  Serial.begin(9600);

  Serial.println();
  
  Serial.print("Testing ButtonLeft ");
  leftButton.waitForButton();
  Serial.print("OK\n");
  buzzer.playNote (NOTE_A(4), 125, 15);
  
  Serial.print("Testing ButtonRight ");
  rightButton.waitForButton();
  Serial.print("OK\n");
  buzzer.playNote (NOTE_A(5), 125, 15);

  // Wait in order to avoid covering the sensors while pressing the button
  delay(BUTTON_DELAY);
  
  for(i = 0; i < NUM_PHOTORES; i++)
  {
    // Calibration of the sensors
    thresh[i] = analogRead(photores_pins[i]) + BASE_THRESH;
  }
}

void loop()
{
  Serial.println("FW\tSX\tDX\tBW");
  
  for(i = 0; i < NUM_PHOTORES; i++)
  {
    array[i] = analogRead(photores_pins[i]);
    Serial.print(array[i]);
    Serial.print("\t");
  }
  Serial.print("\n");
  
  for(i = 0; i < NUM_PHOTORES; i++)
  {
    if( array[i] > thresh[i])
      Serial.print("ON\t");
    else
      Serial.print("OFF\t");
  }
  Serial.print("\n\n");
  delay(500);
}
