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
 
 int SCORE = 0;
 unsigned long timeLimit = 0;
 int randNum = 0;
 
 int setNumber = 0;
 
 int stirCurrent = 0;
 int setCurrent = 0;

 int setCount = 0;
 int prevNum = -1;

 int frySum = 0;

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

  prevClkStir = digitalRead(8);
  prevClkSet = digitalRead(A3);
  
  //randomSeed(42);

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

  if(digitalRead(startBtn) == HIGH)
  {
    digitalWrite(ledYLW, LOW);
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


/*********************************************************
 *************************GAME****************************
 *********************************************************/
void logic() {
   
   ///////////////////////////////////////////////////////
   stirIt.comMode = 1;
   setIt.comMode = 2;
   fryIt.comMode = 3;
   ///////////////////////////////////////////////////////
   SCORE = 0;
    
   for(int x = 0; x < 99; x++){
    
    //////////////////////////////////////////////////////
    stirCurrent = 0;
    
    stirIt.curMode = 0;
    setIt.curMode = 0;
    fryIt.curMode = 0;
    
    setCurrent = 0;
    prevClkSet = 0;
    prevClkStir = 0;
    
    frySum = 0;

    digitalWrite(ledGRN, LOW);
    digitalWrite(ledRED, LOW);
    ///////////////////////////////////////////////////////
    
    
    ///////////////////////////////////////////////////////
    randNum = random(1,4);
    delay(500);
    
    setNumber = random(1,10);

    if(prevNum == setNumber)
    {
      if(setNumber == 9)
        setNumber--;
      else if(setNumber == 1) 
        setNumber++;
      else setNumber++;
    }

    prevNum = setNumber;
    ///////////////////////////////////////////////////////

    
    ///////////////////////////////////////////////////////
    if(randNum == 1){
      
      display.showNumberDec(1);
      
      for(int i = 0; i < 4; i++){
        tone(1, 350);
        delay(200);
        noTone(1);
        delay(10); 
      }
    }
    else if(randNum == 2){
     display.showNumberDec(2);
  
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
      
    }
    else {
      
     display.showNumberDec(3);
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
    ///////////////////////////////////////////////////////  

    display.showNumberDec(setNumber);
    delay(200);
    timeLimit = millis() + 1000 * (pow(1.1, 11.525 - (x / 5)) + 2);
        

    ///////////////////////////////////////////////////////
    while((millis() < timeLimit) && ((stirIt.curMode < 5) || (setIt.curMode != setNumber)|| (fryIt.curMode < 150))){  
      stirIt.curMode = isStir();
      setIt.curMode = isSet(setNumber);
      fryIt.curMode = isFry();
    }
    
    stirIt.isCorrect = ((stirIt.comMode == randNum) && (stirIt.curMode > 5)) || ((stirIt.comMode != randNum) && (stirIt.curMode < 5));
    setIt.isCorrect = ((setIt.comMode == randNum) && (setIt.curMode == setNumber)) || ((setIt.comMode != randNum) && (setIt.curMode != setNumber));
    fryIt.isCorrect = ((fryIt.comMode == randNum) && (fryIt.curMode > 150)) || ((fryIt.comMode != randNum) && (fryIt.curMode < 150));

    if(stirIt.isCorrect && setIt.isCorrect && fryIt.isCorrect){
      SCORE++;
      
      tone(1, 600);
      delay(500);
      noTone(1);
      delay(10);
     
      digitalWrite(ledGRN, HIGH);
      display.setSegments(good);
      delay(500); 
    }
    else{
      tone(1, 250);
      delay(3000);
      noTone(1);
      delay(10);
      
      digitalWrite(ledRED, HIGH);
      display.setSegments(fail);
      delay(500);
      break;
    }
   }
   display.showNumberDec(SCORE);
   digitalWrite(ledYLW, LOW);
   
   delay(1000);
   ///////////////////////////////////////////////////////
   
   startGame();
}

/*********************************************************
 *************************COMMANDS************************
 *********************************************************/
int isStir() {

 curClkStir = digitalRead(8); // Reads the "current" state of the outputA
   if (curClkStir != prevClkStir){     
     if (digitalRead(9) != curClkStir) { 
       stirCurrent++;
     } else {
       stirCurrent++;
     }
   } 
  
    prevClkStir = curClkStir; 
    return stirCurrent;
  
}

int isSet(int setNumber) {
  curClkSet = digitalRead(A3);
  if (curClkSet != prevClkSet){     
     if (digitalRead(A2) != curClkSet) { 
       setCurrent++;
     } else {
       setCurrent++;
     }
     
   }
  if(setCurrent == 20)
    setCurrent = 0;
  
  prevClkSet = curClkSet;
  int displayNum = setCurrent/2;
  display.showNumberDec(displayNum);
  
  return displayNum;
  
}

int isFry() {
  Vector normAccel = mpu.readNormalizeAccel();
  frySum += abs((10 - normAccel.ZAxis));
  return frySum;
  
}
