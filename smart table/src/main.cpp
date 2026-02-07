// DIGITAL_PIN: 2,7,8,9,10    ANALOG_PIN: A7, A4, A5

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
bool LCD_Backlight = true; 
#include <IRremote.hpp>
#include "onButton.h"
#include <BigDigitsTron.h>

ON_BUTTON on_button(lcd);
BigDigitsTron tron(lcd);

// LDR
const int ldr = A7;
int ldr_val;
unsigned long ldr_prev_time = 0LL;
int ldr_threshold = 25;

// Ultrasonic Sensor
#define TRIG_PIN 9
#define ECHO_PIN 10
int _distance = 0;
int ultrasonic_sensor_threshold = 6;
unsigned long ultrasonic_sensor_prev_time = 0LL;

// Relay
const int fan_relay_pin = 5;
bool fan_relay_status = HIGH;
bool is_fan_relay_control_by_key = false;
const int led_relay_pin = 6;
bool led_relay_status = HIGH;
bool is_led_relay_control_by_key = false;

// IR remote
const int IR_PIN = 4;


// Menu
String upKey="2657158919", downKey="2673870599", leftKey="2590312199", rightKey="2640447239", okKey="2540177159", powerKey="4244768519";
const int menu_size = 9;
String Menu_Item[menu_size] = {" Home", " Tap Counter", " Table Lamp", " Cooling Fan", " Sound System", " LCD Backlit", " LDR Value", " U Sensor Value", " Shut Down"};
int menuPointer = 0;
int menuPointerPrev = 0;

int LCD00 = 0;
int LCD01 = 1;


// Time Counter
unsigned long previousMillisForTimer = 0;
const unsigned long TimerInterval = 1000; // 1 second

int seconds = 0;
int minutes = 0;
int hours   = 0;
bool is_display_time = false;

// Tap Counter
int tap_counter = 0;


void Print_ON_OFF_Button(bool state) {
  lcd.clear();

  if(state) {
    on_button.ON();
  } else {
    on_button.OFF();
  }
}

void Print_Menu() {
  lcd.clear();

  if(!(menuPointer == LCD00 || menuPointer == LCD01)) {
    LCD00 = menuPointer;
    LCD01 = menuPointer + 1;
  }
  
  Menu_Item[menuPointer][0] = '>';
  
  lcd.setCursor(0, 0);
  lcd.print(Menu_Item[LCD00]);
  lcd.setCursor(0, 1);
  lcd.print((LCD01 == menu_size)? "________________": Menu_Item[LCD01]);

  Menu_Item[menuPointer][0] = ' ';
}


void displayTime() {

  if (hours < 10) { 
    tron.drawDigit(0, 2, 0);
    tron.drawDigit(hours, 4, 0);
  } else {
    int first_digit = hours/10;
    int second_digit = hours%10;
    tron.drawDigit(first_digit, 2, 0);
    tron.drawDigit(second_digit, 4, 0);
  }

  lcd.print(".");
  
  if (minutes < 10) { 
    tron.drawDigit(0, 7, 0);
    tron.drawDigit(minutes, 9, 0);
  } else {
    int first_digit = minutes/10;
    int second_digit = minutes%10;
    tron.drawDigit(first_digit, 7, 0);
    tron.drawDigit(second_digit, 19, 0);
  }

  lcd.print(".");


  if (seconds < 10) { 
    tron.drawDigit(0, 12, 0);
    tron.drawDigit(seconds, 14, 0);
  } else {
    int first_digit = seconds/10;
    int second_digit = seconds%10;
    tron.drawDigit(first_digit, 12, 0);
    tron.drawDigit(second_digit, 14, 0);
  }
}

void display_counter(String s) {
  uint8_t col = 0;

  for (size_t i = 0; i < s.length(); i++) {
      if (s[i] < '0' || s[i] > '9') continue;

      uint8_t digit = s[i] - '0';
      tron.drawDigit(digit, col, 0);

      col += 2;              // 2 columns for digit + 1 space
      if (col >= 16) break;  // prevent LCD overflow
  }
}

