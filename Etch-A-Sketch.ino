#include <SPI.h>
#include "SH1106_SPI.h"

SH1106_SPI_FB lcd;

#define BUTTON_LEFT  4
#define BUTTON_RIGHT 5

void setup(void)
{
  Serial.begin(9600);
  lcd.begin();
  lcd.print(F("Etch A Sketch"));
  lcd.renderAll();
  delay(1000);
  drawClear();

  pinMode(BUTTON_LEFT, INPUT);
  digitalWrite(BUTTON_LEFT, HIGH);
  pinMode(BUTTON_RIGHT, INPUT);
  digitalWrite(BUTTON_RIGHT, HIGH);
}

void drawCursor(int h, int v)
{
}

void drawClear(void)
{
  for (int y = 0; y < SH1106_Y_PIXELS; y++) {
    if (y>0) {
      for(int x = 0; x < SH1106_X_PIXELS; x++) {
        lcd.setPixel(x, y-1, 0);
      }
    }
    lcd.writeLine(0,y,SH1106_X_PIXELS-1,y);
    lcd.renderAll();
    delay(8);
  }
  lcd.clear();
}


void loop(void)
{
  static bool penDown = false;
  static int phase = 0;
  static unsigned long next_serial_timestamp = 0;
  int H = map(analogRead(A0), 0, 1023, 0, SH1106_X_PIXELS - 1);
  int V = map(analogRead(A1), 0, 1023, 0, SH1106_Y_PIXELS - 1);


  if (digitalRead(BUTTON_LEFT) == LOW) {
    Serial.println("BUTTON_LEFT pushed");
    penDown = false;
    drawClear();
    while (digitalRead(BUTTON_LEFT) == LOW) ;
  }

  if (digitalRead(BUTTON_RIGHT) == LOW) {
    Serial.println("BUTTON_RIGHT pushed");
    penDown = !penDown;
    while (digitalRead(BUTTON_RIGHT) == LOW) ;
  }

  if (next_serial_timestamp < millis()) {
    Serial.print("Pen is ");
    Serial.println(penDown ? "down" : "up");
    Serial.print("H="); Serial.println(H);
    Serial.print("V="); Serial.println(V);
    next_serial_timestamp = millis() + 1000;
  }
  drawCursor(H, V);
  if (penDown) {
    lcd.setPixel(H, V, 255);
  }
  lcd.renderAll();
}

