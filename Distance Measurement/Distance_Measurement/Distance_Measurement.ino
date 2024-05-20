#include <Wire.h> 

#include <LiquidCrystal_I2C.h>



LiquidCrystal_I2C lcd(0x27, 16, 2);

#define trigPin A0 //Sensor Trig pin connected to Arduino pin A0

#define echoPin A1 //Sensor Echo pin connected to Arduino pin A1

long distanceInch;



void setup()

{

  pinMode(trigPin, OUTPUT);

  pinMode(echoPin, INPUT);

  lcd.init();

  lcd.backlight();

  lcd.clear();

  lcd.setCursor(0,0);

  lcd.print("Simple  Circuits");

  delay(2000);

  lcd.clear();

  lcd.setCursor(0,0);  //Set LCD cursor to upper left corner, column 0, row 0

  lcd.print("Distance:");//Print Message on First Row

  lcd.setCursor(0,1);

  lcd.print("Distance:");

}



void loop()

{

  long duration, distance;

  digitalWrite(trigPin, LOW);

  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);

  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = (duration/2) / 29.1;

  distanceInch = duration*0.0133/2;



  

  

  lcd.setCursor(9,0);   

  lcd.print("                         ");  

  lcd.setCursor(9,0);    

  lcd.print(                distance); //Print measured distance

  lcd.print(" cm");  //Print your units.

  lcd.setCursor(9,1);  // 

  lcd.print("                         "); //Print blanks to clear the row

  lcd.setCursor(9,1);   

  lcd.print(distanceInch);

  lcd.print(" inch");  //Print your units. 

  

  delay(200); //pause to let things settle

}