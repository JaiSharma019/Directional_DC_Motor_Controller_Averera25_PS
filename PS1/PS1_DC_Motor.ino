#define pinQ1 4
#define pinQ2 7
#define pinQ4 6
#define pinQ3 5
#define ACWPin 8
#define CWPin 10
#define stopPin 9
#define potPin A0
#define thresholdPin1 A1
#define thresholdPin2 A2
#define encoderA 2
#define encoderB 3
volatile long pos = 0;
unsigned long now = 0;
const int threshold = 563; // Threshold voltage = 2.5V + (2.5A)*(0.1V/A) = 2.75V
// mapping 2.75 for 1023
int pot;

void setup() {
  pinMode(pinQ1, OUTPUT);
  pinMode(pinQ2, OUTPUT);
  pinMode(pinQ3, OUTPUT);
  pinMode(pinQ4, OUTPUT);
  pinMode(encoderA, INPUT);
  pinMode(encoderB, INPUT);
  pinMode(CWPin, INPUT_PULLUP);
  pinMode(ACWPin, INPUT_PULLUP);
  pinMode(stopPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoderA),encode, RISING);
  Serial.begin(9600);
  
}

void loop() {
  
  volatile bool shootThrough = false;
  
  const int interval = 100;
  Serial.print("Position: ");
  float angle = (pos*15.0);
  Serial.println(angle);

  if(millis()-now>=interval) {
    now = millis();
    if((analogRead(thresholdPin1)>threshold) || (analogRead(thresholdPin2)>threshold)) {
      shootThrough = true;
      shutDown();
      Serial.println("Status: Shoot Through");
//      delay(50);
    }
  }
  pot = analogRead(potPin);
  int PWM = map(pot, 0, 1023, 0, 255);

  
  if(digitalRead(CWPin)==LOW && shootThrough==false) {
    Serial.println("Direction: Clockwise");
    analogWrite(pinQ3, PWM);
    digitalWrite(pinQ2, HIGH);
    digitalWrite(pinQ1, LOW);
    analogWrite(pinQ4, 0);
    
  }
  else if(digitalRead(ACWPin)==LOW && shootThrough==false) {
    Serial.println("Direction: Anti-clockwise");
    analogWrite(pinQ4, PWM);
    digitalWrite(pinQ1, HIGH);
    digitalWrite(pinQ2, LOW);
    analogWrite(pinQ3, 0);
    
    
  }
  else if(digitalRead(stopPin)==LOW && shootThrough==false) {
    Serial.println("Status: Stopped");
    shutDown();
    
  }
}
  

  

void encode() {
  if(digitalRead(stopPin)==HIGH) {
  int b = digitalRead(encoderB);
//  int a = digitalRead(encoderA);
  if( b==HIGH) {
    pos++;
    
  }
  else {
    pos--;
    
  }

  }
}

void shutDown() {
  digitalWrite(pinQ1, LOW);
  digitalWrite(pinQ2, LOW);
  digitalWrite(pinQ3, HIGH);
  digitalWrite(pinQ4, HIGH);
  
}
