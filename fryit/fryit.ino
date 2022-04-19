#include <Arduino.h>
#include <TM1637Display.h>
#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

 int audioVar = 1;
 int CLK = 2;
 int DIO = 3;
 int ledRED = 4;
 int ledGRN = 5;
 int ledYLW = 6;
 int startBtn = 7;
 //int outputA = 8;
 int outputB = 9;

 int CLKStir = 8;
 int dataSet = A2;
 int CLKSet = A3;

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

 int counter = 0; 
 int aState;
 int aLastState;
 int aSetState = 0;
 int aSetLast = 0;
 int setCount = 0;
 int prevNum = -1;

 int currentState = 0;
 int lastState = LOW;
 unsigned long pressedTime = 0;
 unsigned long releasedTime = 0;
 int frySum = 0;

 int prevDisplay = 0;

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

void setup() {
  // put your setup code here, to run once:
  // Serial.begin (9600); 
  //Analog
    pinMode(A0, INPUT); //stirIt
 // pinMode(A1, INPUT); //setItU
 // pinMode(A2, INPUT); // flipIt

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
  pinMode(dataSet, INPUT);
  pinMode(CLKSet, INPUT);
//  pinMode (outputA,INPUT);
  pinMode (outputB,INPUT);

  aLastState = digitalRead(8);
  aSetLast = digitalRead(A3);
  
  randomSeed(42);

  display.setBrightness(0x0f);

  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    //Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(10);
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  startGame(); 
}

void startGame() {
  digitalWrite(ledYLW, HIGH);
  currentState = digitalRead(startBtn);
  //When the start button is flipped, start the game.

  if(digitalRead(startBtn) == HIGH)
  {
    digitalWrite(ledYLW, LOW);
    //digitalWrite(ledRED, HIGH);
    delay(300);
    logic();
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
    stirCurrent = 0;
    stirIt.curMode = 0;
    setIt.curMode = 0;
    fryIt.curMode = 0;
    setCurrent = 0;
    prevClkSet = 0;
    prevClkStir = 0;
    counter = 0;
    prevDisplay = 0;
    frySum = 0;
    
    randNum = random(1,4);
    delay(500);
    
    digitalWrite(ledGRN, LOW);
    digitalWrite(ledRED, LOW);
    setNumber = random(0,10);

    if(prevNum == setNumber)
    {
      if(setNumber == 9)
        setNumber--;
      else if(setNumber == 0) 
        setNumber++;
      else setNumber++;
    }
    
    if(randNum == 1){
      
      display.showNumberDec(1);////////////////////////////////////////////////////////////////
      
      for(int i = 0; i < 4; i++){
        tone(1, 350);
        delay(200);
        noTone(1);
        delay(10); 
      }
    }
    else if(randNum == 2){
     display.showNumberDec(2);////////////////////////////////////////////////////////////////
  
     for(int i = 0; i < 2; i++){
        tone(1, 500);
        delay(250);
        noTone(1);
        delay(10);
        tone(1, 450);
        delay(250);
        noTone(1);
        delay(10);
        tone(1, 400);
        delay(250);
        noTone(1);
        delay(10);
        tone(1, 450);
        delay(250);
        noTone(1);
        delay(10);
      } 

      delay(50);
      display.showNumberDec(setNumber);
      
    }
    else {
      
     display.showNumberDec(3);////////////////////////////////////////////////////////////////
      for(int i = 0; i < 3; i++){
        tone(1, 500);
        delay(200);
        noTone(1);
        delay(10);
      tone(1, 500);
      delay(500);
      noTone(1);
        delay(10);
      }
    }
    
      
    prevNum = setNumber;
   // display.showNumberDec(setNumber);////////////////////////////////////////////////////////
    delay(200);
   timeLimit = millis() + 3000;// * (pow(1.1, 11.525 - (x / 5)) + 2);
    
    while((millis() < timeLimit) && ((stirIt.curMode != -1) || (setIt.curMode != -1)|| (fryIt.curMode != -1))){  
      stirIt.curMode = isStir(stirCurrent);
      setIt.curMode = isSet(setNumber, setCurrent);
      fryIt.curMode = isFry();
    }
    
    stirIt.isCorrect = ((stirIt.comMode == randNum) && (stirIt.curMode == -1)) || ((stirIt.comMode != randNum) && (stirIt.curMode != -1));
    setIt.isCorrect = ((setIt.comMode == randNum) && (setIt.curMode == -1)) || ((setIt.comMode != randNum) && (setIt.curMode != -1));
    fryIt.isCorrect = ((fryIt.comMode == randNum) && (fryIt.curMode == -1)) || ((fryIt.comMode != randNum) && (fryIt.curMode != -1));

    if(stirIt.isCorrect && setIt.isCorrect && fryIt.isCorrect){
      count++;
      
      tone(1, 600);
      delay(500);
      noTone(1);
      delay(10);
     
      digitalWrite(ledGRN, HIGH);
      display.setSegments(good); ////////////////////////////////////////////////////////////////
      delay(500); 
    }
    else{
      tone(1, 250);
      delay(3000);
      noTone(1);
      delay(10);
      
      digitalWrite(ledRED, HIGH);
      display.setSegments(fail);////////////////////////////////////////////////////////////////
      delay(500);
      break;
    }
   }
   display.showNumberDec(count);////////////////////////////////////////////////////////////////
   count = 0;
   digitalWrite(ledYLW, LOW);
   
   delay(1000);
   
   startGame();
}

