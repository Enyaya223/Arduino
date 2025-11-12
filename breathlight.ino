// === Pins ===
const int buttonPin = 2;
const int r = 3, g = 10, b = 5;     // 共陽RGB：LOW亮、HIGH滅（3、5 可PWM）

// === Button debouncing ===
int stableState = HIGH;            // 去抖後狀態，高=放開
int lastReading = HIGH;
unsigned long lastDebounce = 0;
const unsigned long debounceMs = 30;

// === Modes ===
enum Mode { CHANGE_COLOR = 0, FLASH_YELLOW = 1, FLASH_RED = 2 };
int mode = CHANGE_COLOR;
int prevMode = CHANGE_COLOR;       // 記住切入「按住」前的模式

// === Short press vs hold ===
bool pressed = false;
unsigned long pressStart = 0;
const unsigned long clickMaxMs = 200;     // ≤200ms 視為短按（切模式）
const unsigned long holdToBreatheMs = 1500; // 按住≥1.5s 才進入呼吸燈

// === Timers for modes ===
unsigned long t = 0;
const unsigned long changeEvery = 2000;   // 變色每2秒
const unsigned long flashEvery  = 500;    // 閃爍間隔
int  colorIndex = 0;                      // 0=綠 1=黃 2=紅
bool onFlag = false;

// === Breathing (按住才用) ===
unsigned long lastBreathStep = 0;
const unsigned long breathStepMs = 8;     // 呼吸步進(越小越平滑)
int  breathVal = 0;                       // 0..255
int  breathDir = +1;                      // +1 變亮 / -1 變暗
bool breatheRed = true;                   // 每次完整呼吸切換紅/藍

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(r, OUTPUT); pinMode(g, OUTPUT); pinMode(b, OUTPUT);
  allOff();
}

void loop() {
  // -- 去抖與邊緣偵測 --
  int reading = digitalRead(buttonPin);
  if (reading != lastReading) {
    lastDebounce = millis();
  }
  if (millis() - lastDebounce > debounceMs) {
    if (reading != stableState) {
      stableState = reading;

      if (stableState == LOW) {        // 按下瞬間
        pressed = true;
        pressStart = millis();
        prevMode = mode;               // 記住原模式
        renderBreathing();              // 準備可能進入呼吸
      } else {                         // 放開瞬間
        unsigned long dur = millis() - pressStart;
        if (pressed && dur <= clickMaxMs) {   // 短按：切模式
          mode = (mode + 1) % 3;
          resetModes();
        }
        pressed = false;               // 放開後回到 prevMode 或新 mode
      }
    }
  }
  lastReading = reading;

  // -- 按住：只有超過1.5秒才進呼吸；未滿就保持全暗 --
  if (pressed) {
    unsigned long heldTime = millis() - pressStart;
    if (heldTime >= holdToBreatheMs) {
      renderBreathing();
      return;
    } else {
      allOff();    // 按住但未滿1.5秒 → 全暗
      return;
    }
  }

  // -- 放開：依目前模式顯示（非阻塞） --
  unsigned long now = millis();
  switch (mode) {

    case FLASH_YELLOW:
      if (now - t >= flashEvery) { onFlag = !onFlag; t = now; }
      if (onFlag) setYellow(); else allOff();
      break;

    case FLASH_RED:
      if (now - t >= flashEvery) { onFlag = !onFlag; t = now; }
      if (onFlag) setRed(); else allOff();
      break;
  }
}

/* ===== LED helpers (共陽：LOW亮 / HIGH滅) ===== */
void setRGB(bool R, bool G, bool B) {
  digitalWrite(r, R ? LOW : HIGH);
  digitalWrite(g, G ? LOW : HIGH);
  digitalWrite(b, B ? LOW : HIGH);
}
void allOff()    { setRGB(false,false,false); }
void setRed()    { setRGB(true,  false,false); }
void setGreen()  { setRGB(false, true, false); }
void setYellow() { setRGB(true,  true, false); }
void setBlue()   { setRGB(false, false,true ); }

void renderColor(int idx) {
  if (idx == 0) setGreen();
  else if (idx == 1) setYellow();
  else setRed();
}

void resetModes() {
  breathVal = 0; 
  breathDir = +1; 
  breatheRed = true;
  lastBreathStep = millis();

  // 一開始紅燈從暗到亮
  digitalWrite(b, HIGH);   
  analogWrite(r, 255);
  analogWrite(g, 255);
}

void renderBreathing() {
  unsigned long now = millis();
  if (now - lastBreathStep >= breathStepMs) {
    lastBreathStep = now;
    breathVal += breathDir;                 
    if (breathVal >= 255) {                 
      breathVal = 255; 
      breathDir = -1;
    } else if (breathVal <= 0) {           
      breathVal = 0; 
      breathDir = +1; 
      breatheRed = !breatheRed;             
    }
  }

  // 共陽 → PWM 反相
  if (breatheRed) {
    analogWrite(r, 255 - breathVal);                  
    analogWrite(g, 255 - breathVal);
    digitalWrite(b, HIGH);                  
  } else {
    analogWrite(b, 255 - breathVal);       
    analogWrite(r, 255 - breathVal);
    digitalWrite(g, HIGH);
  }
  
}

  

