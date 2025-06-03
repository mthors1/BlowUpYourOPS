const int segmentPins[] = {15, 14, 1, 0, 11, 10, 9, 8}; // A-G, DP
const int digitPins[] = {7, 6, 16, 17};                   // Digit 1–4
const int buzzerPin = 18;                                 // Buzzer
const int buttonPin = 26;


// LED pins: [Bottom-left RED, Top-right RED, Bottom-right YELLOW, Top-left YELLOW]
const int ledPins[] = {19, 20, 21, 22}; // BL Red, TR Red, BR Yellow, TL Yellow


bool wireGameDone = false;
bool memory_done = false;
bool memory_rgb_done = false;
bool codeGameMode = false;



bool initialized = false;
bool finalBuzzed = false;
int totalSeconds = 2 * 60;


const byte digits[10] = {
  0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110,
  0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01101111
};


// Forward declarations
int getDigitValue(int digitIndex);
void showDigit(int digitIndex, int num);
void clearDisplay();


void setup() {
  delay(3000);
  for (int i = 0; i < 8; i++) pinMode(segmentPins[i], OUTPUT);
  for (int i = 0; i < 4; i++) pinMode(digitPins[i], OUTPUT);
  for (int i = 0; i < 4; i++) pinMode(ledPins[i], OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  Serial.begin(115200);
  Serial.println("Serial initialized");

  Serial1.setTX(12);
  Serial1.setRX(13);
  Serial1.begin(115200);
  Serial.println("Serial 1 initialized");

  Serial2.setTX(4);
  Serial2.setRX(5);
  Serial2.begin(115200);
  Serial.println("Serial 2 initialized");
  delay(3000);
  while (!Serial1.available() || !Serial2.available()) {
    Serial1.write((uint8_t)0);
    Serial2.write((uint8_t)0);
    Serial.println("Serials not connected");
    Serial.print("Serial 1:  ");
    Serial.println(Serial1.read());
    Serial.print("Serial 2:  ");
    Serial.println(Serial2.read());
  }
}


void loop() {
  Serial.println("Entered loop");
  
  while (digitalRead(buttonPin) == HIGH && !initialized) {
    // LCD "Press to start" Display
    Serial.println("Not pushed");
    Serial1.write((uint8_t)0);
    Serial2.write((uint8_t)0);

  }


  // Wait for signal to begin
  if (!initialized) {
    initialized = true;
    finalBuzzed = false;
    totalSeconds = 5 * 60;

  }

  if (Serial2.available() && Serial1.available()) {
    uint8_t Serial2Received = Serial2.read();
    uint8_t Serial1Received = Serial1.read();
    Serial2.write((uint8_t)1);
    if(!wireGameDone) {
      // LCD Display for wires
      if (Serial2Received == 3) {
        totalSeconds = max(0, totalSeconds - 30); // prevent underflow
        }
      else if (Serial2Received == 4) {
        wireGameDone = true;
        // LCD for memory game
        Serial1.write((uint8_t)1);
      }
    }
    else {
      // Going to other controller
      if (!memory_rgb_done){
        Serial1.write((uint8_t)1);
        // Serial1.write((uint8_t)1);
        if (Serial1Received == 4 && !memory_done){
          memory_done = true;
          //LCD Display for rgb game
        }
        if (Serial1Received == 5 && memory_done){
          memory_rgb_done = true;
          // LCD Display for maze
        }
      }
      else{
        Serial2.write((uint8_t)6);
        if (Serial2Received == 7 && !codeGameMode){
          codeGameMode = true;
        }
        if (codeGameMode){
          if (Serial2Received == 1){
            // LCD Display 1
          }
          else if (Serial2Received == 2){
            // LCD Display 2
          }
          else if (Serial2Received == 3){
            // LCD Display 3
          }
          else if (Serial2Received == 4){
            // LCD Display 4
          }
          else if (Serial2Received == 5){
            // LCD Display 5
          }
          else if (Serial2Received == 6){
            // LCD Display 6
          }
          else if (Serial2Received == 7){
            // LCD Display 7
          }
          else if (Serial2Received == 8){
            // LCD Display 8
          }
          if (Serial2Received == 9){
            // LCD DISPLAY "YOU DEFUSED THE BOMB"
              // ── 1. Latch WIN state and freeze timer ──────────────────
            static bool winLatched = false;
            if (!winLatched) {
                winLatched  = true;
                finalBuzzed = false;   // ensure normal “explosion” path is disabled
                noTone(buzzerPin);     // silence anything currently playing
            }

            // ── 2. Show frozen time (display code already handles this) ──
            // Blink decimal points so the freeze is obvious
            static unsigned long dpTimer = 0;
            if (millis() - dpTimer >= 500) {  // 2 Hz blink
                dpTimer = millis();
                static bool dpState = false;
                dpState = !dpState;
                digitalWrite(segmentPins[7], dpState);   // DP segment common to all digits
            }

            // ── 3. Light all four panel LEDs solid ────────────────────
            for (int i = 0; i < 4; i++) digitalWrite(ledPins[i], HIGH);

            // ── 4. One‑time 5‑second victory tune (non‑blocking) ──────
            const int winNotes[] = { 988, 1175, 1319, 1568, 1760, 1976, 2349, 2637, 0 }; // nice rising scale
            const int winDur[]   = { 400, 300, 300, 300, 300, 300, 300, 300, 400 };      // totals ~4.8 s
            static uint8_t  tuneStep  = 0;
            static unsigned long tuneTimer = 0;
            static bool tuneDone = false;

            if (!tuneDone && millis() - tuneTimer >= winDur[tuneStep]) {
                tuneTimer = millis();
                if (winNotes[tuneStep] != 0)
                    tone(buzzerPin, winNotes[tuneStep], winDur[tuneStep] - 20);
                else
                    noTone(buzzerPin);           // final silence
                tuneStep++;
                if (winNotes[tuneStep] == 0) {   // reached final “rest”
                    tuneDone = true;             // mark tune finished
                    noTone(buzzerPin);
                }
            }

            // ── 5. Wait for restart push‑button ───────────────────────
            if (digitalRead(buttonPin) == LOW) {
                // Reset all runtime flags/variables exactly like your original logic
                totalSeconds   = 2 * 60;   // back to 2:00
                initialized    = false;
                winLatched     = false;
                tuneDone       = false;
                tuneStep       = 0;
                finalBuzzed    = false;
                wireGameDone   = false;
                memory_done    = false;
                memory_rgb_done= false;
                codeGameMode   = false;
                for (int i = 0; i < 4; i++) digitalWrite(ledPins[i], LOW);
                digitalWrite(segmentPins[7], LOW);  // turn DP off
                delay(300);   // debounce
            }

            return;   // keep main loop parked in WIN state until restart
          }
        }
      }
      
    }
  }
    // return;
  


  static unsigned long lastUpdate = 0;
  static unsigned long lastBeep = 0;
  static unsigned long lastFlash = 0;
  static bool ledPhase = false;


  // Refresh 7-segment display
  for (int d = 0; d < 4; d++) {
    showDigit(d, getDigitValue(d));
    delay(2);
    clearDisplay();
  }


  // Countdown logic
  if (millis() - lastUpdate >= 1000) {
    lastUpdate = millis();
    if (totalSeconds > 0) {
      totalSeconds--;
    } else if (!finalBuzzed) {
      initialized = false;


      // Panic LED strobe
      for (int i = 0; i < 100; i++) {
        digitalWrite(22, i % 2);
        digitalWrite(21, !(i % 2));
        digitalWrite(20, i % 2);
        digitalWrite(19, !(i % 2));
        delay(20);
      }


     


      // Final explosion buzzer
      for (int i = 0; i < 30; i++) {
        int freq = random(100, 300);
        int dur = random(10, 40);
        tone(buzzerPin, freq, dur);
        delay(dur);
      }
      for (int f = 600; f > 100; f -= 20) {
        tone(buzzerPin, f, 20);
        delay(20);
      }
      noTone(buzzerPin);


      for (int i = 0; i < 4; i++) digitalWrite(ledPins[i], LOW);
      finalBuzzed = true;
    }
  }


  // Beep & flash while counting
  if (totalSeconds > 0) {
    unsigned long currentMillis = millis();
    int interval;
    int pitch = 700;


    if (totalSeconds > 30) {
      interval = 1000;
    } else if (totalSeconds > 10) {
      interval = map(totalSeconds, 30, 10, 1000, 250);
    } else {
      interval = map(totalSeconds, 10, 0, 250, 80);
      pitch = 800 + (10 - totalSeconds) * 10;
    }


    if (currentMillis - lastBeep >= interval) {
      lastBeep = currentMillis;
      tone(buzzerPin, pitch, min(120, interval - 10));
    }


    if (currentMillis - lastFlash >= interval) {
      lastFlash = currentMillis;
      ledPhase = !ledPhase;
      digitalWrite(22, ledPhase ? HIGH : LOW);
      digitalWrite(21, ledPhase ? HIGH : LOW);
      digitalWrite(20, ledPhase ? LOW : HIGH);
      digitalWrite(19, ledPhase ? LOW : HIGH);
    }
  }


  // Reset via button after time runs out
  if (finalBuzzed && digitalRead(buttonPin) == LOW) {
    totalSeconds = 2 * 60;
    finalBuzzed = false;
    initialized = true;
    delay(300);
  }
}


// Get digit value from totalSeconds
int getDigitValue(int digitIndex) {
  int minutes = totalSeconds / 60;
  int seconds = totalSeconds % 60;
  if (digitIndex == 0) return minutes / 10;
  if (digitIndex == 1) return minutes % 10;
  if (digitIndex == 2) return seconds / 10;
  return seconds % 10;
}


// Show one digit on the display
void showDigit(int digitIndex, int num) {
  digitalWrite(digitPins[digitIndex], LOW);
  byte segs = digits[num];
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], (segs >> i) & 1);
  }
  digitalWrite(segmentPins[7], (digitIndex == 1) ? HIGH : LOW); // Light DP after MM
}


// Clear all digits and segments
void clearDisplay() {
  for (int i = 0; i < 4; i++) digitalWrite(digitPins[i], HIGH);
  for (int i = 0; i < 8; i++) digitalWrite(segmentPins[i], LOW);
}
