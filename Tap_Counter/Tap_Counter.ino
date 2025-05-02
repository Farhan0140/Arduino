#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // 16 chars, 2 lines

const int Up_buttonPin = 2;
const int Down_buttonPin = 3;

int buttonPushCounter = 1;  // Start at 1
int resetCounter = 1;       // Cycle: 1 → 2 → 3 → 1 ...

int up_buttonState = 0;
int up_lastButtonState = 0;

int down_buttonState = 0;
int down_lastButtonState = 0;

void setup() {
  Serial.begin(9600);
  pinMode(Up_buttonPin, INPUT_PULLUP);
  pinMode(Down_buttonPin, INPUT_PULLUP);

  lcd.init();  
  lcd.backlight();
  displayMainCounter();  
}

void loop() {
  checkUp();
  checkDown();
}

void checkUp() {
  up_buttonState = digitalRead(Up_buttonPin);

  if (up_buttonState != up_lastButtonState) {
    if (up_buttonState == LOW) {
      buttonPushCounter++;

      if (buttonPushCounter > 100) {
        buttonPushCounter = 1;
        resetCounter++;
        if (resetCounter > 3) {
          resetCounter = 1;
        }
      }

      displayMainCounter();
    }
    delay(50);  // debounce
  }
  up_lastButtonState = up_buttonState;
}

void checkDown() {
  down_buttonState = digitalRead(Down_buttonPin);

  if (down_buttonState != down_lastButtonState) {
    if (down_buttonState == LOW) {
      buttonPushCounter--;
      if (buttonPushCounter < 1) {
        buttonPushCounter = 1;
      }
      displayMainCounter();
    }
    delay(50);  // debounce
  }
  down_lastButtonState = down_buttonState;
}

void displayMainCounter() {
  lcd.clear();  
  lcd.setCursor(0, 0);
  lcd.print("Main: ");
  lcd.print(buttonPushCounter);

  lcd.setCursor(0, 1);
  if (resetCounter == 1) {
    lcd.print("LaIlahaIllallah");
  } else if (resetCounter == 2) {
    lcd.print("Astaghfirullah");
  } else if (resetCounter == 3) {
    lcd.print("LaHaulaWalakuata");
  }
}
