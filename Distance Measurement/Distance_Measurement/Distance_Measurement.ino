#include "LiquidCrystal_I2C.h"
LiquidCrystal_I2C lcd(0x27,16,2);

int buttons[4] = {2, 3, 4,  5};
int leds[4]    = {8, 9, 10, 11};

boolean button[4] = {0, 0, 0, 0};

#define buzzer 6

#define levelsInGame 50

//create an array for this game
int bt_simonSaid[100]; //initialise the array
int led_simonSaid[100]; //initialise the array

boolean lost;
int game_play, level, stage;

void setup(){ // the setup function runs once when you press reset or power the board
Serial.begin(9600);

// initialize digital pins as outputs for LEDS in the buttons.
for(int i=0; i<=3; i++) {
pinMode(buttons[i], INPUT_PULLUP);// set the button pins as inputs
pinMode(leds[i], OUTPUT); // set the led pins as outputs
}

pinMode(buzzer, OUTPUT); 
 
lcd.begin();// initialize the lcd
lcd.backlight();
lcd.setCursor(0,0);
lcd.print("      HI   ");
lcd.setCursor(0,1);
lcd.print(" > I'm NaDiM <");
delay(2000);
lcd.setCursor(0,0);
lcd.print("   Welcome To   ");
lcd.setCursor(0,1);
lcd.print("> Memory  Game <");
delay(2000); // Waiting for a while
lcd.clear(); 

randomSeed(analogRead(0));  //make our random numbers more random
}


void loop() {// the loop function runs over and over again forever
  
switch(stage){
case 0:
      lcd.setCursor(0, 0); lcd.print("Press Red Button");
      lcd.setCursor(0, 1); lcd.print(" for Start Game ");
      button[0] = digitalRead(buttons[0]);
      while(button[0] == HIGH) {
        button[0] = digitalRead(buttons[0]);
      }
      level=1, stage=1, game_play=1;
break;
      
case 1:
      lcd.clear();
      //print level on screen
      lcd.setCursor(4,0);
      lcd.print("Level: ");
      lcd.print((level/10)%10);
      lcd.print(level%10);
 
      lcd.setCursor(0,1);
      lcd.print(" -- Memorize -- ");
      delay(1500);
      led_simonSaid[level] = random(8, 12); // populate the array with random 'colours'
      for(int i=1; i<=level; i++) {
        digitalWrite(led_simonSaid[i], HIGH);//turn on the button light
        playBuzzer(led_simonSaid[i]-7);
        digitalWrite(led_simonSaid[i], LOW);
        delay(400);
      }
      delay(500);
      stage=2;
break;
      
case 2:
      stage=3;
      lcd.setCursor(0, 1); 
      lcd.print("   -- Play --   "); 
break;

case 3:
     for(int i=0; i<=3; i++){
        button[i] = digitalRead(buttons[i]);
        if(button[i] == LOW){
          bt_simonSaid[game_play] = leds[i];
          digitalWrite(leds[i], HIGH);
          playBuzzer(i+1); //make the sound of the button pressed - right or wrong
          while(button[i] == LOW){
            button[i] = digitalRead(buttons[i]);
          }
          delay(50);
          digitalWrite(leds[i], LOW);
          game_play++;
          if(game_play-1==level){game_play=1;
            stage=4;
            break;
          }
        }
      }
      delay(10);
break;

case 4:
     lcd.setCursor(0, 1); 
     lcd.print("  Verification  ");
     delay(500);
     for(int i=1; i<=level; i++){
        if(led_simonSaid[i]!=bt_simonSaid[i]){
          lost=1;
          break;
        }
      }      
      if(lost==1)stage=5;
            else stage=6;
break;

case 5:
      lcd.setCursor(0, 1); 
      lcd.print(" !! You Lost !! ");
      tone(buzzer, 350); //play game over low tone on buzzer
      for(int i=0; i<=3; i++) {
      digitalWrite(leds[i], HIGH);  
      }
      delay(1000);
      lcd.setCursor(0, 1); 
      lcd.print("!! GAME  OVER !!");
      noTone(buzzer);
      delay(1000);
      for(int i=0; i<=3; i++) {
      digitalWrite(leds[i], LOW);  
      }
      level=1, stage=0, lost=0;
 break;

case 6:
      lcd.setCursor(0, 1); 
      lcd.print(" ** You  Win ** ");
      delay(1000);   
    if(level==levelsInGame){
      lcd.setCursor(0, 0); 
      lcd.print("Congratulation"); 
      lcd.setCursor(0, 1); 
      lcd.print(" Level Complete");
      delay(1000); 
      lcd.clear();           
      level=1;          
      }else{
      if(level<levelsInGame)level++;       
      }
      
      stage=1;
break;

default:
break;
}

}

void playBuzzer(int x) {
tone(buzzer, 650+(x*100));
delay(300);
noTone(buzzer);
}