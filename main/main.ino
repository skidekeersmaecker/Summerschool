//buttons & leds
const int buttonPin = 11;
const int switchPin = 12;
const int led1 = 6;
const int led2 = 7;

int switchState = 0;
int buttonState = 0;
int led1State = LOW;
int led2State = LOW;

//potentiometer
int potPin = 2;
int potVal = 0;

//Piezo element
const int knockSensor = A0;
const int threshold = 100;

int sensorPiezo = 0;

void setup() {
  Serial.begin(9600);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(switchPin, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  digitalWrite(led1, led1State);
  digitalWrite(led2, led2State);

  attachInterrupt(digitalPinToInterrupt(buttonPin), hit, RISING);
}

void loop() {

  //readPot();
  //readPiezo();
  //checkSwitch();
  delay(100);
}

void readPot(){
  potVal = analogRead(potPin);
  Serial.println(potVal);
  led2State = !led2State;
  digitalWrite(led2, led2State);
  delay(potVal);
}

void readPiezo(){
  sensorPiezo = analogRead(knockSensor);
  Serial.println(sensorPiezo);

  if (sensorPiezo >= threshold) {
    led1State = !led1State;
    digitalWrite(led1, led1State);
    Serial.println("Knock");
  }
}

void checkSwitch() {
  switchState = digitalRead(switchPin);
  if (switchState == HIGH) {
    Serial.println("switch is HIGH");
  }
  else {
    Serial.println("switch is LOW");
  }
}

void hit() {
  buttonState = !buttonState;
}


