#include <Arduino.h>
#include <TM1637Display.h>

void setup() {

 
}

  #define CLK 2
  #define DIO 3
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

  void finalScore(int x) {
    int score = x;
    display.showNumberDec(score);
  };
  
void loop() {

  display.setBrightness(0x0f);

  uint8_t data[] = { 0x0, 0x0, 0x0, 0x0};
  display.setSegments(data);

  data[0] = display.encodeDigit(0);
  data[1] = display.encodeDigit(1);
  data[2] = display.encodeDigit(2);
  data[3] = display.encodeDigit(3);
  display.setSegments(data);

  delay(1000);
  display.clear();

  display.setSegments(good);
  delay(1000);

  display.setSegments(fail);
  delay(1000);

  finalScore(100);

  while(1);
}
