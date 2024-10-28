#include <Wire.h>
#include <hd44780.h>                       // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header

hd44780_I2Cexp lcd; // declare lcd object: auto locate & auto config expander chip

const int LCD_COLS = 16;
const int LCD_ROWS = 2;

const int RGB_PIN = 8;

void setup()
{
  int status;
  pinMode(RGB_PIN, OUTPUT);
  digitalWrite(RGB_PIN, LOW);
  Wire.begin(6, 7);

  status = lcd.begin(LCD_COLS, LCD_ROWS);
  if (status) // non zero status means it was unsuccesful
  {
    Serial.println("ERROR");
    hd44780::fatalError(status); // does not return
  }
  // Print a message to the LCD
  lcd.print("Hello, World!");
}

void loop() {}