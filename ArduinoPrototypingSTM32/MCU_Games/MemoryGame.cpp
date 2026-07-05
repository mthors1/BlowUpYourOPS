#include <Arduino.h>

const int speaker = PD1;
#include "Pitches.h"

const int B_LED = PD3;
const int G_LED = PD6;
const int Y_LED = PB3;
const int R_LED = PB5;
const int B_BUT = PD7;
const int G_BUT = PB8;
const int Y_BUT = PB4;
const int R_BUT = PB7;

const int DONE_LED = PD2;

const int LED_PINS[] = {B_LED, G_LED, Y_LED, R_LED};
const int BUT_PINS[] = {B_BUT, G_BUT, Y_BUT, R_BUT};
const int tones[] = {1915, 1700, 1519, 1432, 2700}; // Last tone is for failure

// Game settings
int n_levels = 20;
int speedfactor = 3;
int leddelay = 300;
int ledtime = 700;
int butwait = 800;

int buttonState[] = {0,0,0,0};
int lastButtonState[] = {0,0,0,0};

int n_array[50]; 
int u_array[50];

int game_on = 0;
int wait = 0;
int currentlevel = 1;
long rand_num = 0;
int rando = 0;
int pinandtone = 0;
int right = 0;

void MemoryGamesetup() {
  Serial.begin(115200);

  randomSeed(micros());
  pinMode(B_BUT, INPUT_PULLUP);
  pinMode(G_BUT, INPUT_PULLUP);
  pinMode(Y_BUT, INPUT_PULLUP);
  pinMode(R_BUT, INPUT_PULLUP);

  pinMode(B_LED, OUTPUT);
  pinMode(G_LED, OUTPUT);
  pinMode(Y_LED, OUTPUT);
  pinMode(R_LED, OUTPUT);
  pinMode(speaker, OUTPUT);
}

void memoryGameInit() {
  pinMode(B_BUT, INPUT_PULLUP);
  pinMode(G_BUT, INPUT_PULLUP);
  pinMode(Y_BUT, INPUT_PULLUP);
  pinMode(R_BUT, INPUT_PULLUP);

  pinMode(B_LED, OUTPUT);
  pinMode(G_LED, OUTPUT);
  pinMode(Y_LED, OUTPUT);
  pinMode(R_LED, OUTPUT);
  pinMode(speaker, OUTPUT);
}

int getLEDIndex(int pin) {
  for (int i = 0; i < 4; i++) {
    if (LED_PINS[i] == pin) return i;
  }
  Serial.println("error!");
  return 0;
}

// Skeleton code developed by Sai Poojitha, https://www.tinkercad.com/things/9s8pjKKYGmk-memory-game 
bool memoryGame() {
  delay(700);
  int i;

  if (game_on == 0){
    for(i = 0; i < n_levels; i++){
      n_array[i]=0;
      u_array[i]=0;
      rand_num = random(1,200);
      if (rand_num <= 50) {rando=0;}
      else if (rand_num <= 100) {rando=1;}
      else if (rand_num <= 150) {rando=2;}
      else {rando=3;}
      n_array[i]=rando;
    }
    game_on = 1; 
  }

  if (wait == 0){
    delay(200);
    for (i = 0; i < currentlevel; i++) {
      leddelay = ledtime / (1 + (speedfactor / n_levels) * (currentlevel - 1));
      pinandtone = n_array[i];
      digitalWrite(LED_PINS[pinandtone], HIGH);
      playTone(tones[pinandtone], leddelay, speaker);
      digitalWrite(LED_PINS[pinandtone], LOW);
      delay(200 / speedfactor);
    }
    wait = 1;
  }

  int buttonchange = 0;    
  int j = 0;

  while (j < currentlevel) {    
    buttonchange = 0;

    while (buttonchange == 0) {
      for (i = 0; i < 4; i++) {
        int currentState = digitalRead(BUT_PINS[i]);
        if (currentState == HIGH && lastButtonState[i] == LOW) {
          buttonchange = 1;
          u_array[j] = i;
          digitalWrite(LED_PINS[i], HIGH);
          playTone(tones[i], ledtime, speaker);
          digitalWrite(LED_PINS[i], LOW);
          wait = 0;
          lastButtonState[i] = currentState;
          break;
        }
        lastButtonState[i] = currentState;
      }
    }

    if (u_array[j] == n_array[j]) {
      j++;  
      right = 1;
    } else { 
      right = 0;
      j = currentlevel;
      wait = 0;
    }
  }

  if (right == 0){
    delay(300);
    for (i = 0; i < 4; i++){
      digitalWrite(LED_PINS[i], HIGH);
    }
    playTone(tones[4], ledtime, speaker);
    for (i = 0; i < 4; i++){
      digitalWrite(LED_PINS[i], LOW); 
    }
    delay(300);
    for (i = 0; i < 4; i++){
      digitalWrite(LED_PINS[i], HIGH);
    }
    playTone(tones[4], ledtime, speaker);
    for (i = 0; i < 4; i++){
      digitalWrite(LED_PINS[i], LOW); 
    }
    delay(500);
    game_on = 0;
    currentlevel = 1;
  }

  if (right == 1){
    currentlevel++;
    wait = 0;
  }

  if (currentlevel == n_levels) {
    int victory_notes[] = {
      NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_G4, NOTE_GS4,
      NOTE_A4, NOTE_C5
    };
    int victory_durs[] = {
      2, 2, 5, 2, 2, 5, 2, 2,
      5, 9
    };
    int num_notes = sizeof(victory_notes) / sizeof(int);
    int NOTE_DUR_temp = 25;

    delay(500);
    for (int i = 0; i < num_notes; i++) {
      int led_index = i % 4;
      digitalWrite(LED_PINS[led_index], HIGH);
      tone(speaker, victory_notes[i]);
      delay(victory_durs[i] * NOTE_DUR_temp);
      noTone(speaker);
      digitalWrite(LED_PINS[led_index], LOW);
      delay(victory_durs[i] * NOTE_DUR_temp / 3);
    }
    delay(500);
    game_on = 0;
    currentlevel = 1;
    n_levels += 2;
    speedfactor += 1;
    // digitalWrite(mem_game_LED, HIGH);
    delay(2000);
    return true;
  }
  return false;
}

void MemoryGameloop() {
  memoryGame();
}
