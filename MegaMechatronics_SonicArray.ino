// ---------------------------------------------------------------------------
// Example NewPing library sketch that does a ping about 20 times per second.
// ---------------------------------------------------------------------------

#include <NewPing.h>
#include <Servo.h> 

const int trig1 = 2;
const int echo1 = 3;
const int trig2 = 4;
const int echo2 = 5;
const int trig3 = 6;
const int echo3 = 7;
const int trig4 = 8;
const int echo4 = 9;
const int trig5 = 14;
const int echo5 = 15;
const int trig6 = 16;
const int echo6 = 17;
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

Servo myservo;

NewPing sonar1(trig1, echo1, MAX_DISTANCE);// NewPing setup of pins and maximum distance.
NewPing sonar2(trig2, echo2, MAX_DISTANCE);
NewPing sonar3(trig3, echo3, MAX_DISTANCE);
NewPing sonar4(trig4, echo4, MAX_DISTANCE);
NewPing sonar5(trig5, echo5, MAX_DISTANCE);
NewPing sonar6(trig6, echo6, MAX_DISTANCE);

const int pingDelay = 50; //ms between pings
const int adjacent = 3; //number of inches to split difference between 2 US sensors
int minDist;
int target;
int targetServo;
unsigned long timer;

void setup() {
  myservo.attach(10);
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  timer = millis() + 3000;
}

void loop() {
  //firing order 1, 3, 5, 2, 6, 4
  unsigned int uS1 = sonar1.ping() / 74 / 2; // Send ping, get ping time in microseconds (uS) (divide by 74 and 2 to get INCHES)
  uS1 = constrain(uS1, 0, 75);
  if(uS1 < 2){
    uS1 = 75;
  }
  delay(pingDelay);
  
  unsigned int uS3 = sonar3.ping() / 74 / 2;
  uS3 = constrain(uS3, 0, 75);
  if(uS3 < 2){
    uS3 = 75;
  }
  delay(pingDelay);
  
  unsigned int uS5 = sonar5.ping() / 74 / 2;
  uS5 = constrain(uS5, 0, 75);
  if(uS5 < 2){
    uS5 = 75;
  }
  delay(pingDelay);
  
  unsigned int uS2 = sonar2.ping() / 74 / 2;
  uS2 = constrain(uS2, 0, 75);
  if(uS2 < 2){
    uS2 = 75;
  }
  delay(pingDelay);
  
  unsigned int uS6 = sonar6.ping() / 74 / 2;
  uS6 = constrain(uS6, 0, 75);
  if(uS6 < 2){
    uS6 = 75;
  }
  delay(pingDelay);
  
  unsigned int uS4 = sonar4.ping() / 74 / 2;
  uS4 = constrain(uS4, 0, 75);
  if(uS4 < 2){
    uS4 = 75;
  }
  delay(pingDelay);
//Get minimum distance measured, to detect nearest person
  if(uS1 < uS2 && uS1 < uS3 && uS1 < uS4 && uS1 < uS5 && uS1 < uS6) minDist = 1;
  if(uS2 < uS1 && uS2 < uS3 && uS2 < uS4 && uS2 < uS5 && uS2 < uS6) minDist = 2;
  if(uS3 < uS2 && uS3 < uS1 && uS3 < uS4 && uS3 < uS5 && uS3 < uS6) minDist = 3;
  if(uS4 < uS2 && uS4 < uS3 && uS4 < uS1 && uS4 < uS5 && uS4 < uS6) minDist = 4;
  if(uS5 < uS2 && uS5 < uS3 && uS5 < uS4 && uS5 < uS1 && uS5 < uS6) minDist = 5;
  if(uS6 < uS2 && uS6 < uS3 && uS6 < uS4 && uS6 < uS5 && uS6 < uS1) minDist = 6;
//Set target angle or position to move robot
if(minDist == 1)
{
  if(uS1 > uS2 - adjacent) target = 15; //-adjacent is fine tuning angle or position 
  else target = 10;
}
else if(minDist == 2)
{
  if(uS2 > uS1 - adjacent)target = 15; 
  else if(uS2 > uS3 - adjacent) target = 25;
  else target = 20;
}
else if(minDist == 3)
{
  if(uS3 > uS2 - adjacent) target = 25;
  else if(uS3 > uS4 - adjacent) target = 35;
  else target = 30;
}
else if(minDist == 4)
{
  if(uS4 > uS3 - adjacent) target = 35;
  else if(uS4 > uS5 - adjacent) target = 45;
  else target = 40;
}
else if(minDist == 5)
{
  if(uS5 > uS6 - adjacent) target = 55;
  else if(uS5 > uS4 - adjacent) target = 45;
  else target = 50;
}
else if(minDist == 6)
{
  if(uS6 > uS5 - adjacent) target = 55;
  else target = 60;
}

if(timer < millis())
{
  if(uS1 > 74 && uS2 > 74 && uS3 > 74 && uS4 > 74 && uS5 > 74 && uS6 > 74)
  {
    myservo.write(90); //center angle or position, must calibrate
    Serial.println("centering");
  }
  else
  {
  targetServo = map(target, 0, 60, 30, 150); //map target to command window, must calibrate
  myservo.write(targetServo);
  }
  timer = millis() + 2000;
  Serial.println("servo update");
}
//debug
  Serial.print(minDist);
  Serial.print(" ");
  Serial.print(target);
  Serial.print(" ");
  Serial.print(uS1);
  Serial.print(", ");
  Serial.print(uS2);
  Serial.print(", ");
  Serial.print(uS3);
  Serial.print(", ");
  Serial.print(uS4);
  Serial.print(", ");
  Serial.print(uS5);
  Serial.print(", ");
  Serial.print(uS6);
  Serial.println();
}
