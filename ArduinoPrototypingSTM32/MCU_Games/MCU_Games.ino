// --- Global Initialization ---
bool started = true; // Should be initialized as false, only true for testing
bool memoryGame = false;
bool colorGame = false;


void setup() {
  SerialUSB.begin(115200);
  SerialUSB.println("Trying to start");

  delay(2000);
  
  SerialUSB.println("Serial started!");
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

