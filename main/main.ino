const int buttonPin = 11;
const int switchPin = 12;
const int led1 = 6;
const int led2 = 7;

int switchState = 0;
int buttonState = 0;

void setup() {
  Serial.begin(9600);

  pinMode(buttonPin, INPUT);
  pinMode(switchPin, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);

  attachInterrupt(buttonPin, hit, RISING);
}

void loop() {
  switchState = digitalRead(switchPin);
  if (switchState == HIGH) {
    Serial.println("switch is HIGH");
  }
  else {
    Serial.println("switch is LOW");
  }

  Serial.print("buttonState: ");
  Serial.println(buttonState);
  delay(2000);
}

void hit() {
  buttonState = 1;
}


