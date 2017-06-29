//buttons & leds
const int buttonPin = 11;
const int switchPin = 12;
const int led1 = 6;
const int led2 = 7;

int buttonState = 0;
int switchState = 0;
int led1State = HIGH;
int led2State = HIGH;
//debounce on button
long lastDebounceTime = 0;
long debounceDelay = 200;

//Piezo element
const int piezoPin = A0;
const int piezoThreshold = 1000;

int sensorPiezo = 0;
boolean piezoActive = false;

//photocell
const int photoPin = A1;
const int photoSensorMin = 0;
const int photoSensorMax = 600;

boolean photoActive = false;

//potentiometer
const int potPin = A2;

int potVal = 0;

//camera
const int cameraFocus = 10;
const int cameraTrigger = 9;
const int flash = 8;

//syncing
boolean receivedNothing = true;
const int slave1 = 2;
const int slave2 = 3;

boolean slave1Active = false;
long slave1Prev = 0;
boolean slave2Active = false;
long slave2Prev = 0;
int slaveDelay = 1000;

void setup() {
  Serial.begin(9600);

  initializeComponents();
  ledOnSetup();
  //interrupt
  PCICR |= (1 << PCIE0);
  PCMSK0 |= (1 << PCINT3);
  sei();
}

void loop() {
  readPiezo();
  readPhoto();
  checkSwitch();

  Serial.print("piezo: ");
  Serial.print(piezoActive);
  Serial.print(", photo: ");
  Serial.print(photoActive);
  Serial.print(", slave1: ");
  Serial.print(digitalRead(slave1));
  Serial.print(", slave2: ");
  Serial.println(digitalRead(slave2));

  if (digitalRead(slave1) == HIGH) {
    slave1Active = true;
    slave1Prev = millis();
  }
  if (digitalRead(slave2) == HIGH) {
    slave2Active = true;
    slave2Prev = millis();
  }
  if (millis() - slave1Prev > slaveDelay){
    slave1Active = false;
  }
  if (millis() - slave2Prev > slaveDelay){
    slave2Active = false;
  }
  if (digitalRead(slave1) && digitalRead(slave2) && piezoActive && photoActive) {
    triggerCamera();
  }
  //triggerCamera();
  //readPhoto();
  //readPot();
  //readPiezo();
  //checkSwitch();
  //delay(3000);
}

void triggerCamera() {
  digitalWrite(cameraTrigger, HIGH);
  delay(5);
  digitalWrite(cameraTrigger, LOW);
  Serial.println("FOTO");
}

volatile int lastButtonState;
ISR(PCINT0_vect) {
  int x = (PINB >> PINB3) & 0x1;
  if (x != lastButtonState) {
    if ((millis() - lastDebounceTime) > debounceDelay) {
      triggerCamera();
      digitalWrite(led2, not led2State);
      delay(200);
      digitalWrite(led2, led2State);
      lastDebounceTime = millis();
    }
    lastButtonState = x;
  }
}

void readPhoto() {
  int sensorPhoto = analogRead(photoPin);
  int rangePhoto = map(sensorPhoto, photoSensorMin, photoSensorMax, 0, 3);
  //Serial.println(rangePhoto);
  switch (rangePhoto) {
    case 0:
      //Serial.println("dark");
      photoActive = false;
      break;
    case 1:
      //Serial.println("dim");
      photoActive = false;
      break;
    case 2:
      //Serial.println("medium");
      photoActive = false;
      break;
    case 3:
      //Serial.println("bright");
      photoActive = true;
      break;
  }
}

void readPot() {
  potVal = analogRead(potPin);
  Serial.println(potVal);
  led2State = !led2State;
  digitalWrite(led2, led2State);
  delay(potVal);
}

void readPiezo() {
  sensorPiezo = analogRead(piezoPin);
  //Serial.println(sensorPiezo);

  if (sensorPiezo <= piezoThreshold) {
    piezoActive = true;
    led1State = !led1State;
    digitalWrite(led1, led1State);
    //Serial.println("Knock");
  } else {
    piezoActive = false;
  }
}

void checkSwitch() {
  switchState = digitalRead(switchPin);
  if (switchState == HIGH) {
    led1State = HIGH;
    digitalWrite(led1, led1State);
    //Serial.println("switch is HIGH");
    digitalWrite(cameraFocus, HIGH);
  }
  else {
    //Serial.println("switch is LOW");
    led1State = LOW;
    digitalWrite(led1, led1State);
    digitalWrite(cameraFocus, LOW);
  }
}

void hit() {
  buttonState = !buttonState;
  Serial.print("pressed");
}

void initializeComponents() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(switchPin, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(cameraFocus, OUTPUT);
  pinMode(cameraTrigger, OUTPUT);
  pinMode(flash, OUTPUT);
  pinMode(slave1, INPUT);
  pinMode(slave2, INPUT);
}

void ledOnSetup() {
  digitalWrite(led1, led1State);
  digitalWrite(led2, led2State);
  delay(250);
  digitalWrite(led1, !led1State);
  digitalWrite(led2, !led2State);
  delay(250);
  digitalWrite(led1, led1State);
  digitalWrite(led2, led2State);
  delay(250);
  digitalWrite(led1, !led1State);
  digitalWrite(led2, !led2State);
  delay(250);
  digitalWrite(led1, led1State);
  digitalWrite(led2, led2State);
  delay(250);
  led1State = LOW;
  led2State = LOW;
  digitalWrite(led1, led1State);
  digitalWrite(led2, led2State);
}


