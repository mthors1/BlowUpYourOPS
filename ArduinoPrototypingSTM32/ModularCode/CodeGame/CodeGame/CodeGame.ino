
const int speaker = PC6;

const int segments[7] = {PD1, PD2, PD3, PD6, PD7, PB3, PB4}; // segments a-g
const int digits[3] = {PC8, PA8, PC11};
const int button[3] = {PC9, PA15, PD0};

#include "numbers.h"
#include "pitches.h"

int count[3] = {0, 0, 0};

HardwareSerial Serial2(PA3, PA2);

// button debouncing code: source = https://forum.arduino.cc/t/buttons-and-other-electro-mechanical-inputs-introduction/688576
void buttons(int index) {
  #define buttonPressed LOW

  uint32_t currentMillis = millis();
  const uint32_t BOUNCETIMEOUT = 20;

  static uint32_t lastMillis[3] = {0, 0, 0};
  static bool lastButtonState[3] = {HIGH, HIGH, HIGH};

  bool currentButtonState = digitalRead(button[index]);

  if (lastButtonState[index] != currentButtonState) {
    if (currentMillis - lastMillis[index] >= BOUNCETIMEOUT) {
      lastButtonState[index] = currentButtonState;

      if (currentButtonState == buttonPressed) {
        Serial2.print("button pressed! number = ");
        Serial2.println(count[index]);

        count[index] = count[index] + 1;

        if (count[index] == 10) {
          count[index] = 0;
        }
      }
    }
  }
  else {
    lastMillis[index] = currentMillis;
  }
}

void setup() {
  Serial2.begin(115200); 

  for (int i = 0; i < 3; i++) {
    pinMode(button[i], INPUT_PULLDOWN);
    pinMode(digits[i], OUTPUT);
    pinMode(segments[i], OUTPUT);
  }

  for (int i = 3; i < 7; i++) {
    pinMode(segments[i], OUTPUT);
  }

  pinMode(speaker, OUTPUT);
} 

void loop() {  
  for (int i = 0; i < 3; i++) {
    displayOneDigit(i, count[i]);  
    if (count[0] == 9 && count[1] == 6 && count[2] == 7) {
      // song3(); MAKE THE OTHER MICROCONTROLLER PLAY THE SONG, so the numbers can stay
    }
    else {
      buttons(i); 
    }

  }
}