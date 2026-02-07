#ifndef ON_BUTTON_H
#define ON_BUTTON_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class ON_BUTTON {
public:
    ON_BUTTON(LiquidCrystal_I2C &lcdRef) : lcd(lcdRef) {}

    void ON() {
        lcd.createChar(0, a);
        lcd.createChar(1, b);
        lcd.createChar(2, c);
        lcd.createChar(3, d);
        lcd.createChar(4, e);
        lcd.createChar(5, f);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.write(byte(0));
        lcd.write(byte(2));
        lcd.write(byte(4));

        lcd.setCursor(0, 1);
        lcd.write(byte(1));
        lcd.write(byte(3));
        lcd.write(byte(5));
    }

    void OFF() {
        lcd.createChar(0, g);
        lcd.createChar(1, h);
        lcd.createChar(2, i);
        lcd.createChar(3, j);
        lcd.createChar(4, k);
        lcd.createChar(5, l);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.write(byte(0));
        lcd.write(byte(2));
        lcd.write(byte(4));

        lcd.setCursor(0, 1);
        lcd.write(byte(1));
        lcd.write(byte(3));
        lcd.write(byte(5));
    }

private:
    LiquidCrystal_I2C &lcd;

    // Custom character definitions
    uint8_t a[8] = {
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00111,
        0b01000,
        0b10000,
        0b10000
    };

    uint8_t b[8] = {
        0b10000,
        0b10000,
        0b01000,
        0b00111,
        0b00000,
        0b00000,
        0b00000,
        0b00000
    };

    uint8_t c[8] = {
        B00000,
        B00000,
        B00000,
        B00000,
        B11111,
        B00000,
        B00011,
        B00111
    };

    uint8_t d[8] = {
        B00111,
        B00011,
        B00000,
        B11111,
        B00000,
        B00000,
        B00000,
        B00000
    };

    uint8_t e[8] = {
        B00000,
        B00000,
        B00000,
        B00000,
        B11100,
        B00010,
        B11001,
        B11101
    };

    uint8_t f[8] = {
        B11101,
        B11001,
        B00010,
        B11100,
        B00000,
        B00000,
        B00000,
        B00000
    };

    uint8_t g[8] = {
        B00000,
        B00000,
        B00000,
        B00000,
        B00111,
        B01000,
        B10011,
        B10111
    };

    uint8_t h[8] = {
        B10111,
        B10011,
        B01000,
        B00111,
        B00000,
        B00000,
        B00000,
        B00000
    };
    uint8_t i[8] = {
        B00000,
        B00000,
        B00000,
        B00000,
        B11111,
        B00000,
        B11000,
        B11100
    };


    uint8_t j[8] = {
        B11100,
        B11000,
        B00000,
        B11111,
        B00000,
        B00000,
        B00000,
        B00000
    };

    uint8_t k[8] = {
        B00000,
        B00000,
        B00000,
        B00000,
        B11100,
        B00010,
        B00001,
        B00001
    };

    uint8_t l[8] = {
        B00001,
        B00001,
        B00010,
        B11100,
        B00000,
        B00000,
        B00000,
        B00000
    };
};

#endif
