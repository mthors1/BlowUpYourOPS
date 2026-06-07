
const int button3 = PC15;
const int button2 = PB7;
const int button1 = PD1;

const int speaker = PB13;

const int display3[7] = {PB8, PE2, PE4, PE5, PE6, PC14, PC13}; // segments a-g
const int display2[7] = {PD2, PD3, PD6, PB3, PD7, PB4, PB5};
const int display1[7] = {PC6, PC9, PC8, PA8, PA15, PC10, PD0};

#include "numbers.h"
#include "pitches.h"

int count3 = 0;
int count2 = 0;
int count1 = 0;

HardwareSerial Serial2(PA3, PA2);

// button debouncing code: source = https://forum.arduino.cc/t/buttons-and-other-electro-mechanical-inputs-introduction/688576
void buttons(int button, int &count, int index) {
  #define buttonPressed LOW

  uint32_t currentMillis = millis();
  const uint32_t BOUNCETIMEOUT = 20;

  static uint32_t lastMillis[3] = {0, 0, 0};
  static bool lastButtonState[3] = {HIGH, HIGH, HIGH};

  bool currentButtonState = digitalRead(button);

  if (lastButtonState[index] != currentButtonState) {
    if (currentMillis - lastMillis[index] >= BOUNCETIMEOUT) {
      lastButtonState[index] = currentButtonState;

      if (currentButtonState == buttonPressed) {
        Serial2.print("button pressed! number = ");
        Serial2.println(count);

        count++;

        if (count == 10) {
          count = 0;
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
  for (int i = 0; i < 7; i++) {
    pinMode(display1[i], OUTPUT);
    pinMode(display2[i], OUTPUT);
    pinMode(display3[i], OUTPUT);
  }

  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);

  pinMode(speaker, OUTPUT);
} 

void loop() {  
  displayDigit(display1, count1);   
  displayDigit(display2, count2);   
  displayDigit(display3, count3);   

  buttons(button1, count1, 0);
  buttons(button2, count2, 1);
  buttons(button3, count3, 2);

  if (count1 == 9 && count2 == 6 && count3 == 7) {
    displayDigit(display1, count1);   
    displayDigit(display2, count2);   
    displayDigit(display3, count3); 

    song3();
  }
}