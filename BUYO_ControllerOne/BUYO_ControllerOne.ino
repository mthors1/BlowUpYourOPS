/* Define Variables */

// Maze game pins
const int SW_Pin = 16; // digital pin for switch output
const int X_Pin = 27; // analog pin for X output on joystick
const int Y_Pin = 26; // analog pin for Y output on joystick
const int speaker = 28; // digital pin for speaker output
const int mazeWinLED = 18; // digital pin for win LED

// Maze game variables
int xpos; // store joystick X output
int ypos; // store joystick Y output
int playerX = 0; // store player X pos
int playerY = 0; // store player Y pos
bool joystickLatched = false; // boolean to prevent multiple shifts from one joystick movement

const int maze_size = 15; // side length for maze grid
int maze[maze_size][maze_size]; // creates 2D array for maze grid
int goalX = maze_size - 1; // sets goal to bottom right
int goalY = maze_size - 1; // ^
static unsigned long lastPressTime; // ensures new input from joystick is after a small time to avoid misinputs

// Wire game pins
const int wirePins[4] = {2, 3, 4, 5}; // array for digital pins connected to pull-wires
int wireWinLED = 12; // digital pin for win LED

// Wire game variables
int correctWire; // initialize variable for randomly generated correct wire
bool wirePulled[4]; // array to keep track of which wires have been pulled

// Speaker & sound variables
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

int note_dur = 30; // duration of music notes 


/* Speaker and music functions*/
void play(int note, int dur) {
  tone(speaker, note);
  delay(dur * note_dur);
  noTone(speaker);
  delay(dur * note_dur / 3);
}


void successSound() {
  play(NOTE_E4, 1);
  play(NOTE_G4, 1);
  play(NOTE_C5, 2);
}


void failureSound() {
  play(NOTE_C3, 3);
  play(NOTE_E3, 2);
  play(NOTE_A3, 1);
}


void song3() {
  play(NOTE_DS4, 2);
  play(NOTE_E4, 2);
  play(NOTE_F4, 5);
  play(NOTE_F4, 2);
  play(NOTE_FS4, 2);
  play(NOTE_G4, 5);
  play(NOTE_G4, 2);
  play(NOTE_GS4, 2);
  play(NOTE_A4, 5);
  play(NOTE_C5, 9);
}

/* Maze game functions */

int readJoystick() {
  // Map potentiometer readings to 1-100 scale
  xpos = map(analogRead(X_Pin), 0, 1023, 1, 100);
  ypos = map(analogRead(Y_Pin), 0, 1023, 1, 100);

  // Based on 1-100 scale, read directional inputs
  if (xpos < 40) { return -1; } // go left
  if (xpos > 60) { return 1; }  // go right
  if (ypos < 40) { return -2; } // go up
  if (ypos > 60) { return 2; }  // go down
  return 0; // no movement
}

// Maze generation process developed with the help of AI tools

bool visited[maze_size][maze_size];

bool isSolvableDFS(int x, int y) {
  if (x < 0 || x > maze_size || y < 0 || y >= maze_size) return false;
  if (maze[x][y] == 1 || visited[x][y]) return false;
  if (x == goalX && y == goalY) return true;

  visited[x][y] == true;

  return isSolvableDFS(x + 1, y) ||
         isSolvableDFS(x - 1, y) ||
         isSolvableDFS(x, y + 1) ||
         isSolvableDFS(x, y - 1);
}


bool mazeIsSolvable() { 
  memset(visited, 0, sizeof(visited));
  return isSolvableDFS(playerX, playerY);
}


void generateMaze() {
  do {
    randomSeed(micros());
    for (int i = 0; i < maze_size; i++) {
      for (int j = 0; j < maze_size; j++) {
        maze[i][j] = (random(0, 100) < 30) ? 1 : 0;
      }
    }
    maze[playerX][playerY] = 0;
    maze[goalX][goalY] = 0;
  } while (!mazeIsSolvable());
}


void resetGame() {
  Serial.println("Resetting game...");

  // reset player position
  playerX = 0;
  playerY = 0;

  // clear visited array
  for (int i = 0; i < maze_size; i++){
    for (int j = 0; j < maze_size; j++){
      if (random(0, 100) < 30){
        maze[i][j] = 1;
      }
      else{
        maze[i][j] = 0;
      }
    }
  }
  
  // reset player and goal positions
  maze[playerX][playerY] = 0;
  maze[goalX][goalY] = 0;
  while (!mazeIsSolvable());
}


bool checkMazeWin() {
  if (playerX == goalX && playerY == goalY) {
    return true;
  }
  return false;
}


bool mazeWinner() {
  if (checkMazeWin()) {
    delay(500);
    song3();
    return true;
  }
  return false;
}


