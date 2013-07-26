#define RND A0
#define MOTOR_GEAR1 7
#define MOTOR_GEAR2 8
#define MOTOR_GEAR_PWM 9
#define SW_GEAR1 10
#define SW_GEAR2 11

boolean rotateDirection = false;
boolean rotateState = false;
boolean activeSwitch = false;
unsigned long duration = 0;
unsigned long interval = 0;
unsigned long timeStampRotate = 0;
unsigned long timeStampSwitch = 0;

//seconds
const int durationMin = 15;
const int durationMax = 30;
const long intervalMin = 180;
const long intervalMax = 300;
const int debounceDelay = 2000;

void setup(){
  Serial.begin(9600);

  pinMode(MOTOR_GEAR1, OUTPUT);
  pinMode(MOTOR_GEAR2, OUTPUT);
  pinMode(MOTOR_GEAR_PWM, OUTPUT);
  pinMode(SW_GEAR1, INPUT);
  pinMode(SW_GEAR2, INPUT);

  randomSeed(analogRead(RND));
  duration = random(durationMin, durationMax) * 1000;
  interval = random(intervalMin, intervalMax) * 1000;
}

void loop(){

  if(timeStampRotate + interval < millis() &&
    millis() < timeStampRotate + interval + duration) {
    rotateState = true;
  } 
  else if(timeStampRotate + interval + duration < millis()){
    rotateState = false;
    duration = random(durationMin, durationMax) * 1000;
    interval = random(intervalMin, intervalMax) * 1000;
    timeStampRotate = millis();
  }

  if(rotateState){
    analogWrite(MOTOR_GEAR_PWM, 255);  
    if(rotateDirection){
      digitalWrite(MOTOR_GEAR1, HIGH);
      digitalWrite(MOTOR_GEAR2, LOW);
    }
    else{
      digitalWrite(MOTOR_GEAR1, LOW);
      digitalWrite(MOTOR_GEAR2, HIGH);
    }
  }
  else{
    analogWrite(MOTOR_GEAR_PWM, 0);  
    digitalWrite(MOTOR_GEAR1, LOW);
    digitalWrite(MOTOR_GEAR2, LOW);
  }

  if(timeStampSwitch + debounceDelay < millis()){
    activeSwitch = true;
  }

  if(digitalRead(SW_GEAR1) == 1 && activeSwitch){
    rotateDirection = !rotateDirection;
    timeStampSwitch = millis();
    activeSwitch = false;
  }

  Serial.print("state = ");
  Serial.print(rotateState);
  Serial.print(", direction = ");
  Serial.print(rotateDirection);
  Serial.print(", interval = ");
  Serial.print(interval);
  Serial.print(", duration = ");
  Serial.print(duration);
  Serial.print(", timeStampRotate = ");
  Serial.print(timeStampRotate);
  Serial.print(", millis = ");
  Serial.println(millis());

  //  Serial.print(digitalRead(SW_GEAR1));
  //  Serial.print(" ");
  //  Serial.println(digitalRead(SW_GEAR2));
}



