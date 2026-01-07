const int buttonPin = 2;
const int r = 3;
const int g = 4;
const int b = 5;

int ledcolor = 0;

bool lastButtonState = HIGH;   // 上一次按鈕狀態
bool currentButtonState;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(r, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(b, OUTPUT);
}

void loop() {
  currentButtonState = digitalRead(buttonPin);

  // 偵測「剛按下」的瞬間（HIGH -> LOW）
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    ledcolor++;
    if (ledcolor > 4) {
      ledcolor = 0;
    }
  }

  lastButtonState = currentButtonState;

  // 顏色控制
  if (ledcolor == 0) {
    digitalWrite(r, HIGH);
    digitalWrite(g, LOW);
    digitalWrite(b, LOW);
  } 
  else if (ledcolor == 1) {
    digitalWrite(r, LOW);
    digitalWrite(g, HIGH);
    digitalWrite(b, LOW);
  } 
  else if (ledcolor == 2) {
    digitalWrite(r, LOW);
    digitalWrite(g, LOW);
    digitalWrite(b, HIGH);
  }
  else if (ledcolor == 3) {
    digitalWrite(r, HIGH);
    digitalWrite(g, LOW);
    digitalWrite(b, HIGH);
  }
  else if (ledcolor == 4) {
    digitalWrite(r, LOW);
    digitalWrite(g, LOW);
    digitalWrite(b, LOW);
  }
}