void movePlayer(int direction){
  int newX = playerX;
  int newY = playerY;

  if (direction == -2) newX--; // Up
  if (direction == 2)  newX++; // Down 
  if (direction == -1) newY--; // Left 
  if (direction == 1)  newY++; // Right

  // Debugging movement
  Serial.print("Attempting to move to: ");
  Serial.print(newX);
  Serial.print(", ");
  Serial.println(newY);

  // Check if wall is hit
  if (newX >= 0 && newX < maze_size && newY >= 0 && newY < maze_size) {
    Serial.print("Maze value at position: ");
    Serial.println(maze[newX][newY]);
    if (maze[newX][newY] == 0) { // Valid move
      playerX = newX;
      playerY = newY;
      Serial.print("Moved to: ");
      Serial.print(playerX);
      Serial.print(", ");
      Serial.println(playerY);
      successSound(); // Successful move sound
    } else {
      failureSound(); // Wall hit sound
      Serial.println("Hit a wall!");
    }
  } else {
    failureSound(); // Hit wall
    Serial.println("Out of bounds!");
  }
}


void printMaze() {
  for (int i = 0; i < maze_size; i++) {
    for (int j = 0; j < maze_size; j++) {
      if (i == playerX && j == playerY) {
        Serial.print("P ");
      } else {
        Serial.print(maze[i][j]);
        Serial.print(" ");
      }
    }
    Serial.println();
  }
}


/* Whole program set up */
void setup() {
  pinMode(SW_Pin, INPUT_PULLUP);
  pinMode(speaker, OUTPUT);
  pinMode(mazeWinLED, OUTPUT);
  pinMode(wireWinLED, OUTPUT);
  Serial.begin(115200);
  // Initialize Serial 1 connection (to central)
  Serial1.setTX(0);
  Serial1.setRX(1);
  Serial1.begin(115200);

  while (!Serial) {}
  randomSeed(micros());

  for (int i = 0; i < 4; i++) {
    pinMode(wirePins[i], INPUT_PULLUP);
    wirePulled[i] = false;
    Serial.print("Wire ");
    Serial.print(i);
    Serial.print(" state: ");
    //Serial.println(digitalRead(wirePins[i]));
  }
  correctWire = random(0, 4);
}


/* Whole Maze Game Sequence */
bool mazeGame(){
  generateMaze();
  Serial.println("Generated maze: ");
  printMaze();
  bool playingMaze = true;
  while (playingMaze){
    lastPressTime = 0;
    // If game is won, if joystick is pressed, reset game (debugging)
    if (digitalRead(SW_Pin) == LOW && checkMazeWin() && millis() - lastPressTime > 550) {
      resetGame();
      lastPressTime = millis();
    }
    int moveDirection = readJoystick(); // Read joystick input


    if (moveDirection != 0 && !joystickLatched) {
      movePlayer(moveDirection);  // Update player position
      // Check if player reached the goal
      if (mazeWinner()){
        playingMaze = false;
        digitalWrite(mazeWinLED, HIGH);
      }  
      Serial.println("Current maze state: ");
      printMaze();
      joystickLatched = true; // Prevent repeat movement
    }


    // Once joystick returns to neutral, unlatch
    if (moveDirection == 0) {
      joystickLatched = false;
    }

    delay(50); // Small delay to prevent excessive polling
  }

  return playingMaze;
}


/* Full Wire Game Sequence */
bool wireGame(){
  // Debugging
  Serial.print("Game started. Pull correct wire to win. Correct wire: ");
  Serial.println(correctWire + 1);

  bool stillPlaying = true;
  while (stillPlaying){
    for (int i = 0; i < 4; i++){
      if (digitalRead(wirePins[i]) == HIGH && wirePulled[i] == false){
        wirePulled[i] = true;

        if (i == correctWire){
          Serial.println("Correct wire Pulled");
          stillPlaying = false;
          song3();
          digitalWrite(wireWinLED, HIGH);
          
        }
        else{
          Serial.println("Incorrect wire Pulled");
          // UPDATE BELOW TO SUBTRACT TIME FOR INCORRECT WIRE
          Serial2.write((uint8_t)3);
          Serial2.write((uint8_t)0);
        }
      }
    }
    // Prevent excess polling
    delay(50);
  }
  return stillPlaying;
}


void loop() {while (!Serial2.available()){}
    uint8_t received = Serial2.read();
    Serial2.write((uint8_t)0);
    
    if (received == 1){
      Serial.println("Starting wire game");
      Serial2.write((uint8_t)0); // Clear Serial2
      while(wireGame()){}
      Serial2.write((uint8_t)4); // Send back to main to move on to other games on other controller
    }

    if (received == 6){
      Serial.println("MAZE GAME TIME");
      Serial2.write((uint8_t)0); // Clear Serial2
      while(mazeGame()){}
      Serial2.write((uint8_t)7); // Start codeGameMode
    }

}
