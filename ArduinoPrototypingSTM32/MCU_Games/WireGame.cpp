#include <Arduino.h>

#include "pitches.h"
HardwareSerial Serial2(PA3, PA2);

// Wire game pins
const int wirePins[4] = {PE4, PE5, PB4, PB5}; // array for digital pins connected to pull-wires

// Wire game variables
int correctWire; // initialize variable for randomly generated correct wire
bool wirePulled[4]; // array to keep track of which wires have been pulled

//Helper variables
unsigned long printTimer;
unsigned long currentTime;
int wiresReplaced = 0;

//For debugging
void printWireStatus(){
  Serial2.print("Correct wire:");
  Serial2.println(correctWire);

  Serial2.println("Wire signals:");
    for (int i = 0; i < 4; i++){
      Serial2.print("Wire ");
      Serial2.print(i);
      Serial2.print(": ");
      Serial2.println(digitalRead(wirePins[i]));
    }

    Serial2.println("Wire pulled statuses:");
    for (int i = 0; i < 4; i++){
      Serial2.print("Wire ");
      Serial2.print(i);
      Serial2.print(": ");
      Serial2.println(wirePulled[i]);
    }
}

bool wireGame(){
  correctWire = random(0, 4);

  // Debugging
  Serial2.print("Game started. Pull correct wire to win. Correct wire: ");
  Serial2.println(correctWire + 1);

  bool snippingWires = true;
  bool replacingWires = true;

  printTimer = millis();
  while (snippingWires){
    for (int i = 0; i < 4; i++){
      //If wire pulled but not updated yet
      if (digitalRead(wirePins[i]) == HIGH && wirePulled[i] == false){
        wirePulled[i] = true;

        if (i == correctWire){
          Serial2.println("Correct wire Pulled");
          snippingWires = false;
          correctWireSound();
        }
        else{
          Serial2.println("Wrong wire pulled");
          wrongWireSound();
          //Send signal to subtract time
        }
      }
    }
    // Print wire status every 2 seconds for debugging
    currentTime = millis();
    if (currentTime - printTimer > 2000){
      printTimer = millis();
      printWireStatus();
    }
  }

  Serial2.println("Pulling phase done");

  Serial2.println("Replace time");

  printTimer = millis();
  while (replacingWires){
    wiresReplaced = 0;
    for (int i = 0; i < 4; i++){
      //Print when unplugged wire gets replaced
      if (digitalRead(wirePins[i]) == LOW && wirePulled[i] == true){
        Serial2.print("Wire ");
        Serial2.print(i);
        Serial2.print(" replaced");
      }
      if (digitalRead(wirePins[i]) == LOW){
        wirePulled[i] = false;
        wiresReplaced++;
      }
    }
    if (wiresReplaced == 4){
      replacingWires = false;
    }
    //debugging
    currentTime = millis();
    if (currentTime - printTimer > 2000){
      printTimer = millis();
      printWireStatus();
    }
  }

  Serial2.println("Wire game completed");
  song3();
  return true;
}


void setup() {
  Serial2.begin(115200);
  printWireStatus();

  for (int i = 0; i < 4; i++) {
    pinMode(wirePins[i], INPUT_PULLUP);
    wirePulled[i] = false;
    Serial2.print("Wire ");
    Serial2.print(i);
    Serial2.print(" state: ");
    //Serial.println(digitalRead(wirePins[i]));
  }
  randomSeed(micros());

}

void wireGameInit() {
  Serial2.begin(115200);
  printWireStatus();

  for (int i = 0; i < 4; i++) {
    pinMode(wirePins[i], INPUT_PULLUP);
    wirePulled[i] = false;
    Serial2.print("Wire ");
    Serial2.print(i);
    Serial2.print(" state: ");
    //Serial.println(digitalRead(wirePins[i]));
  }
  randomSeed(micros());

}

void loop() {
  wireGame();
}
