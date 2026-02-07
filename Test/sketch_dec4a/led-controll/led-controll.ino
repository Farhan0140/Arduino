#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Change address if needed (0x27 or 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned long startMillis;

void setup() {
  lcd.init();
  lcd.backlight();

  startMillis = millis();

  lcd.setCursor(0, 0);
  lcd.print("TIME:");
}

void loop() {
  unsigned long currentMillis = millis() - startMillis;

  unsigned long ms = currentMillis % 1000;
  unsigned long totalSeconds = currentMillis / 1000;

  unsigned int seconds = totalSeconds % 60;
  unsigned int minutes = (totalSeconds / 60) % 60;
  unsigned int hours   = (totalSeconds / 3600);

  lcd.setCursor(0, 1);

  // HH
  if (hours < 10) lcd.print("0");
  lcd.print(hours);
  lcd.print(":");

  // MM
  if (minutes < 10) lcd.print("0");
  lcd.print(minutes);
  lcd.print(":");

  // SS
  if (seconds < 10) lcd.print("0");
  lcd.print(seconds);
  lcd.print(":");

  // MS (3 digits)
  if (ms < 100) lcd.print("0");
  if (ms < 10)  lcd.print("0");
  lcd.print(ms);

  delay(50); // LCD refresh rate
}
