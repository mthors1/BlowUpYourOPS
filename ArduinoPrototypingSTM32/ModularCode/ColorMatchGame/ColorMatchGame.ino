#include <math.h>

const int speaker = PB8;
#include "pitches.h"

// pin 1 is controlled by player
const int redPin1 = PC13;
const int greenPin1 = PE5;
const int bluePin1 = PE4;

// pin 2 is randomized/controlled by program
const int redPin2 = PD7;
const int greenPin2 = PD6;
const int bluePin2 = PD3;

const int redPot = PA0;   
const int greenPot = PA1;  
const int bluePot = PA2;   

int colors[8][3] = {
  {0, 255, 255},   // Red
  {0, 140, 255},   // Orange
  {0, 0, 255},     // Yellow
  {255, 0, 255},   // Green
  {255, 0, 0},     // Cyan
  {255, 255, 0},   // Blue
  {128, 255, 0},   // Violet
  {0, 255, 0}      // Magenta
};

// if the game is too easy, we can add steps here to increase difficulty
int steps[] = {0, 64, 128, 140, 192, 255}; 
int numSteps = 6;

int redIndex, greenIndex, blueIndex, colorDistance, randomIndex;

void debugPrints() {
  Serial.print("Random LED 1: (");
  Serial.print(steps[redIndex]); Serial.print("r, ");
  Serial.print(steps[greenIndex]); Serial.print("g, ");
  Serial.print(steps[blueIndex]); Serial.print("b)       ");

  Serial.print("Controlled LED 2: (");
  Serial.print(colors[randomIndex][0]); Serial.print(", ");
  Serial.print(colors[randomIndex][1]); Serial.print(", ");
  Serial.print(colors[randomIndex][2]); Serial.print(")    ");
  
  Serial.println(colorDistance);
}

int distanceCalc(int playRed, int playGreen, int playBlue) {
  int redDist = colors[randomIndex][0] - playRed;
  int blueDist = colors[randomIndex][1] - playBlue;
  int greenDist = colors[randomIndex][2] - playGreen;

  return abs(redDist) + abs(blueDist) + abs(greenDist);
}

void setup() {
  Serial.begin(115200);
  pinMode(speaker, OUTPUT);
  // pins for rgb game
  pinMode(redPin1, OUTPUT);
  pinMode(greenPin1, OUTPUT);
  pinMode(bluePin1, OUTPUT);
  pinMode(redPin2, OUTPUT);
  pinMode(greenPin2, OUTPUT);
  pinMode(bluePin2, OUTPUT);
  pinMode(speaker, OUTPUT);

  randomSeed(analogRead(PA3));
  randomIndex = random(0, 8);
}

bool rgbGame() {
  Serial.println("game started");

  redIndex = map(analogRead(redPot), 0, 676, 0, numSteps); // max potentiometer reading for 3v3 on 5v scale is ~675
  greenIndex = map(analogRead(greenPot), 0, 676, 0, numSteps);
  blueIndex = map(analogRead(bluePot), 0, 676, 0, numSteps);

  analogWrite(redPin1, steps[redIndex]);
  analogWrite(greenPin1, steps[greenIndex]);
  analogWrite(bluePin1, steps[blueIndex]);

  // randomly selected color from fixed list
  analogWrite(redPin2,   colors[randomIndex][0]);
  analogWrite(greenPin2, colors[randomIndex][1]);
  analogWrite(bluePin2,  colors[randomIndex][2]);

  debugPrints();
  colorDistance = distanceCalc(steps[redIndex], steps[greenIndex], steps[blueIndex]);

  if (colorDistance <= 10) {
    song3(); 
    Serial.println("MATCHED");
    delay(2000);               
    return true;
  } else {
    noTone(speaker);
    return false;
  }
  delay(10);
}

void loop() {
  rgbGame();
}
