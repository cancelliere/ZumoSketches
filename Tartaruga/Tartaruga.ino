#include <Wire.h>
#include <L3G.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <ZumoBuzzer.h>

//questi sono i parametri regolabili, ma non vi consiglio di toccarli durante l'evento per possibili effetti collaterali
#define COMMANDS_NUM 12
#define STRAIGHT_DURATION 500
#define STRAIGHT_SPEED 200
#define TURN_SPEED 25
#define TURN_ANGLE 90000

#define W_OFFSET 1120
#define POWER_OF_TWO 64
#define INITIAL_CORRECTION 30
#define SAMPLING_TIME 11
#define THRESHOLD 125
#define STRAIGHT_TH 125
#define CORRECTION_FACTOR 20

#define FRONT_LDR 4
#define BACK_LDR 0
#define SX_LDR 1
#define RX_LDR 3

#define LOW_TIME 500

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
int stepDeviation1 = 0;
int stepDeviation2 = 0;
int offo = 0;
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

  off2();

  for (int i=0; i<COMMANDS_NUM; i++) {
    commandList[i]=99;  //set to a value that doesn't interfere with the real commands
  }

  LDRcalibration2();

  delay(1000);
}

void loop() {

  buildList();

  evaluateList(commandList);

  absoluteAngle = targetAngle  = 0;

  for (int i=0; i<COMMANDS_NUM; i++) {
    commandList[i]=99;  //set to a value that doesn't interfere with the real commands
  }

}

void off2() {
  int dir = 1;
  unsigned long startStraight;
  unsigned int cycle = 0;
  int absStepDeviation;
  int leftCorrection = 0, rightCorrection = 0;
  long previous = 0;

  currentAngle = absoluteAngle;

  motors.setSpeeds( STRAIGHT_SPEED, STRAIGHT_SPEED);
  for ( int j = 0; j < 10; j++ ) {
    for (int i=0; i<10; i++) {
      gyro.read();
      reading = (gyro.g.z/POWER_OF_TWO) *POWER_OF_TWO; // remove static noise
      angularReading = reading * 17.5 - offset; // compensate for residual noise (almost constant with time)
      currentAngle += (angularReading + previous)/2 * SAMPLING_TIME / 1000.0; // sampling time is in ms
      previous = angularReading;
      delay(10);
     }
    offo += currentAngle;
    currentAngle = 0;
    previous = 0;
  }
  offo = offo / 10;
  Serial.println(offo);


  motors.setSpeeds(0,0);
  delay(1000);


  motors.setSpeeds( STRAIGHT_SPEED + INITIAL_CORRECTION, STRAIGHT_SPEED);
  for ( int j = 0; j < 10; j++ ) {
    for (int i=0; i<10; i++) {
      gyro.read();
      reading = (gyro.g.z/POWER_OF_TWO) *POWER_OF_TWO; // remove static noise
      angularReading = reading * 17.5 - offset; // compensate for residual noise (almost constant with time)
      currentAngle += (angularReading + previous)/2 * SAMPLING_TIME / 1000.0; // sampling time is in ms
      previous = angularReading;
      delay(10);
     }
    stepDeviation1 += currentAngle;
    currentAngle = 0;
    previous = 0;
  }
  stepDeviation1 = stepDeviation1 / 10;
  Serial.println(stepDeviation1);

  motors.setSpeeds(0,0);
  delay(1000);

  motors.setSpeeds( STRAIGHT_SPEED - INITIAL_CORRECTION, STRAIGHT_SPEED);

 for ( int j = 0; j < 10; j++ ) {
    for (int i=0; i<10; i++) {
      gyro.read();
      reading = (gyro.g.z/POWER_OF_TWO) *POWER_OF_TWO; // remove static noise
      angularReading = reading * 17.5 - offset; // compensate for residual noise (almost constant with time)
      currentAngle += (angularReading + previous)/2 * SAMPLING_TIME / 1000.0; // sampling time is in ms
      previous = angularReading;
      delay(10);
     }
    stepDeviation2 += currentAngle;
    currentAngle = 0;
    previous = 0;
  }
  stepDeviation2 = stepDeviation2 / 10;
  Serial.println(stepDeviation2);
  //stepDeviation2  la deviazione che ho quando la ruota sinistra  piu' lenta e quindi quella che deve essere utilizzata
  //quando stepDeviation e' minore di zero!!!
  motors.setSpeeds( 0, 0);
  stepDeviation1 = abs(stepDeviation1);

  if ( offo > 0 ) {
    offo = 5/stepDeviation1*offo;
  }
  else
    offo = -5/stepDeviation2*offo;

  buzzer.playNote (NOTE_A(4), 125, 15);
  //delay(250);
}



