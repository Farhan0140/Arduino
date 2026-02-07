#include "BigDigitsTron.h"
#include <avr/pgmspace.h>

/* ---------- TRON FONT BITMAPS ---------- */
static const uint8_t tron_00[8] PROGMEM = {B11111,B11000,B00000,B00000,B00000,B00000,B00000,B00000};
static const uint8_t tron_01[8] PROGMEM = {B00000,B00000,B00000,B00000,B00000,B00000,B11000,B11111};
static const uint8_t tron_02[8] PROGMEM = {B11111,B00011,B00011,B00011,B00011,B00011,B00011,B00011};
static const uint8_t tron_03[8] PROGMEM = {B11111,B11000,B11000,B11000,B11000,B11000,B11000,B11111};
static const uint8_t tron_04[8] PROGMEM = {B11111,B11000,B11000,B11000,B11000,B11000,B11000,B11000};
static const uint8_t tron_05[8] PROGMEM = {B11000,B11000,B11000,B11000,B11000,B11000,B11000,B11000};
static const uint8_t tron_06[8] PROGMEM = {B11111,B00000,B00000,B00000,B00000,B00000,B00000,B11111};
static const uint8_t tron_07[8] PROGMEM = {B11000,B11000,B11000,B11000,B11000,B11000,B11000,B11111};

static const uint8_t tron_digits[10][4] = {
    {4,2,7,255}, {254,5,254,5}, {0,2,3,6}, {0,2,1,255},
    {7,1,254,5}, {3,6,1,255}, {3,6,3,255}, {0,2,254,5},
    {3,2,3,255}, {3,2,254,2}
};

BigDigitsTron::BigDigitsTron(LiquidCrystal_I2C& lcd)
    : _lcd(lcd) {}


void BigDigitsTron::loadFont() {
    memcpy(_fontDigits, tron_digits, sizeof(tron_digits));

    memcpy_P(_buffer, tron_00, 8); _lcd.createChar(0, _buffer);
    memcpy_P(_buffer, tron_01, 8); _lcd.createChar(1, _buffer);
    memcpy_P(_buffer, tron_02, 8); _lcd.createChar(2, _buffer);
    memcpy_P(_buffer, tron_03, 8); _lcd.createChar(3, _buffer);
    memcpy_P(_buffer, tron_04, 8); _lcd.createChar(4, _buffer);
    memcpy_P(_buffer, tron_05, 8); _lcd.createChar(5, _buffer);
    memcpy_P(_buffer, tron_06, 8); _lcd.createChar(6, _buffer);
    memcpy_P(_buffer, tron_07, 8); _lcd.createChar(7, _buffer);
}

void BigDigitsTron::drawDigit(uint8_t digit, uint8_t x, uint8_t y) {
    loadFont();
    if (digit > 9) return;

    _lcd.setCursor(x, y);
    _lcd.write(_fontDigits[digit][0]);
    _lcd.write(_fontDigits[digit][1]);

    _lcd.setCursor(x, y + 1);
    _lcd.write(_fontDigits[digit][2]);
    _lcd.write(_fontDigits[digit][3]);
}
