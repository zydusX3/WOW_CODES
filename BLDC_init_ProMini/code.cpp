/*
  Auto detect ESC and Config BLDC
  Pot control of BLDC
*/

#include <Servo.h>

Servo esc;  // create Servo object to control a servo

int potpin = A1;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin

int ESCpin = 10;
bool confDone = false;
int isESC = 0;

void setup() {

  Serial.begin(9600);
  Serial.println("in Setup");

  pinMode(ESCpin, INPUT_PULLUP);

  esc.attach(9);  // attaches the servo on pin 9 to the Servo object

  if(digitalRead(ESCpin)) {
    Serial.println("ESC found");
    // initMotor();
  } else {
    Serial.println("ESC not found");
  }

}

void initMotor() {
  Serial.println("in initMotor");
  // esc.attach(9);  // attaches the servo on pin 9 to the Servo object
  delay(400);
  esc.writeMicroseconds(2000); // Minimum throttle to arm ESC
  delay(1000); // wait till battery connect to ESC

  for(int i = 2000;i>=1000;i--) {
    esc.writeMicroseconds(i);
    delayMicroseconds(200);
  }

  delay(1000);
  // esc.writeMicroseconds(1000); // Set throttle to 50%
  // delay(2000); // Run for 1 second
  esc.writeMicroseconds(1500); // Set throttle to 50%
  delay(400);
  esc.writeMicroseconds(1000); // Set throttle to 50%
  delay(1000);
  confDone = true;
  Serial.println("confDone:"+String(confDone));
}

void loop() {
  isESC = digitalRead(ESCpin);
  Serial.println("isESC:"+String(isESC));
  if(isESC) {
    // confDone = true;
    if(!confDone) {
      initMotor();
      // Serial.println("in initMotor");
    //  confDone = false
    }else{
      Serial.println("already configured");
    }
  } else {
      Serial.println("No Esc");
      confDone = false;
  }

  val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023)
  val = map(val, 0, 1023, 1000, 2000);     // scale it for use with the servo (value between 0 and 180)
  esc.writeMicroseconds(val);                  // sets the servo position according to the scaled value
  delay(15);                           // waits for the servo to get there
}
