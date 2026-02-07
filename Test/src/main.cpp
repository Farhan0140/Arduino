#include <Arduino.h>
#include <IRremote.hpp>

#define IR_RECEIVE_PIN 15
#define IR_PIN 12


unsigned long ir_obstacle_prev_millis = 0;


void setup() {
  Serial.begin(9600);
  // IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  // pinMode(IR_PIN, INPUT);
}

void loop() {
  // IR Receiver 
  /*
  if (IrReceiver.decode()) {  
    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
        IrReceiver.resume();
        return;
    }

    String IRValue = String(IrReceiver.decodedIRData.decodedRawData, HEX);
    Serial.println(IRValue);

    Serial.println("--------------------");

    IrReceiver.resume();
  }
  */




  // IR Infrared Obstacle Avoidance Sensor
  /* 
  if(millis() - ir_obstacle_prev_millis >= 2000 && digitalRead(IR_PIN) == 0) {
    ir_obstacle_prev_millis = millis();
    Serial.println("Obstacle Detected..");
  }
  */
}
