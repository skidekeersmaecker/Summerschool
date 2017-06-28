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
const int piezoThreshold = 100;

int sensorPiezo = 0;

//photocell
const int photoPin = A1;
const int photoSensorMin = 0;
const int photoSensorMax = 600;

//potentiometer
const int potPin = A2;

int potVal = 0;

//camera
int cameraFocus = 0;
int cameraTrigger = 0;
int flash = 0;

void setup() {
  Serial.begin(9600);

  ledOnSetup();
 
  PCICR |= (1 << PCIE0);    // set PCIE0 to enable PCMSK0 scan
  PCMSK0 |= (1 << PCINT3);
  sei();
  
}

void loop() {

  
  
  //readPhoto();
  //readPot();
  //readPiezo();
  //checkSwitch();
  //Serial.println(buttonState);
  delay(100);
}

volatile int lastButtonState;
ISR(PCINT0_vect) {
  int x = (PINB >> PINB3) & 0x1;
  Serial.println(x);
  if (x != lastButtonState) {
    if ((millis() - lastDebounceTime) > debounceDelay) {
      Serial.println("pressed");
      
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
  Serial.println(rangePhoto);
  switch (rangePhoto) {
    case 0:
      Serial.println("dark");
      break;
    case 1:
      Serial.println("dim");
      break;
    case 2:
      Serial.println("medium");
      break;
    case 3:
      Serial.println("bright");
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
  Serial.println(sensorPiezo);

  if (sensorPiezo >= piezoThreshold) {
    led1State = !led1State;
    digitalWrite(led1, led1State);
    Serial.println("Knock");
  }
}

void checkSwitch() {
  switchState = digitalRead(switchPin);
  if (switchState == HIGH) {
    led1State = HIGH;
    digitalWrite(led1, led1State);
    Serial.println("switch is HIGH");
  }
  else {
    Serial.println("switch is LOW");
    led1State = LOW;
    digitalWrite(led1, led1State);
  }
}

void hit() {
  buttonState = !buttonState;
  Serial.print("pressed");
}

void ledOnSetup(){
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(switchPin, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  digitalWrite(led1, led1State);
  digitalWrite(led2, led2State);
  delay(500);
  digitalWrite(led1, !led1State);
  digitalWrite(led2, !led2State);
  delay(500);
  digitalWrite(led1, led1State);
  digitalWrite(led2, led2State);
  delay(500);
  digitalWrite(led1, !led1State);
  digitalWrite(led2, !led2State);
  delay(500);
  digitalWrite(led1, led1State);
  digitalWrite(led2, led2State);
  delay(500);
  led1State = LOW;
  led2State = LOW;
  digitalWrite(led1, led1State);
  digitalWrite(led2, led2State);
}