void MENU(String key) {
  switch (menuPointer)
  {
    case 0: 
      lcd.clear();
      displayTime();
      is_display_time = true;
      break;

    case 1: {

      if(key == rightKey) {
        tap_counter++;
      } else if(key == powerKey) {
        tap_counter = 0;
        lcd.clear();
      } else {
        lcd.clear();
      }

      display_counter(String(tap_counter));
      break;
    }

    case 2:
      Print_ON_OFF_Button(!led_relay_status);

      if(key == powerKey) {
        led_relay_status = !led_relay_status;
        is_led_relay_control_by_key = !is_led_relay_control_by_key;
      }

      Print_ON_OFF_Button(!led_relay_status);
      break;
    
    case 3:
      Print_ON_OFF_Button(!fan_relay_status);

      if(key == powerKey) {
        fan_relay_status = !fan_relay_status;
        is_fan_relay_control_by_key = !is_fan_relay_control_by_key;
      }

      Print_ON_OFF_Button(!fan_relay_status);
      break;

    case 5:
      if(LCD_BACKLIGHT) {
        on_button.ON();
      } else {
        on_button.OFF();
      }
      
      if(key == powerKey) {
        if(LCD_Backlight) {
          lcd.noBacklight();
          on_button.OFF();
          LCD_Backlight = false;
        } else {
          lcd.backlight();
          on_button.ON();
          LCD_Backlight = true;
        }
      }

      break;
    
    case 6: {
      String digits = String(ldr_val);
      uint8_t col = 0;
      lcd.clear();

      for (size_t i = 0; i < digits.length(); i++) {
          if (digits[i] < '0' || digits[i] > '9') continue;

          uint8_t digit = digits[i] - '0';
          tron.drawDigit(digit, col, 0);

          col += 2;              // 2 columns for digit + 1 space
          if (col >= 16) break;  // prevent LCD overflow
      }

      break;
    }

    case 7: {
      String digits = String(_distance);
      uint8_t col = 0;
      lcd.clear();

      for (size_t i = 0; i < digits.length(); i++) {
          if (digits[i] < '0' || digits[i] > '9') continue;

          uint8_t digit = digits[i] - '0';
          tron.drawDigit(digit, col, 0);

          col += 2;              // 2 columns for digit + 1 space
          if (col >= 16) break;  // prevent LCD overflow
      }

      lcd.setCursor(col, 1);
      lcd.print(" cm");
      
      break;
    }
    
    default:
      is_display_time = false;
      Print_Menu();
      break;
  }
}


void setup() {

  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(fan_relay_pin, OUTPUT);
  pinMode(led_relay_pin, OUTPUT);

  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);
  pinMode(3, OUTPUT);

  Print_Menu();

}


void loop() {

  // Ultrasonic Sensor
  if(!is_fan_relay_control_by_key && (ultrasonic_sensor_prev_time == 0 || millis() - ultrasonic_sensor_prev_time >= 10000)) {

    ultrasonic_sensor_prev_time = millis();

    // Clear trigger
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    // Send a 10µs pulse to trigger pin
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    // Read echo time
    long duration = pulseIn(ECHO_PIN, HIGH);
    // Calculate distance (speed of sound = 343 m/s)
    float distance = duration * 0.034 / 2;
    _distance = round(distance);

    Serial.println(_distance);


    if(_distance <= ultrasonic_sensor_threshold) {
      fan_relay_status = LOW;
    } else {
      fan_relay_status = HIGH;
    }

  } 


  // LDR
  if(!is_led_relay_control_by_key && (ldr_prev_time == 0 || millis() - ldr_prev_time >= 2000)) {
    ldr_prev_time = millis();
    ldr_val = analogRead(ldr);

    if(ldr_val <= ldr_threshold) {
      led_relay_status = LOW;
    } else {
      led_relay_status = HIGH;
    }

  }


  // IR remote
  if(IrReceiver.decode()) {

    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
        IrReceiver.resume();
        return;
    }
    
    // uint32_t IRValue = IrReceiver.decodedIRData.decodedRawData;
    String IRValue = String(IrReceiver.decodedIRData.decodedRawData);

    Serial.println(IRValue);
    Serial.println();

    bool subMenu = 0;
    String tempKey;

    if(IRValue == upKey) {
      menuPointer = (menuPointer == menu_size-1) ? 0: menuPointer + 1;
    } else if(IRValue == downKey) {
      menuPointer = (menuPointer == 0) ? menu_size-1: menuPointer - 1;
    } else if(IRValue == okKey) {
      subMenu = 1;
      tempKey = okKey;
    } else if(IRValue == powerKey) {
      subMenu = 1;
      tempKey = powerKey;
    } else if(IRValue == rightKey) {
      subMenu = 1;
      tempKey = rightKey;
    } else {
      if(is_display_time == true) {
        is_display_time = false;
      }
    }

    if(!is_display_time) {
      subMenu ? MENU(tempKey) : Print_Menu();
    }

    IrReceiver.resume();
  }


  // Time Counter
  if (millis() - previousMillisForTimer >= TimerInterval) {
    previousMillisForTimer = millis();

    seconds++;

    if (seconds >= 60) {
      seconds = 0;
      minutes++;
    }

    if (minutes >= 60) {
      minutes = 0;
      hours++;
    }

    if (hours >= 24) {
      hours = 0;
    }

    if(is_display_time) {
      displayTime();
    }

  }

  digitalWrite(fan_relay_pin, fan_relay_status);
  digitalWrite(led_relay_pin, led_relay_status);

}