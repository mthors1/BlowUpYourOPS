#include "pitches.h"
HardwareSerial Serial2(PA3, PA2);

// Wire game pins
const int wirePins[4] = {PE4, PE5, PB4, PB5}; // array for digital pins connected to pull-wires

// Wire game variables
int correctWire; // initialize variable for randomly generated correct wire
bool wirePulled[4]; // array to keep track of which wires have been pulled

unsigned long printTimer;
unsigned long currentTime;


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
  correctWire = random(0, 4);

}

void printWireStatus(){
  Serial2.println("Wire signals:");
    for (int i = 0; i < 4; i++){
      Serial2.print("Wire ");
      Serial2.print(i);
      Serial2.print(": ");
      Serial2.println(digitalRead(wirePins[i]));
    }

    Serial2.println();

    Serial2.println("Wire pulled statuses:");
    for (int i = 0; i < 4; i++){
      Serial2.print("Wire ");
      Serial2.print(i);
      Serial2.print(": ");
      Serial2.println(wirePulled[i]);
    }

    Serial2.println();
}

bool wireGame(){
  // Debugging
  Serial2.print("Game started. Pull correct wire to win. Correct wire: ");
  Serial2.println(correctWire + 1);

  bool snippingWires = true;
  bool replacingWires = false;

  printTimer = millis();
  while (snippingWires){
    for (int i = 0; i < 4; i++){
      if (digitalRead(wirePins[i]) == HIGH && wirePulled[i] == false){
        wirePulled[i] = true;

        if (i == correctWire){
          Serial2.println("Correct wire Pulled");
          snippingWires = false;
          song3();
        }
        else{
          Serial2.println("Wrong wire pulled");
          wrongWireSound();
          //Send signal to subtract time
        }
      }
    }
    // Debugging
    currentTime = millis();
    if (currentTime - printTimer > 1500){
      printTimer = millis();
      printWireStatus();
    }
  }

  int wiresReplaced = 0;
  printTimer = millis();
  while (replacingWires){
    for (int i = 0; i < 4; i++){
      if (digitalRead(wirePins[i]) == LOW && wirePulled[i] == true){
        wirePulled[i] = false;
        wiresReplaced++;
      }
    }
    if (wiresReplaced == 4){
      replacingWires = false;
    }

    //debugging
    currentTime = millis();
    if (currentTime - printTimer > 1500){
      printTimer = millis();
      printWireStatus();
    }
  }
  Serial2.println("Wire game completed");
  delay(5000);


  return true;
}


void loop() {
  wireGame();
}
