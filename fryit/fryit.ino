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

 int prevClkStir = 0;
 int prevClkSet = 0;

 int curClkStir = 0;
 int curClkSet = 0;
 
 int count = 0;
 unsigned long timeLimit = 0;
 int randNum = 0;
 int rotating = 0;
 
 int setNumber = 0;
 int stirCurrent = 0;
 int setCurrent = 0;

 int prevNum = -1;

 int longPressTime = 3000;
 int currentState = 0;
 int lastState = LOW;
 unsigned long pressedTime = 0;
 unsigned long releasedTime = 0;

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
  pinMode(A1, INPUT); //setItU
  pinMode(A2, INPUT); // flipIt

  pinMode(CLKStir, INPUT);
  pinMode(CLKSet, INPUT);

  prevClkStir = digitalRead(CLKStir);
  prevClkSet = digitalRead(CLKSet);
 
  //Digital
  pinMode(audioVar, OUTPUT); //speaker
  pinMode(CLK, OUTPUT);
  pinMode(DIO, OUTPUT); //7-segment first place
  pinMode(ledRED, OUTPUT); //Red Leds
  pinMode(ledGRN, OUTPUT); //Green Leds
  pinMode(ledYLW, OUTPUT); //Yellow Leds
  pinMode(startBtn, INPUT); //Start button

  randomSeed(42);

  display.setBrightness(0x0f);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(5000);
  startGame(); 
}

void startGame() {
  digitalWrite(ledYLW, HIGH);
  currentState = digitalRead(startBtn);
  //When the start button is flipped, start the game.

  if(digitalRead(startBtn) == HIGH)
  {
    digitalWrite(ledYLW, LOW);
    digitalWrite(ledRED, HIGH);
    delay(300);
    logic();
  }

  /*if(lastState == HIGH && currentState == LOW){
    display.showNumberDec(1);
    delay(500);
    pressedTime = millis();}
  else if(lastState == LOW && currentState == HIGH){
    releasedTime == millis();

    long pressDuration = releasedTime - pressedTime;

    display.showNumberDec(2);
    delay(500);

    if( pressDuration > longPressTime ){
      digitalWrite(ledYLW, LOW);
      display.showNumberDec(3);
      delay(1000);
      logic();
    }
    
   }

   lastState = currentState;*/
    
}

void audio(int choice){
  /*
   * accepts integer arguement
   * integer corresponds to 5 sounds made
   */
   switch(choice){
    case '1':
      for(int i = 0; i < 4; i++){
        tone(1, 350);
        delay(200); 
      }
      break;
     
     case '2':
      for(int i = 0; i < 2; i++){
        tone(1, 500);
        delay(250);
        tone(1, 450);
        delay(250);
        tone(1, 400);
        delay(250);
        tone(1, 450);
        delay(250);
      }
      break;
     
     case'3':
      for(int i = 0; i < 3; i++){
        tone(1, 500);
        delay(200);
      tone(1, 500);
      delay(500);
      }
      break;
     case'4'://Win
      tone(1, 600);
      delay(500);
      break;
     
     case'5'://Fail
      tone(1, 250);
      delay(3000);
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
    
    stirIt.curMode = 0;
    setIt.curMode = 0;
    fryIt.curMode = LOW;
    randNum = random(1,4);
    
    digitalWrite(ledGRN, LOW); 
    audio(randNum);

    setNumber = random(0,10);

    if(prevNum == setNumber)
    {
      if(setNumber == 9)
        setNumber--;
      else if(setNumber == 0) 
        setNumber++;
      else setNumber++;
    }
      
    prevNum = setNumber;
    display.showNumberDec(375);//DEBUG
    timeLimit = millis() + 1000;// * (pow(1.1, 11.525 - (x / 5)) + 2);
    
    while((millis() < timeLimit)){// || (stirIt.curMode == -1) || (setIt.curMode == -1) || (fryIt.curMode == HIGH)){ 
      stirIt.curMode = isStir(stirIt.curMode);
      setIt.curMode = isSet(setNumber, setCurrent);
      fryIt.curMode = isFry();
    }
    display.showNumberDec(66);
    stirIt.isCorrect = ((stirIt.comMode == randNum) && (stirIt.curMode == -1)) || ((stirIt.comMode != randNum) && (stirIt.curMode != -1));
    setIt.isCorrect = ((setIt.comMode == randNum) && (setIt.curMode == -1)) || ((setIt.comMode != randNum) && (setIt.curMode != -1));
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
   digitalWrite(ledYLW, LOW);
   
   delay(1000);
   
   display.setSegments(data);
   startGame();
}

int isStir(int stirTemp) {
  display.showNumberDec(1);
  curClkStir = digitalRead(CLKStir);
  if(curClkStir != prevClkStir){
    if(digitalRead(A0)!= curClkStir)
      stirTemp++;
    else
      stirTemp++;
  }
  
  if(stirTemp > 6)
    return -1;
  else return stirTemp;
}

int isSet(int setNumber, int setTemp) {
  display.showNumberDec(2);
  curClkStir = digitalRead(CLKStir);
  
  if(curClkStir != prevClkStir){
    if(digitalRead(A0)!= curClkStir)
    {
      if(setTemp == 0) {
        setTemp = 10;
      }
      setTemp-=0.5;
    }  
    else
    {
      if(setTemp == 10) {
        setTemp = 0;
      }
     setTemp+=0.5;
    }
  }

  if(setTemp == setNumber)
    return -1;
  else return setTemp;
}

int isFry() {
  display.showNumberDec(3);
  zval = analogRead(A2);
  int z = map(zval, 301, 443, -100, 100);
  float zg = (float)z/(-100.00);

  if(zg < 0.70 || zg > 1.10)
    return HIGH;
  else return LOW;
}
