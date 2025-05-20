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
#include <math.h>


const int B_LED = 16;
const int G_LED = 18;
const int Y_LED = 20;
const int R_LED = 22;
const int speaker = 14;
const int B_BUT = 17;
const int G_BUT = 19;
const int Y_BUT = 21;
const int R_BUT = 6;


const int mem_game_LED = 3;
const int rgb_game_LED = 11;


const int LED_PINS[] = {B_LED, G_LED, Y_LED, R_LED};
const int BUT_PINS[] = {B_BUT, G_BUT, Y_BUT, R_BUT};
const int tones[] = {1915, 1700, 1519, 1432, 2700}; // Last tone is for failure


// Game settings
int n_levels = 8;
int speedfactor = 2;
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
int NOTE_DUR;


// === RGB LED Pins ===
const int redPin1 = 2;
const int greenPin1 = 7;
const int bluePin1 = 5;


const int redPin2 = 15;
const int greenPin2 = 13;
const int bluePin2 = 12;


const int redPot = 26;    // A0
const int greenPot = 27;  // A1
const int bluePot = 28;   // A2


int redVal1, greenVal1, blueVal1;
int redVal2, greenVal2, blueVal2;
int colorDistance = 0;

bool controller1executed = false;


void play(int note, int dur) {
  tone(speaker, note);
  delay(dur * NOTE_DUR);
  noTone(speaker);
  delay(dur * NOTE_DUR / 3);
}


void song3() {
  NOTE_DUR = 30;
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


void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 100L; i += tone * 2) {
    digitalWrite(speaker, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speaker, LOW);
    delayMicroseconds(tone);
  }
}


void setup() {
  Serial.begin(115200);
  Serial1.setTX(0);
  Serial1.setRX(1);
  Serial1.begin(115200);
  // pins for memory game
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
  // winning pins
  pinMode(mem_game_LED, OUTPUT);
  pinMode(rgb_game_LED, OUTPUT);
  // pins for rgb game
  analogWriteResolution(8); // 0–255 PWM
  pinMode(redPin1, OUTPUT);
  pinMode(greenPin1, OUTPUT);
  pinMode(bluePin1, OUTPUT);
  pinMode(redPin2, OUTPUT);
  pinMode(greenPin2, OUTPUT);
  pinMode(bluePin2, OUTPUT);
  pinMode(speaker, OUTPUT);
  // === Vibrant Random Color using HSV ===
  float h = random(0, 360);
  float s = 0.8;
  float v = 0.9;
  float r, g, b;
  int i = int(h / 60) % 6;
  float f = (h / 60) - i;
  float p = v * (1 - s);
  float q = v * (1 - f * s);
  float t = v * (1 - (1 - f) * s);
  switch (i) {
    case 0: r = v; g = t; b = p; break;
    case 1: r = q; g = v; b = p; break;
    case 2: r = p; g = v; b = t; break;
    case 3: r = p; g = q; b = v; break;
    case 4: r = t; g = p; b = v; break;
    case 5: r = v; g = p; b = q; break;
  }
  // Ensure the values do not go below 20
  redVal1 = max(int(r * 255), 20);
  greenVal1 = max(int(g * 255), 20);
  blueVal1 = max(int(b * 255), 20);
}


int getLEDIndex(int pin) {
  for (int i = 0; i < 4; i++) {
    if (LED_PINS[i] == pin) return i;
  }
  Serial.println("error!");
  return 0;
}


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
      playTone(tones[pinandtone], leddelay);
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
          playTone(tones[i], ledtime);
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
    playTone(tones[4], ledtime);
    for (i = 0; i < 4; i++){
      digitalWrite(LED_PINS[i], LOW);
    }
    delay(300);
    for (i = 0; i < 4; i++){
      digitalWrite(LED_PINS[i], HIGH);
    }
    playTone(tones[4], ledtime);
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
    NOTE_DUR = 25;


    delay(500);
    for (int i = 0; i < num_notes; i++) {
      int led_index = i % 4;
      digitalWrite(LED_PINS[led_index], HIGH);
      tone(speaker, victory_notes[i]);
      delay(victory_durs[i] * NOTE_DUR);
      noTone(speaker);
      digitalWrite(LED_PINS[led_index], LOW);
      delay(victory_durs[i] * NOTE_DUR / 3);
    }
    delay(500);
    game_on = 0;
    currentlevel = 1;
    n_levels += 2;
    speedfactor += 1;
    digitalWrite(mem_game_LED, HIGH);
    delay(2000);
    return true;
  }
  return false;
}


bool rgbGame() {
  int rawRed = analogRead(redPot);
  int rawGreen = analogRead(greenPot);
  int rawBlue = analogRead(bluePot);


  redVal2 = map(rawRed, 30, 1023, 0, 255);
  greenVal2 = map(rawGreen, 30, 1023, 0, 255);
  blueVal2 = map(rawBlue, 30, 1023, 0, 255);
  // Ensure that the controlled colors do not go below 20
  redVal2 = max(redVal2, 20);
  greenVal2 = max(greenVal2, 20);
  blueVal2 = max(blueVal2, 20);


  analogWrite(redPin1, redVal1);
  analogWrite(greenPin1, greenVal1);
  analogWrite(bluePin1, blueVal1);


  analogWrite(redPin2, redVal2);
  analogWrite(greenPin2, greenVal2);
  analogWrite(bluePin2, blueVal2);


  // === Perceptual RGB distance (weighted) ===
  // Use different weights for R, G, and B based on human color perception sensitivity
  int rDiff = redVal1 - redVal2;
  int gDiff = greenVal1 - greenVal2;
  int bDiff = blueVal1 - blueVal2;
  // Apply perceptual weights to each channel
  colorDistance = sqrt(0.3 * rDiff * rDiff + 0.59 * gDiff * gDiff + 0.11 * bDiff * bDiff);


  Serial.print("Random LED 1: (");
  Serial.print(redVal1); Serial.print(", ");
  Serial.print(greenVal1); Serial.print(", ");
  Serial.print(blueVal1); Serial.print(")       ");


  Serial.print("Controlled LED 2: (");
  Serial.print(redVal2); Serial.print(", ");
  Serial.print(greenVal2); Serial.print(", ");
  Serial.print(blueVal2); Serial.print(")    ");
  Serial.println(colorDistance);
  if (colorDistance <= 10) {
    song3();
    Serial.println("MATCHED");
    digitalWrite(rgb_game_LED, HIGH);
    delay(2000);
    return true;
  } else {
    noTone(speaker);
    return false; // Ensure silence when not matched
  }
  delay(10); // Smoother update rate
}


void loop() {
  while (!Serial1.available()){}
  uint8_t received = Serial1.read();
  Serial.print("Received byte: ");
  Serial.println(received);
  
  if (received == 1 && !controller1executed){
    Serial1.write((uint8_t)0);
    delay(300);
    Serial.println("STARTING MEMORY GAME");
    while (!memoryGame()) {Serial1.write((uint8_t)0);}
    Serial1.write((uint8_t)4);
    while (!rgbGame()) {Serial1.write((uint8_t)0);}
    controller1executed = true;
  }
  
  Serial.println("GOING BACK TO MAIN PICO");
  Serial1.write((uint8_t)5);
}



