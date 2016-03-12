#include <Wire.h>
#include <L3G.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <ZumoBuzzer.h>

#define W_OFFSET 1120
#define POWER_OF_TWO 64
#define SAMPLING_TIME 11
#define TURN_SPEED 25
#define STRAIGHT_SPEED 200
#define TURN_ANGLE 90000
#define THRESHOLD 125
#define STRAIGHT_TH 250
#define CORRECTION_FACTOR 20
#define STRAIGHT_DURATION 1000
#define COMMANDS_NUM 10

#define LDR_THRESHOLD 150

#define FRONT_LDR 4
#define BACK_LDR 0
#define SX_LDR 1
#define RX_LDR 3

#define LOW_TIME 600

#define LED_PIN 13

ZumoMotors motors;
ZumoBuzzer buzzer;
Pushbutton startButton(ZUMO_BUTTON);
Pushbutton stopButton(11);
L3G gyro;

int reading; // numerical value from gyro
long angularReading; // conversion from "reading" to angular Velocity
long currentAngle = 0; // integration over time
long currentOffset = 0; // to check if the defined offset is correct
int offset = W_OFFSET;
long absoluteAngle = 0; // absolute angular position relative to the starting point
int stepDeviation = 0;
long targetAngle; // ideal angular position to reach
byte commandList[COMMANDS_NUM];
unsigned int sensorTh[4];

unsigned long start; // check loops duration

void setup() {
  pinMode(LED_PIN, OUTPUT);
  
  Serial.begin (115200);
  Wire.begin();

  if (!gyro.init())
  {
    Serial.println("Failed to autodetect gyro type!");
    while (1);
  }

  gyro.enableDefault();

  setFullScale();
  
  checkOffset();
  
  delay(500);
}


void loop() {
  startButton.waitForButton();

  delay(500);
  
  straight_steps(10);
  
  absoluteAngle = targetAngle  = 0;
}


void straight_steps (int totalSteps) {
  int dir = 1;
  unsigned long startStraight;
  unsigned int cycle = 0;
  int absStepDeviation;
  int leftCorrection = 0, rightCorrection = 0;
  currentAngle = absoluteAngle;

  //Serial.print("START: ");
  //Serial.println(currentAngle);
  
    motors.setSpeeds( 0, 0);
    for (int steps = 0; steps < totalSteps; steps++) {
      
      startStraight = millis();
      Serial.print("Step: ");
      Serial.println(steps);
      //currentAngle = 0; //--------------------------------ogni step è indipendente ai fini della deviazione (in fase di testing)
      
      while (millis() - startStraight < STRAIGHT_DURATION) {
        motors.setSpeeds( dir * (+STRAIGHT_SPEED + leftCorrection), dir * (+STRAIGHT_SPEED + rightCorrection) );
        
        for (int i=0; i<10; i++) {
          gyro.read();
          reading = (gyro.g.z/POWER_OF_TWO) *POWER_OF_TWO; // remove static noise
          angularReading = reading * 17.5 - offset; // compensate for residual noise (almost constant with time)
          currentAngle += angularReading * SAMPLING_TIME / 1000.0; // sampling time is in ms
          delay(10);
       }
        
        stepDeviation = currentAngle - targetAngle;
        
        cycle++;
      
        //Serial.print (stepDeviation);
        //Serial.print (" ");
        
        if (stepDeviation > 5*STRAIGHT_TH) {
          leftCorrection += STRAIGHT_SPEED/10;  
        }
        else if (stepDeviation > STRAIGHT_TH) {                                                                                                                                          
          leftCorrection += STRAIGHT_SPEED/40;
        }
        
        else if (stepDeviation < -5*STRAIGHT_TH) {
          leftCorrection -= STRAIGHT_SPEED/10;
        }
        else if (stepDeviation > STRAIGHT_TH) {                                                                                                                                          
          leftCorrection -= STRAIGHT_SPEED/40;
        }

        if (leftCorrection > 75) leftCorrection = 75;
        else if (leftCorrection < -75) leftCorrection = -75;

        Serial.print("L: ");
        Serial.println(leftCorrection);
      }
      absoluteAngle = targetAngle+stepDeviation;

    Serial.print ("Deviation from straight: ");
    Serial.println(absoluteAngle-targetAngle);
    }

    motors.setSpeeds( 0, 0);

  buzzer.playNote (NOTE_A(4), 125, 15);  
  //delay(250);
}