int isStir(int stirTemp) {

 aState = digitalRead(8); // Reads the "current" state of the outputA
   if (aState != aLastState){     
     if (digitalRead(9) != aState) { 
       counter ++;
     } else {
       counter ++;
     }
     display.showNumberDec(counter);
   } 
   //aLastState = aState; // Updates the previous state of the outputA with the current state
  
  /*curClkStir = digitalRead(CLKStir);
  if(curClkStir != prevClkStir){
    if(digitalRead(9)!= curClkStir)
    {
      stirTemp++;
      display.showNumberDec(55);
      delay(50);
    }
    else
    {
      stirTemp++;
      display.showNumberDec(99);
      delay(50);
    }
  }
  display.showNumberDec(stirCurrent);*/
  if(counter > 10)
    return -1;
  else{ 
  aLastState = aState; // Updates the previous state of the outputA with the current state
  //stirCurrent = stirTemp;
  return 500;
  }
}

int isSet(int setNumber, int setTemp) {
  //display.showNumberDec(setNumber);
  aSetState = digitalRead(A3);
  if (aSetState != aSetLast){     
     if (digitalRead(A2) != aSetState) { 
       setTemp ++;
     } else {
       setTemp ++;
     }
     
   }
  //display.showNumberDec(setTemp);
   //if(setTemp == 20)
    //setTemp = 0;
  
  /*curClkSet = digitalRead(A3);
  //display.showNumberDec(setTemp);
  if(curClkSet != prevClkSet){
    if(digitalRead(A2)!= curClkSet)
    {
      setTemp++;
    }  
    else
    {
     setTemp++;
    }
    //display.showNumberDec(setTemp);
  }*/
  if(setTemp == 20)
    setTemp = 0;
  
  //prevClkSet = curClkSet;
  aSetLast = aSetState;
  setCurrent = setTemp;
  int displayNum = setTemp/2;
 display.showNumberDec(displayNum);
  if(displayNum == setNumber)
  {
    return -1;
  }
  else{
    return 500;
  }
  
}

int isFry() {
  Vector normAccel = mpu.readNormalizeAccel();
  frySum += abs((10 - normAccel.ZAxis));

  //display.showNumberDec(frySum);
  
   if(frySum > 100)
  {
   // display.showNumberDec(normAccel.ZAxis);
    return -1;
  }
  else{
  // display.showNumberDec(normAccel.ZAxis);
    return 0;
  }
  
/*
 zval = analogRead(A2);
  int z = map(zval, 301, 443, -100, 100);
  float zg = (float)z/(-100.00);

  if(zg > -0.65)
    return -1;
  else return 0;
*/
}
