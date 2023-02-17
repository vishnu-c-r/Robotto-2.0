#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define SMIN 150       // This is the 'minimum' pulse length count (out of 4096)
#define SMAX 600       // This is the 'maximum' pulse length count (out of 4096)
#define SERVO_FREQ 50  // Analog servos run at ~50 Hz updates

const int legServos[8] = {1, 2, 3, 4, 5, 6, 7, 8};
const int headServo = 9;
const int neckServo = 10;
const int tailServo = 11;
const float a = 50.6; //in mm
const float b = 58.2; //in mm
const int steplen = 30;
float c;  //max and min height is 84 and 40
float inangle[8] = {90,90,90,90,90,90,90,90};
float cursangle[4]={};
float curkangle[4]={};

double knee(float c)
{
   double ang1,deg1; 
   double height ;
   height= ((a*a)+(b*b)-(c*c))/(2*a*b);
   ang1= acos(height);
   deg1 = (ang1 * 180) / PI;
   return deg1;
}

double shoulder(float c)
{
   double ang2,deg2;
   double height ;
   height=((c*c)+(a*a)-(b*b))/(2*a*c);
   ang2= acos(height);
   deg2 = (ang2 * 180) / PI;
   return deg2;
}

void adjustHeight(int x) {
  if(x >= 40 && x <= 84) {
    float kneeAngle = knee(x);
    float shoulderAngle = shoulder(x);
    for(int i = 0; i < 4; i++) {
      smoothMove(legServos[i], angleToPulse(shoulderAngle));
      cursangle[i]=shoulderAngle;
      smoothMove(legServos[i+4], angleToPulse(kneeAngle));
      curkangel[i]=kneeAngle;
    }
  }
  else {
    Serial.println("Invalid Height (40-84mm)");
  }
}

void wHeight(int i) {
    float kneeAngle = knee(50);
    float shoulderAngle = shoulder(50);
      smoothMove(legServos[i], angleToPulse(shoulderAngle));
      smoothMove(legServos[i+4], angleToPulse(kneeAngle));
}

int angleToPulse(double ang) {
  int pulse = map(ang, 0, 180, SMIN, SMAX);  // map angle of 0 to 180 to Servo min and Servo max
  return pulse;
}

void smoothMove(int servoNum, int newPos) {
  // Get the current position of the servo
  int currentPos = pwm.getPWM(servoNum);

  // Calculate the step size based on the difference between the current and new positions
  int stepSize = (newPos - currentPos) / 10;

  // Move the servo by the step size 10 times
  for (int i = 0; i < 10; i++) {
    pwm.setPWM(servoNum, 0, currentPos + stepSize);
    delay(20);
  }
  // Set the servo to the final position
  pwm.setPWM(servoNum, 0, newPos);
}

void walk() {
  adjustHeight(60);
  for(int j =0; j<5;j++)
  { 
    for(int i=1; i<5; i++)
    {
    wHeight(i);
    delay(100);
    smoothMove(legServos[i], angleToPulse(3.438262641938681));
    smoothMove(legServos[i+4], angleToPulse(75.772114945769));
    delay(100);
    adjustHeight(60);
    } 
  }
      for (int i=0;i<5;i++)
    {
      smoothMove(legServos[i], angleToPulse(inangle[i]));
      cursangle[i]=inangle[i];
      smoothMove(legServos[i+4], angleToPulse(inangle[i+4]));
      curkangel[i]=inangle[i+4];
    }
  
} 

void turnHeadLeft() {

  smoothMove(headServo, angleToPulse(0));
  delay(500);
  smoothMove(headServo, angleToPulse(90));
}

void turnHeadRight() {
  smoothMove(headServo, angleToPulse(180));
  delay(500);
  smoothMove(headServo, angleToPulse(90));
}

// void lookDown() {
//      smoothMove(neckServo, angleToPulse(30));
// }

// void lookUp() {
//   // Use the smoothMove() function to move the tail servo
//   // Example:
//   // smoothMove(tailServo, angleToPulse(150));
// }

void shakeHand() {
  // Write code to move legs in a coordinated manner to simulate shaking hand
}

void sit()
{
   int steps = 20;
  for (int i = 0; i < steps; i++) {
    smoothMove(hip1, hip1_pos, 5);
    smoothMove(hip2, hip2_pos, 5);
    smoothMove(knee1, knee1_pos, 5);
    smoothMove(knee2, kne
    delay(50);
  }
}

void sleep() {
  // Write code to move legs in a coordinated manner to simulate sleeping
}

void wakeUp() {
  // Write code to move legs in a coordinated manner to simulate waking up
}

void tailMovement() {
  // Write code to move tail servo to simulate tail movement
}

void setup() {
  Serial.begin(115200);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
  delay(5000);
  for(int i=0;i<4;i++)
  {
      smoothMove(legServos[i], angleToPulse(90));
      smoothMove(legServos[i+4], angleToPulse(90));
  }
}

void loop() {
  if (Serial.available() > 0) {
    char input = Serial.read();
    switch (input) {
      case 'h':
        Serial.println("enter new height");
        while (Serial.available()==0) {}
         int x = Serial.parseFloat();
        adjustHeight();
        break;
      case 'w':
        walk(step);
        Serial.println("Walking forward");
        break;
      case 's':
        walk(-step);
        Serial.println("Walking backward");
        break;
      case 'q':
        turnHeadLeft();
        Serial.println("Head turned left");
        break;
      case 'e':
        turnHeadRight();
        Serial.println("Head turned right");
        break;
      case 'r':
        lookDown();
        Serial.println("Looking down");
        break;
      case 'f':
        lookUp();
        Serial.println("Looking up");
        break;
      case 'a':
        shakeHand();
        Serial.println("Shaking hand");
        break;
      case 'd':
        sit();
        Serial.println("Sitting");
        break;
      case 'z':
        sleep();
        Serial.println("Sleeping");
        break;
      case 'x':
        wakeUp();
        Serial.println("Wake up");
        break;
      case 'c':
        tailMovement();
        Serial.println("Tail movement");
        break;
    }
  }
}