void turn(int dir) {
  currentAngle = absoluteAngle;

  if (dir == -1) { //right turn
    Serial.println("");
    Serial.println("RIGHT");
      
    targetAngle -= TURN_ANGLE;
    
    motors.setSpeeds (+4*TURN_SPEED, -4*TURN_SPEED);

      while (currentAngle > targetAngle + 5000) {
        gyro.read();
          reading = (gyro.g.z/POWER_OF_TWO) *POWER_OF_TWO; // remove static noise
        angularReading = reading * 17.5 - offset; // compensate for residual noise (almost constant with time)
        currentAngle += angularReading * SAMPLING_TIME / 1000; // sampling time is in ms
        //cycles++;
        delay(10);
      }
  
      motors.setSpeeds (+3*TURN_SPEED, -3*TURN_SPEED);
      while (currentAngle > targetAngle + THRESHOLD) {
        gyro.read();
          reading = (gyro.g.z/POWER_OF_TWO) *POWER_OF_TWO; // remove static noise
        angularReading = reading * 17.5 - offset; // compensate for residual noise (almost constant with time)
        currentAngle += angularReading * SAMPLING_TIME / 1000; // sampling time is in ms
        //cycles++;
        delay(10);
      }
    }

  else if (dir == +1) { //left turn
    Serial.println("");
    Serial.println("LEFT");

      
    targetAngle += TURN_ANGLE;
    
    motors.setSpeeds (- 4*TURN_SPEED, + 4*TURN_SPEED);

      while (currentAngle < targetAngle - 5000) {
        gyro.read();
          reading = (gyro.g.z/POWER_OF_TWO) *POWER_OF_TWO; // remove static noise
        angularReading = reading * 17.5 - offset; // compensate for residual noise (almost constant with time)
        currentAngle += angularReading * SAMPLING_TIME / 1000; // sampling time is in ms
        //cycles++;
        delay(10);
      }
      motors.setSpeeds (- 3*TURN_SPEED, + 3*TURN_SPEED);
      while (currentAngle < targetAngle - THRESHOLD) {
        gyro.read();
          reading = (gyro.g.z/POWER_OF_TWO) *POWER_OF_TWO; // remove static noise
        angularReading = reading * 17.5 - offset; // compensate for residual noise (almost constant with time)
        currentAngle += angularReading * SAMPLING_TIME / 1000; // sampling time is in ms
        //cycles++;
        delay(10);
      }
    }
  
  motors.setSpeeds(0,0);
  absoluteAngle = currentAngle;
  
  Serial.print(absoluteAngle);
  Serial.print(" ");
  Serial.print (targetAngle);
  Serial.print (" ");
  Serial.println (absoluteAngle - targetAngle);

  buzzer.playNote (NOTE_A(4), 125, 15);        
  delay(250); 
}

void setFullScale() {
  byte fsreg = gyro.readReg(0x23); // CTRL_REG4
  byte bitmask = 0b01 << 4; // set full scale to 500 dps
  fsreg = fsreg | bitmask;
  gyro.writeReg(0x23, fsreg);
  
  if (gyro.readReg(0x23) != bitmask) { // Check for errors
    Serial.println("Failed to set new FS!");
  }
}

void checkOffset() {
  int redefinedOffset = 0;

  for (int i=0; i<1000; i++) { // execute some readings when not moving
    gyro.read();
    //reading = ( gyro.g.z >> 8) << 8;
    reading = (gyro.g.z / POWER_OF_TWO) * POWER_OF_TWO;
    //reading = gyro.g.z;

    angularReading = reading * 17.5;
    currentOffset += angularReading;
    Serial.println(angularReading);
    delay(5);
  }
  
  currentOffset /= 1000;

  if (currentOffset > 0) {
    while ( !(abs(currentOffset - redefinedOffset) < W_OFFSET/2) ) { // the only possible values that the offset can assume are multiple of 64*17.5
      redefinedOffset += W_OFFSET;                                                                                                                     // therefore the mean is not directly used, but the nearest multiple of 1120 is
    }
  }  
  else {
     while ( !(abs(currentOffset - redefinedOffset) < W_OFFSET/2) ) { 
      redefinedOffset -= W_OFFSET;                                                                                                                     
    } 
  }
  
    
  Serial.print("Current: ");
  /*Serial.println(currentOffset);
  Serial.print(" -> ");*/
  Serial.println(redefinedOffset);

  offset = redefinedOffset;
  Serial.println("Redefined");
}