void LDRcalibration2() {

  long int readingsH[4];
  long int readingsL[4];
  int frontReading;
  int i,cycle;
  /*        0
   *    1        2
   *         3
   */

  //find min reading for each sensor when lit
 for (i=0; i<4; i++) {
   readingsH[i] = 0;
   readingsL[i] = 0;   //set to zero
 }

 buzzer.playNote (NOTE_A(4), 125, 15);
 digitalWrite(LED_PIN, HIGH);

 for ( cycle = 0; cycle < 40; cycle ++ ) {
  readingsH[0] += analogRead(FRONT_LDR);
  readingsH[1] += analogRead(SX_LDR);
  readingsH[2] += analogRead(RX_LDR);
  readingsH[3] += analogRead(BACK_LDR);
  delay(50);
 }

 for ( i = 0; i < 4; i++ )
   readingsH[i] = readingsH[i] / 40;

 digitalWrite(LED_PIN, LOW);
 delay(500);

 for ( i = 0; i < 3; i++ ) {
  buzzer.playNote (NOTE_A(4), 125, 15);
  delay(125);
  buzzer.playNote (SILENT_NOTE, 125, 15);
 }
 delay(100);
 buzzer.playNote (NOTE_A(5), 125, 15);

 digitalWrite(LED_PIN, HIGH);
 delay(1000);

 for ( cycle = 0; cycle < 40; cycle ++ ) {
  readingsL[0] += analogRead(FRONT_LDR);
  readingsL[1] += analogRead(SX_LDR);
  readingsL[2] += analogRead(RX_LDR);
  readingsL[3] += analogRead(BACK_LDR);
  delay(50);
 }

 for ( i = 0; i < 4; i++ )
   readingsL[i] = readingsL[i] / 40;

 digitalWrite(LED_PIN, LOW);

  // evaluate the weighted average to get the threshold
  for ( i = 0; i < 4; i++ )
    sensorTh[i] = (int) (0.2*readingsH[i] + 0.8*readingsL[i]);

  for (int i=0; i<4; i++) {
    Serial.print(sensorTh[i]);
    Serial.print(" ");
  }

  delay(250);
  buzzer.playNote (NOTE_A(3), 125, 15);

  return;
}

void buildList() {
  buzzer.playNote (NOTE_A(4), 125, 15);
  delay(125);
  buzzer.playNote (SILENT_NOTE, 125, 15);
  delay(125);
  buzzer.playNote (NOTE_A(4), 125, 15);

  Serial.println("Waiting for commands...");

  int readings[4];
  bool lowValue[4]={0}; //sensor is currently low
  bool prevLow[4]={0}; //sensor was low in the previous iteration
  int commands = 0;
  bool commandSet[4] = {0};
  unsigned long lowStart;

  while (commands <= COMMANDS_NUM && !startButton.isPressed() ) { //continue to read until the maximum number of commands is entered
                                                                                                       //or the start button is pressed
    readings[0] = analogRead(FRONT_LDR);
    readings[1] = analogRead(SX_LDR);
    readings[2] = analogRead(RX_LDR);
    readings[3] = analogRead(BACK_LDR);

    for (int i=0; i<4; i++) {
      if (readings[i] < sensorTh[i]) {
        if (prevLow[i]==0) {
          lowValue[i] = 1; //the sensor's reading is below the set threshold
          lowStart = millis();
          prevLow[i] = 1;
        }
        /* checks for the following cases: LDR was pressed before, a command was not already enqueued for this series of readings, the LDR was
         *  covered for a sufficient time and no other sensors are reading a value below threshold
         */
        else if (prevLow[i] == 1 && commandSet[i] == 0 && millis()-lowStart > LOW_TIME && noOtherLow(i, readings) ) {
          commandList[commands++] = i;
          Serial.print(i);
          Serial.println(" SET");
          buzzer.playNote (NOTE_A(4), 125, 15);
          commandSet[i] = 1;
        }
      }
      else {
        lowValue[i] = 0;
        prevLow[i] = 0;
        commandSet[i] = 0;
      }
    }
    delay(25);
  }
  while(startButton.isPressed()) { //wait for the user to realease the button
    delay(10);
  }
  delay(500);
}

/* checks if other sensors other than the one given as argument are below threshold
 *  to avoid the insertion of multiple commands in the execution list
 */
