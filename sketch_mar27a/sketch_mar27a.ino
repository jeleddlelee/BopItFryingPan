#include <Arduino.h>
#include <TM1637Display.h>

 int audioVar = 1;
 int CLK = 2;
 int DIO = 3;
 int ledRED = 4;
 int ledGRN = 5;
 int ledYLW = 6;
 int startBtn = 7;

 int CLKStir = 8;
 int CLKSet = 9;

 int startClkStir = 0;
 int startClkSet = 0;

 int curClkStir = 0;
 int curClkSet = 0;
 
 int count = 0;
 int timeLimit = 0;
 int randNum = 0;

 int zval;

 TM1637Display display(CLK, DIO);

 const uint8_t good[] = {
 SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,   // G
 SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
 SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
 SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
 };

 const uint8_t fail[] = {
 SEG_A | SEG_E | SEG_F | SEG_G,                   // F
 SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,   // A
 SEG_E | SEG_F,                                   // I
 SEG_D | SEG_E | SEG_F,                           // L
 };

 uint8_t data[] = { 0x0, 0x0, 0x0, 0x0};

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600); 
  //Analog
  pinMode(A0, INPUT); //stirIt
  pinMode(A1, INPUT); //setIt
  pinMode(A2, INPUT); // flipIt

  pinMode(CLKStir, INPUT);
  pinMode(CLKSet, INPUT);

  startClkStir = digitalRead(CLKStir);
  startClkSet = digitalRead(CLKSet);
 
  //Digital
  pinMode(audioVar, OUTPUT); //speaker
  pinMode(DIO, OUTPUT); //7-segment first place
  pinMode(ledRED, OUTPUT); //Red Leds
  pinMode(ledGRN, OUTPUT); //Green Leds
  pinMode(ledYLW, OUTPUT); //Yellow Leds
  pinMode(startBtn, INPUT); //Start button
}

void loop() {
  // put your main code here, to run repeatedly:
  startGame(); 
}

void startGame() {
  digitalWrite(ledYLW, HIGH);
  //When the start button is flipped, start the game.
  if(digitalRead(startBtn) == HIGH)
    logic();
}

void audio(int choice){
  /*
   * accepts integer arguement
   * integer corresponds to 5 sounds made
   */
   switch(choice){
    case '1':
      for(int i = 0; i < 4; i++){
        tone(1, 350, 500);
        delay(50); 
      }
      break;
     
     case '2':
      for(int i = 0; i < 2; i++){
        tone(1, 500, 250);
        tone(1, 450, 250);
        tone(1, 400, 250);
        tone(1, 450, 250);
      }
      break;
     
     case'3':
      for(int i = 0; i < 3; i++){
        tone(1, 500, 250);
      tone(1, 500, 600);
      }
      break;
     case'4'://Win
      tone(1, 600, 200);
      break;
     
     case'5'://Fail
      tone(1, 250, 2000);
      break;
   }
   
}

struct UserChoice {
  int comMode;
  int curMode;
  bool isCorrect;
};

UserChoice stirIt, setIt, fryIt;


void logic() {

   stirIt.comMode = 1;
   setIt.comMode = 2;
   fryIt.comMode = 3;

   for(int x = 0; x < 99; x++){
    
    stirIt.curMode = LOW;
    setIt.curMode = LOW;
    fryIt.curMode = LOW;
    randNum = random(1,4);
    
    audio(randNum);
    digitalWrite(ledGRN, LOW);
    
    timeLimit = millis() + 1000 * (pow(1.1, 11.525 - (x / 5)) + 2);
    
    while((millis() < timeLimit) || (stirIt.curMode == HIGH) || (setIt.curMode == HIGH) || (fryIt.curMode == HIGH)){ 
      stirIt.curMode = isStir();
      setIt.curMode = isSet();
      fryIt.curMode = isFry();
      delay(10);
    }
    
    stirIt.isCorrect = ((stirIt.comMode == randNum) && (stirIt.curMode == HIGH)) || ((stirIt.comMode != randNum) && (stirIt.curMode == LOW));
    setIt.isCorrect = ((setIt.comMode == randNum) && (setIt.curMode == HIGH)) || ((setIt.comMode != randNum) && (setIt.curMode == LOW));
    fryIt.isCorrect = ((fryIt.comMode == randNum) && (fryIt.curMode == HIGH)) || ((fryIt.comMode != randNum) && (fryIt.curMode == LOW));

    if(stirIt.isCorrect && setIt.isCorrect && fryIt.isCorrect){
      count++;
      audio(4);
      digitalWrite(ledGRN, HIGH);
      display.setSegments(good); 
      delay(500); 
    }
    else{
      audio(5);
      digitalWrite(ledRED, HIGH);
      display.setSegments(fail);
      delay(500);
      break;
    }
   }
   display.showNumberDec(count);
   count = 0;
   
   delay(1000);
   
   display.setSegments(data);
   startGame();
}

int isStir() {
  audio(2);
  int rotating = 0;
  curClkStir = digitalRead(CLKStir);

  if(curClkStir != startClkStir){
    if(digitalRead(A0)!= curClkStir)
      rotating++;
    else
      rotating++;
  }

  if(rotating > 6)
    return HIGH;
  else return LOW;
}

int isSet() {
  audio(1);
  
  return LOW;
}

int isFry() {
  audio(3);
  zval = analogRead(A2);
  int z = map(zval, 301, 443, -100, 100);
  float zg = (float)z/(-100.00);

  if(zg < 0.70 || zg > 1.10)
    return HIGH;
  else return LOW;
}
