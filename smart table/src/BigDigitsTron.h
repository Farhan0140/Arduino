#ifndef BIG_DIGITS_TRON_H
#define BIG_DIGITS_TRON_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class BigDigitsTron {
public:
    explicit BigDigitsTron(LiquidCrystal_I2C& lcd);
    void drawDigit(uint8_t digit, uint8_t x, uint8_t y);

private:
    LiquidCrystal_I2C& _lcd;

    uint8_t _fontDigits[10][4];
    uint8_t _buffer[8];

    void loadFont();
};

#endif