bool noOtherLow(byte lowPin, int * sensorValues) {
  for (int i=0; i<4; i++) {
    if (i != lowPin && sensorValues[i] < sensorTh[i]) {
      return 0;
    }
  }
  return 1;
}

void evaluateList (byte * list) {
  for (int i=0; i< COMMANDS_NUM; i++) {
    switch (list[i]) {
      case 0:
        straight (1);
        break;
      case 3:
        straight (-1);
        break;
      case 1:
        turn (1);
        break;
      case 2:
        turn (-1);
        break;
      default:
        return;
    }
  }
}

void straight (int dir) {
  unsigned long startStraight;
  unsigned int cycle = 0;
  int leftCorrection = 0, rightCorrection = 0;
  int previous = 0; //prova
  currentAngle = absoluteAngle;

  //Serial.print("START: ");
  //Serial.println(currentAngle);

    startStraight = millis();
    motors.setSpeeds( 0, 0);

    while (millis() - startStraight < STRAIGHT_DURATION) {
      motors.setSpeeds( dir * (+STRAIGHT_SPEED + leftCorrection + offo), dir * (+STRAIGHT_SPEED + rightCorrection) );

      for (int i=0; i<10; i++) {
        gyro.read();
        reading = (gyro.g.z / POWER_OF_TWO) * POWER_OF_TWO; // remove static noise
        angularReading = reading * 17.5 - offset; // compensate for residual noise (almost constant with time)
        currentAngle += (angularReading + previous)/2 * SAMPLING_TIME / 1000.0; // sampling time is in ms
        previous = angularReading;
        delay(10);
     }

      stepDeviation = currentAngle - targetAngle;
      Serial.println(currentAngle);
      cycle++;

      //Serial.print (stepDeviation);
      //Serial.print (" ");
      if ( dir == 1 ) {
        if ( stepDeviation > 0 )
          leftCorrection = stepDeviation*5/stepDeviation1;
        else
          leftCorrection = stepDeviation*5/stepDeviation2;
      }
      else {
        if ( stepDeviation > 0 )
          leftCorrection = -stepDeviation*5/stepDeviation1;
        else
          leftCorrection = -stepDeviation*5/stepDeviation2;
      }


      Serial.print(" - L: ");
      Serial.println(leftCorrection);
    }

    motors.setSpeeds( 0, 0);

    absoluteAngle = targetAngle+stepDeviation;

    Serial.print ("Deviation from straight: ");
    Serial.println(absoluteAngle-targetAngle);

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
        reading = (gyro.g.z / POWER_OF_TWO) * POWER_OF_TWO; // remove static noise
        angularReading = reading * 17.5 - offset; // compensate for residual noise (almost constant with time)
        currentAngle += angularReading * SAMPLING_TIME / 1000; // sampling time is in ms
        //cycles++;
        delay(10);
      }

      motors.setSpeeds (+3*TURN_SPEED, -3*TURN_SPEED);
      while (currentAngle > targetAngle + THRESHOLD) {
        gyro.read();
        reading = (gyro.g.z / POWER_OF_TWO) * POWER_OF_TWO; // remove static noise
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
        reading = (gyro.g.z / POWER_OF_TWO) * POWER_OF_TWO; // remove static noise
        angularReading = reading * 17.5 - offset; // compensate for residual noise (almost constant with time)
        currentAngle += angularReading * SAMPLING_TIME / 1000; // sampling time is in ms
        //cycles++;
        delay(10);
      }
      motors.setSpeeds (- 3*TURN_SPEED, + 3*TURN_SPEED);
      while (currentAngle < targetAngle - THRESHOLD) {
        gyro.read();
        reading = (gyro.g.z / POWER_OF_TWO) * POWER_OF_TWO; // remove static noise
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

  for (int i=0; i<500; i++) { // execute some readings when not moving
    gyro.read();
   //reading = (gyro.g.z >> 14) << 14;
   // reading = (gyro.g.z /16384) * 16384;
   reading = (gyro.g.z / POWER_OF_TWO) * POWER_OF_TWO;
   //reading = (gyro.g.z);

    angularReading = reading * 17.5;
    currentOffset += angularReading;
    Serial.println(angularReading);
    delay(5);
  }

  currentOffset /= 500;

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
  Serial.println(currentOffset);
  Serial.print(" -> ");
  Serial.println(redefinedOffset);

  offset = redefinedOffset;
  Serial.println("Redefined");
}
