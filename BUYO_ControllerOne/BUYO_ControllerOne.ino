// Test comment

/* Define Variables */

// Maze game pins
const int SW_Pin = 16; // digital pin for switch output
const int X_pin = 27; // analog pin for X output on joystick
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



void setup() {
  

}

void loop() {
  // put your main code here, to run repeatedly:

}
