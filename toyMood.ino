const int RledPin = 3;
const int GledPin = 4;
const int BledPin = 5;
const int buttonPin = 2;

int mood = 0;
const int neutralMood = 10;
unsigned long previousMillis = 0;
int fadingDirection = 1;
bool ButtonPressed = false;
int buttonState = 0;
unsigned long touchedTimer = 0;
unsigned long reducedTimer = 0;
const long unTouchInterval = 5000;
const long reducedInterval = 1000;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(RledPin, OUTPUT);
  pinMode(GledPin, OUTPUT);
  pinMode(BledPin, OUTPUT);

  mood = neutralMood;
}

void loop() {
  // put your main code here, to run repeatedly:
  showLEDState(mood);
  buttonState = digitalRead(buttonPin);

  if(buttonState == HIGH && !ButtonPressed){
    mood = mood + 1;
    if(mood > 20) mood = 20;
    touchedTimer = millis();
    ButtonPressed = true;
  }
  if(buttonState ==LOW && ButtonPressed){
    ButtonPressed = false;
  }
  unsigned long currentTimer = millis();
  if(currentTimer - touchedTimer > unTouchInterval){
    if(currentTimer - reducedTimer > reducedInterval){
      mood = mood - 1;
      if(mood < 0 ) mood = 0;
      reducedTimer = currentTimer;
    }
}
}

void showLEDState(int mood) {
  float brightnessInterval = 255.0 / 10.0;
  int diff = abs(mood - neutralMood);
  int value = constrain(brightnessInterval * diff, 0, 255);

  if (mood >= neutralMood) {
    analogWrite(RledPin, 0);
    analogWrite(GledPin, value);
    analogWrite(BledPin, 255 - value);
  } 
  else {
    analogWrite(RledPin, 255 - value);
    analogWrite(GledPin, value);
    analogWrite(BledPin, 0);
  }
}

