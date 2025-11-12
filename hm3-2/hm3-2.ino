
const int r = 3;
const int buttonPin = 2;

enum Mode { NOFLASH, FLASH_LOW, FLASH_FAST, FLASH_MAXFAST };
int mode = NOFLASH;

// 去抖
int lastReading = HIGH, buttonState = HIGH;
unsigned long lastDebounce = 0;
const unsigned long debounceMs = 30;


bool ledOn = false;
unsigned long lastToggle = 0;

。
inline void ledOnWrite()  { digitalWrite(r, LOW);  }
inline void ledOffWrite() { digitalWrite(r, HIGH); }

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(r, OUTPUT);
  ledOffWrite();
}

void loop() {

  int reading = digitalRead(buttonPin);
  if (reading != lastReading) lastDebounce = millis();

  if (millis() - lastDebounce > debounceMs) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {          
        mode = (mode + 1) % 4;           

        ledOn = false;
        lastToggle = millis();
      }
    }
  }
  lastReading = reading;


  unsigned long now = millis();
  unsigned long interval = 0;

  switch (mode) {
    case NOFLASH:
      // 恆亮（active-low）
      ledOnWrite();
      break;

    case FLASH_LOW:
      interval = 1000;
      break;

    case FLASH_FAST:
      interval = 500;
      break;

    case FLASH_MAXFAST:
      interval = 100;
      break;
  }


  if (mode != NOFLASH) {
    if (now - lastToggle >= interval) {
      lastToggle = now;
      ledOn = !ledOn;
      if (ledOn) ledOnWrite(); else ledOffWrite();
    }
  }
}