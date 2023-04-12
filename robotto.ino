#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define SMIN 150       // This is the 'minimum' pulse length count (out of 4096)
#define SMAX 600       // This is the 'maximum' pulse length count (out of 4096)
#define SERVO_FREQ 50  // Analog servos run at ~50 Hz updates

const float FRS = 1; const float FRK = 2; const float FLS = 3; const float FLK = 4;
const float BRS = 5; const float BRK = 6; const float BLS = 7; const float BLK = 8;
const float HEAD = 9; const float NECK = 10; const float TAIL = 11;

const float a=50.6; //in mm
const float b=58.2; //in mm
float c;  //max and min height is 84 and 40
float inangle[8]={};

double knee(float c)
{
  double ang1,deg1;
  double height ;
   height= ((a*a)+(b*b)-(c*c))/(2*a*b);
   ang1= acos(height);
   deg1 = (height * 4068) / 71;
   Serial.println(deg1);
   return deg1;
}

double shoulder(float c)
{
  double ang2,deg2;
  double height ;
   height=((c*c)+(a*a)-(b*b))/(2*a*c);
   ang2= acos(height);
   deg2 = (height * 4068) / 71;
   Serial.println(deg2);
   return deg2;
}

int angleToPulse(double ang) 
{
  int pulse = map(ang, 0, 180, SMIN, SMAX);  // map angle of 0 to 180 to Servo min and Servo max
  return pulse;
}

void heightcontrol()
{
  double x,y;
  while (Serial.available()==0)
   {}
  c=Serial.parseFloat();
  if(c>=40&&c<=84)
  {
  x= knee(c);
  y= shoulder(c);
 for(int i=0;i<4;i++)
  pwm.setPWM(i, 0, angleToPulse(y));
 for(int j=4;j<9;j++)
  pwm.setPWM(j, 0, angleToPulse(x));
  }
  else
  Serial.println("not valid height (40-84mm)");
}

void setup()
{
  Serial.begin(115200);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
  delay(10);
}

void loop()
{ 
  heightcontrol();
}
