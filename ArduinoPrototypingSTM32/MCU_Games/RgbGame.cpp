#include <math.h>
#include <Arduino.h>

#include "Pitches.h"

// pin 1 is controlled by player
const int redPin1 = PB5; // Was A3 // needs to say TIMx_CHy for PWM output
const int greenPin1 = PB0;
const int bluePin1 = PB1;

// pin 2 is randomized/controlled by program
const int redPin2 = PA0;
const int greenPin2 = PA1;
const int bluePin2 = PB7; // was A2

const int redPot = PC1;   // needs to say ADC123_INx for analog input
const int greenPot = PC2;  
const int bluePot = PC4;  

const int speaker = PB8;

int maxRead = 1023;

int colors[8][3] = {
  {255, 0, 0},   // Red
  {255, 116, 0},   // Orange
  {255, 255, 0},     // Yellow
  {0, 255, 0},   // Green
  {0, 255, 255},     // Cyan
  {0, 0, 255},   // Blue
  {128, 0, 255},   // Violet
  {255, 0, 255}      // Magenta
};

// if the game is too easy, we can add steps here to increase difficulty
int steps[] = {0, 64, 116, 128, 192, 255}; 
int numSteps = 6;

int redIndex, greenIndex, blueIndex, colorDistance, randomIndex;

void debugPrints() {
  Serial2.print("Random LED 1: (");
  Serial2.print(steps[redIndex]); Serial2.print("r, ");
  Serial2.print(steps[greenIndex]); Serial2.print("g, ");
  Serial2.print(steps[blueIndex]); Serial2.print("b)       ");

  Serial2.print("Controlled LED 2: (");
  Serial2.print(colors[randomIndex][0]); Serial2.print(", ");
  Serial2.print(colors[randomIndex][1]); Serial2.print(", ");
  Serial2.print(colors[randomIndex][2]); Serial2.print(")    color distance: ");
  
  Serial2.println(colorDistance);
}

int distanceCalc(int playRed, int playGreen, int playBlue) {
  int redDist = abs(colors[randomIndex][0]) - abs(playRed);
  int greenDist = abs(colors[randomIndex][1]) - abs(playGreen);
  int blueDist = abs(colors[randomIndex][2]) - abs(playBlue);

  return abs(redDist) + abs(blueDist) + abs(greenDist);
}

void RgbGamesetup() {
  Serial2.begin(115200);
  Serial2.println("ready");
  pinMode(speaker, OUTPUT);
  // pins for rgb game
  pinMode(redPin1, OUTPUT);
  pinMode(greenPin1, OUTPUT);
  pinMode(bluePin1, OUTPUT);
  pinMode(redPin2, OUTPUT);
  pinMode(greenPin2, OUTPUT);
  pinMode(bluePin2, OUTPUT);

  randomSeed(analogRead(PA3));
  randomIndex = random(0, 8);
}

void rgbGameInit() {
  pinMode(speaker, OUTPUT);
  // pins for rgb game
  pinMode(redPin1, OUTPUT);
  pinMode(greenPin1, OUTPUT);
  pinMode(bluePin1, OUTPUT);
  pinMode(redPin2, OUTPUT);
  pinMode(greenPin2, OUTPUT);
  pinMode(bluePin2, OUTPUT);

  
}

bool rgbGame() {
  randomSeed(micros());
  randomIndex = random(0, 8);
  bool playing = true;
  while (playing){
    // Serial2.println("game started");

    redIndex = map(analogRead(redPot), 0, maxRead, 0, numSteps);
    greenIndex = map(analogRead(greenPot), 0, maxRead, 0, numSteps);
    blueIndex = map(analogRead(bluePot), 0, maxRead, 0, numSteps);

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
      song3(speaker); 
      Serial2.println("MATCHED");
      delay(2000);  
      playing = false;             
      //return true;
    } else {
      noTone(speaker);
      //return false;
    }
    delay(10);
  }
  return true;
}

void RgbGameloop() {
  if (rgbGame()) {
    randomIndex = random(0, 8);
    Serial2.println("resetting...");
  }
}
