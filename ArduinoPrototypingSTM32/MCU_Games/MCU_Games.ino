#include "WireGame.h"
#include "MemoryGame.h"
#include "RgbGame.h"
#include "MazeGame.h"
#include "PasscodeGame.h"
#include "Pitches.h"

// --- Global Initialization ---
bool started = true; // Should be initialized as false, only true for testing

HardwareSerial Serial2(PA3, PA2);



void setup() {
  Serial2.begin(115200);
  Serial2.println("Trying to start");

  delay(2000);
  
  Serial2.println("Serial started!");

  

  // wireGameInit();
  
  
  // mazeGameInit();
  // passcodeGameInit();
}

void loop() {
  while (!started){
    //wait on welcome state
  }
  
  //wireGame();
  //memoryGame();
  rgbGameInit();
  rgbGame();
  
  memoryGameInit();
  memoryGame();

  // passcodeGame();

  while (true) {
    Serial2.println("Games Done!");
  }
  

  //-- Game loop functionality for when we need to know when to send signal to other MCU
  // if (!Game1){
  //   buttonGame();
  // }
  // else if (!Game2){

  // }
  // ...
}

