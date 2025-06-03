// DEFINE VARIABLES

const int SW_pin = 16; // digital pin connected to switch output
const int X_pin = 27;  // analog pin connected to X output
const int Y_pin = 26;  // analog pin connected to Y output
#define speaker 28


int xpos;
int ypos;
int playerX = 0;
int playerY = 0;
int NOTE_DUR = 30; // NOT TRANSFERRED
bool joystickLatched = false;


const int MAZE_SIZE = 15;
int maze[MAZE_SIZE][MAZE_SIZE];
int goalX = MAZE_SIZE - 1;
int goalY = MAZE_SIZE - 1;
static unsigned long lastPressTime;
const int mazeWinLED = 18;

const int wirePins[4] = {2, 3, 4, 5};
int correctWire;
bool wirePulled[4];
int wireLED = 12;

// #include "pitches.h"
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

// CODE FOR SPEAKER FUNCTIONALITY

void play(int note, int dur) {
  tone(speaker, note);
  delay(dur * NOTE_DUR);
  noTone(speaker);
  delay(dur * NOTE_DUR / 3);
}


void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 100L; i += tone * 2) {
    digitalWrite(speaker, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speaker, LOW);
    delayMicroseconds(tone);
  }
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

// CREATE FUNCTIONS FOR MAZE GAME

int readJoystick() {
  xpos = map(analogRead(X_pin), 0, 1023, 1, 100);
  ypos = map(analogRead(Y_pin), 0, 1023, 1, 100);


  if (xpos < 40) { return -1; } // go left
  if (xpos > 60) { return 1; }  // go right
  if (ypos < 40) { return -2; } // go up
  if (ypos > 60) { return 2; }  // go down
  return 0; // no movement
}


// ---------- Maze Pathfinding Check ----------
bool visited[MAZE_SIZE][MAZE_SIZE];


bool isSolvableDFS(int x, int y) {
  if (x < 0 || x >= MAZE_SIZE || y < 0 || y >= MAZE_SIZE) return false;
  if (maze[x][y] == 1 || visited[x][y]) return false;
  if (x == goalX && y == goalY) return true;


  visited[x][y] = true;


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
    for (int i = 0; i < MAZE_SIZE; i++) {
      for (int j = 0; j < MAZE_SIZE; j++) {
        maze[i][j] = (random(0, 100) < 30) ? 1 : 0;
      }
    }
    maze[playerX][playerY] = 0;
    maze[goalX][goalY] = 0;
  } while (!mazeIsSolvable());
}
void resetGame() {
  Serial.println("Resetting game...");
  // Reset player position
  playerX = 0;
  playerY = 0;
  // Clear visited array
  for (int i = 0; i < MAZE_SIZE; i++) {
    for (int j = 0; j < MAZE_SIZE; j++) {
      visited[i][j] = false;
    }
  }
  // Generate a new maze ensuring it's solvable
  generateMaze();
  // Print new maze for debugging
  Serial.println("New maze generated:");
  printMaze();
}
bool CheckWin() {
  if (playerX == goalX && playerY == goalY) {
    return true;
  }
  return false;
}


bool winner() {
  if (CheckWin()) {
    delay(500);
    song3();
    return true;
  }
  return false;
}


void movePlayer(int direction) {
  int newX = playerX, newY = playerY;


  if (direction == -2) newX--; // Up (decrease x)
  if (direction == 2)  newX++; // Down (increase x)
  if (direction == -1) newY--; // Left (decrease y)
  if (direction == 1)  newY++; // Right (increase y)


  // Debugging movement
  Serial.print("Attempting to move to: ");
  Serial.print(newX);
  Serial.print(", ");
  Serial.println(newY);


  // Treat out-of-bounds like walls
  if (newX >= 0 && newX < MAZE_SIZE && newY >= 0 && newY < MAZE_SIZE) {
    Serial.print("Maze value at position: ");
    Serial.println(maze[newX][newY]);
    if (maze[newX][newY] == 0) { // Valid move
      playerX = newX;
      playerY = newY;
      Serial.print("Moved to: ");
      Serial.print(playerX);
      Serial.print(", ");
      Serial.println(playerY);
      successSound();
    } else {
      failureSound(); // Wall hit sound
      Serial.println("Hit a wall!");
    }
  } else {
    failureSound(); // Out-of-bounds treated as wall
    Serial.println("Out of bounds!");
  }
}


void printMaze() {
  for (int i = 0; i < MAZE_SIZE; i++) {
    for (int j = 0; j < MAZE_SIZE; j++) {
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


// WHOLE PROGRAM SET UP
void setup() {
  pinMode(SW_pin, INPUT_PULLUP);
  pinMode(speaker, OUTPUT);
  pinMode(mazeWinLED, OUTPUT);
  pinMode(wireLED, OUTPUT);
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

// MAZE FUNCTIONS
bool mazeGame(){
  generateMaze();
  Serial.println("Generated maze: ");
  printMaze();
  bool playingMaze = true;
  while (playingMaze){
    lastPressTime = 0;
    // If game is won, if joystick is pressed, reset game (debugging)
    if (digitalRead(SW_pin) == LOW && CheckWin() && millis() - lastPressTime > 550) {
      resetGame();
      lastPressTime = millis();
    }
    int moveDirection = readJoystick(); // Read joystick input


    if (moveDirection != 0 && !joystickLatched) {
      movePlayer(moveDirection);  // Update player position
      // Check if player reached the goal
      if (winner()){
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

bool wireGame(){
  Serial.print("🎮 Game started! Pull the correct wire to win (wire ");
  Serial.print(correctWire + 1);
  Serial.println(")");
  bool stillPlaying = true;
  while (stillPlaying){
    for (int i = 0; i < 4; i++){
      if (digitalRead(wirePins[i]) == HIGH && wirePulled[i] == false){
        wirePulled[i] = true;

        if (i == correctWire){
          Serial.println("Correct wire Pulled");
          stillPlaying = false;
          song3();
          digitalWrite(wireLED, HIGH);
          
        }
        else{
          Serial.println("Incorrect wire Pulled");
          Serial2.write((uint8_t)3);
          Serial2.write((uint8_t)0);
        }
      }

      // int state = digitalRead(wirePins[i]);
      // Serial.print("Pin ");
      // Serial.print(wirePins[i]);
      // Serial.print(": ");
      // Serial.println(state == LOW ? "Plugged in" : "Unplugged");
    }
    delay(50);
  }
  Serial2.write((uint8_t)4);
  return stillPlaying;
}

  void loop() {
    while (!Serial2.available()){}
    uint8_t received = Serial2.read();

    Serial2.write((uint8_t)0);
    if (received == 1){
      while(wireGame()){}
    }
    Serial2.write((uint8_t)0); // Send back to main to move on to other games on other controller

    if (received == 6){
      Serial.println("MAZE GAME TIME");
      Serial2.write((uint8_t)0); // Clear Serial2
      while(mazeGame()){}
      Serial2.write((uint8)7); // Start codeGameMode
    }
    
    // Serial2.write((uint8_t)9); // Sends a 9 to tell main we are in "code game state"
    // Serial2.write((uint8_t)0) // Clear Serial2

    // Implement passcode game below
    


   
    
}
