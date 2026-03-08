#include <math.h>

#define pinQ1 13
#define pinQ2 12
#define pinQ3 11
#define pinQ4 10
#define pinQ5 8
#define pinQ6 7
#define pinQ7 9
#define pinQ8 6

#define steerPin A0
#define CWPin 5
#define stopPin 4
#define ACWPin 3

int steerAngle;
float v = 0.56, L = 0.15, d = 0.30, r = 0.06;
float wL, wR;
void setup() {
  
  pinMode(pinQ1, OUTPUT);
  pinMode(pinQ2, OUTPUT);
  pinMode(pinQ3, OUTPUT);
  pinMode(pinQ4, OUTPUT);
  pinMode(pinQ5, OUTPUT);
  pinMode(pinQ6, OUTPUT);
  pinMode(pinQ7, OUTPUT);
  pinMode(pinQ8, OUTPUT);
  pinMode(CWPin, INPUT_PULLUP);
  pinMode(stopPin, INPUT_PULLUP);
  pinMode(ACWPin, INPUT_PULLUP);
  
}

void loop() {
  
  steerAngle = map(analogRead(steerPin), 0, 1023, -45, 45);
  float radAngle = steerAngle * (PI/180.0);
  
  if(radAngle<0) {
    float radius = L/tan(-radAngle);
    wL = (v*(radius-(d/2))*tan(-radAngle))/(L*r);
    wR = (v*(radius+(d/2))*tan(-radAngle))/(L*r);
  }
  else if(radAngle>0) {
    float radius = L/tan(radAngle);
    wL = (v*(radius+(d/2))*tan(radAngle))/(L*r);
    wR = (v*(radius-(d/2))*tan(radAngle))/(L*r);
  }
  else {
    wL = v/r;
    wR = v/r;
  }
  float rpmL = wL*60/(2*PI);
  float rpmR = wR*60/(2*PI);
  

  int PWMSpeedL = rpmL*255.0/180.0;
  PWMSpeedL = constrain(PWMSpeedL, 0, 255);

  int PWMSpeedR = rpmR*255.0/180.0;
  PWMSpeedR = constrain(PWMSpeedR, 0, 255);
  Serial.println(PWMSpeedL);
  
  if(digitalRead(CWPin)==LOW) {
    digitalWrite(pinQ1, HIGH);
    digitalWrite(pinQ2, LOW);
    analogWrite(pinQ3, 0);
    analogWrite(pinQ4, PWMSpeedL);
    digitalWrite(pinQ5, HIGH);
    digitalWrite(pinQ6, LOW);
    analogWrite(pinQ7, 0);
    analogWrite(pinQ8, PWMSpeedR);
    
  }
  else if(digitalRead(stopPin)==LOW) {
    digitalWrite(pinQ1, HIGH);
    digitalWrite(pinQ2, HIGH);
    analogWrite(pinQ3, 0);
    analogWrite(pinQ4, 0);
    digitalWrite(pinQ5, HIGH);
    digitalWrite(pinQ6, HIGH);
    analogWrite(pinQ7, 0);
    analogWrite(pinQ8, 0);
    
  }
  else if(digitalRead(ACWPin)==LOW) {
    digitalWrite(pinQ1, LOW);
    digitalWrite(pinQ2, HIGH);
    analogWrite(pinQ3, PWMSpeedL);
    analogWrite(pinQ4, 0);
    digitalWrite(pinQ5, LOW);
    digitalWrite(pinQ6, HIGH);
    analogWrite(pinQ7, PWMSpeedR);
    analogWrite(pinQ8, 0);
    
  }

}
