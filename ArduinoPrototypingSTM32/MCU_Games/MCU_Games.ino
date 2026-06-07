#include "WireGame.h"
#include "MemoryGame.h"
#include "RgbGame.h"
#include "MazeGame.h"
#include "PasscodeGame.h"
#include "pitches.h"

// --- Global Initialization ---
bool started = true; // Should be initialized as false, only true for testing
bool memoryGame = false;
bool colorGame = false;


void setup() {
  Serial2.begin(115200);
  Serial2.println("Trying to start");

  delay(2000);
  
  Serial2.println("Serial started!");

  randomSeed(micros());

  wireGameInit();
  memoryGameInit();
  rgbGameInit();
  mazeGameInit();
  passcodeGameInit();
}

void loop() {
  while (!started){
    //wait on welcome state
  }


  // -- Game loop functionality
  // if (!Game1){
  //   buttonGame();
  // }
  // else if (!Game2){

  // }
  // ...


 

}

