#include "pitches.h"
HardwareSerial Serial2(PA3, PA2);
// Variable Initialization

const int SW_pin = PD3; // digital pin connected to switch output
const int X_pin = PB0;  // analog pin connected to X output
const int Y_pin = PB1;  // analog pin connected to Y output
// const int speaker = PC8;

int xpos;
int ypos;
int playerX = 0;
int playerY = 0;

bool joystickLatched = false;

const int MAZE_SIZE = 15;
int maze[MAZE_SIZE][MAZE_SIZE];
int goalX = MAZE_SIZE - 1;
int goalY = MAZE_SIZE - 1;
static unsigned long lastPressTime;
const int mazeWinLED = 18;

bool visited[MAZE_SIZE][MAZE_SIZE]; // Used for checking maze solvability


// Helper Functions
int readJoystick() {
  xpos = map(analogRead(X_pin), 0, 1023, 1, 100);
  ypos = map(analogRead(Y_pin), 0, 1023, 1, 100);

  if (xpos < 40) { return -1; } // go left
  if (xpos > 60) { return 1; }  // go right
  if (ypos < 40) { return -2; } // go up
  if (ypos > 60) { return 2; }  // go down
  return 0; // no movement
}


bool isSolvableDFS(int x, int y) {
  if (x < 0 || x >= MAZE_SIZE || y < 0 || y >= MAZE_SIZE) return false; // Invalid input
  if (maze[x][y] == 1 || visited[x][y]) return false; // Looped around
  if (x == goalX && y == goalY) return true; // Maze is solvable, finish reached

  visited[x][y] = true;

  // DFS Recursion
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
    Serial2.println("Trying to generate maze");
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


void resetMaze() { // Debugging
  resetSound();

  Serial2.println("Resetting maze...");
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
  Serial2.println("New maze generated:");

  printMaze();
}


void resetPlayerPos() {
  resetSound();

  Serial2.println("Resetting player position...");
  // Reset player position
  playerX = 0;
  playerY = 0;
  
  printMaze();
}


bool checkWin() {
  if (playerX == goalX && playerY == goalY) {
    return true;
  }
  return false;
}


bool winner() {
  if (checkWin()) {
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
  Serial2.print("Attempting to move to: ");
  Serial2.print(newX);
  Serial2.print(", ");
  Serial2.println(newY);

  // Treat out-of-bounds like walls
  if (newX >= 0 && newX < MAZE_SIZE && newY >= 0 && newY < MAZE_SIZE) {
    Serial2.print("Maze value at position: ");
    Serial2.println(maze[newX][newY]);
    if (maze[newX][newY] == 0) { // Valid move
      playerX = newX;
      playerY = newY;
      Serial2.print("Moved to: ");
      Serial2.print(playerX);
      Serial2.print(", ");
      Serial2.println(playerY);
      successSound();
    } 
    else {
      failureSound(); // Wall hit sound
      Serial2.println("Hit a wall!");
    }
  } 
  else {
    failureSound(); // Out-of-bounds treated as wall
    Serial2.println("Out of bounds!");
  }
}


void printMaze() {
  for (int i = 0; i < MAZE_SIZE; i++) {
    for (int j = 0; j < MAZE_SIZE; j++) {
      if (i == playerX && j == playerY) {
        Serial2.print("P ");
      } else {
        Serial2.print(maze[i][j]);
        Serial2.print(" ");
      }
    }
    Serial2.println();
  }
}

bool mazeGame(){
  resetMaze();
  
  Serial2.println("Generated maze: ");
  printMaze();

  bool playingMaze = true;
  while (playingMaze){
    lastPressTime = 0;
    // If game is won, if joystick is pressed, reset game (debugging)
    if (digitalRead(SW_pin) == LOW && !checkWin() && (millis() - lastPressTime > 550)) {
      resetPlayerPos();
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
      Serial2.println("Current maze state: ");
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



void setup() {
  pinMode(SW_pin, INPUT_PULLUP);
  pinMode(speaker, OUTPUT);
  pinMode(mazeWinLED, OUTPUT);
  

  // Initialize Serial 2 connection (debugging)
  Serial2.begin(115200);
  delay(50);

  randomSeed(micros());
}

bool gameDone = false;
void loop() {
  Serial2.println("Looping");
  if (!gameDone){
    mazeGame();
  }
  
  gameDone = true;

  if (gameDone){
    if (digitalRead(SW_pin) == LOW && millis() - lastPressTime > 550) {
      gameDone = false;
      lastPressTime = millis();
    }
  }
  

}
