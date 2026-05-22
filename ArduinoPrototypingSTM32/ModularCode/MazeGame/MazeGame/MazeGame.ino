// Variable Initialization

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


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
