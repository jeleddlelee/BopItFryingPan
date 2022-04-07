#include <Arduino.h>
#include <TM1637Display.h>

 int audio = 1;
 int CLK = 2;
 int DIO = 3;
 int ledRED = 4;
 int ledGRN = 5;
 int ledYLW = 6;
 int startBtn = 7;

 int count = 0;
 int timeLeft = 0;

const uint8_t SEG_FAIL[] = {
    SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // F
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // A
    SEG_C | SEG_E | SEG_G,                           // I
    SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // L
    };

const uint8_t SEG_GOOD[] = {
    SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // g
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
    SEG_C | SEG_E | SEG_G,                           // O
    SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // d
    };
 
TM1637Display display(CLK, DIO);
 
void setup() {
  // put your setup code here, to run once:
  
  //Analog
  pinMode(A0, INPUT); //setIt
  pinMode(A1, INPUT); //stirIt
  pinMode(A2, INPUT); // flipIt
  
  //Digital
  pinMode(audio, OUTPUT); //speaker
  pinMode(DIO, OUTPUT); //7-segment first place
  pinMode(ledRED, OUTPUT); //Red Leds
  pinMode(ledGRN, OUTPUT); //Green Leds
  pinMode(ledYLW, OUTPUT); //Yellow Leds
  pinMode(startBtn, INPUT); //Start button
}

void loop() {
  // put your main code here, to run repeatedly:
  startGame(); //Allows for repeating Game
}

void startGame() {
  
  //When the start button is flipped, start the game.
  if(digitalRead(6) == HIGH)
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
     
     case'3'://THINK OF SOMETHING
      for(int i = 0; i < 2; i++){
        tone(1, 500, 250);
        tone(1, 450, 250);
        tone(1, 400, 250);
        tone(1, 450, 250);
      }
      break;
     case'4'://Win
      tone(1, 600, 200);
      }
      break;
     
     case'5'://Fail
      tone(1, 250, 2000);
      }
      break;
   }
   
}

struct UserChoice {
  int comMode;
  int curMode;
  bool isCorrect;
};

UserChoice stirIt, setIt, fryIt;

stirIt.comMode = 1;
setIt.comMode = 2;
fryIt.comMode = 3;

void logic() {
  /*
   * forLoop 0 to 99
   * randNum = random number gen(1 to 3)
   * while(TimeLeft and pinModes123 != 1)
   *  comnO = pinModeOfPotentiometerSetIt()
   *  comnT = pinModeOfPotentiometerStirIt()
   *  comnTh = pinModeOfPotentiometerFlipIt()
   * if(userChoice.correct != curMode) {
   *  break()
   *  displayFailure
   *  RED_ON
   * }
   * else {
   *  count = count + 1;
   *  updateTimeLeft(count);
   * }
   * displayScore()
   * resetScore()
   * startGame()
   */

   for(int x = 0; x < 99; x = x + 1){
    randNum = random(1,4);
    stirIt.curMode = analogRead(A0);
    stirIt.isCorrect = XAND((stirIt.comMode == randNUM), bool(stirIt.curMode));
    
    setIt.curMode = analogRead(A1);
    setIt.isCorrect = XAND((setIT.comMode == randNUM), bool(setIT.curMode));
 
    fryIt.curMode = analogRead(A2);
    fryIt.isCorrect = XAND((fryIt.comMode == randNUM), bool(fryIt.curMode));

    

   }
}

void displayScore(int scoreGiven) {//CALCULATION
  //7segment output SCORE
}

void resetScore() {
  count = 0;
}

void updateTimeLeft(int x) {
  timeLeft = pow(1.1, 11.525 - (x/5)) + 2;
}
