#include <Arduino.h>                 // Core Arduino functions
#include <WiFi.h>                    // ESP32 WiFi library
#include <Firebase_ESP_Client.h>     // Firebase client library

// Helper files for Firebase authentication & RTDB debugging
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#include <IRremote.hpp>   // IRremote v4.x+


// ---------------- WIFI CREDENTIALS ----------------
#define WIFI_SSID " "                // Your WiFi SSID
#define WIFI_PASSWORD "        "     // Your WiFi password

// ---------------- FIREBASE CONFIG ----------------
#define API_KEY "AIzaSyCHrJBEwK4yEG61A6lZT4LJCaTBeuPMjnM"
#define DATABASE_URL "https://led-control-system-74ee3-default-rtdb.asia-southeast1.firebasedatabase.app/"

// ---------------- PIN DEFINITIONS ----------------

// For Table Lamp
#define TABLE_LAMP_RELAY_PIN 25      // Digital ON/OFF LED
#define LDR_PIN 36                   // LDR connected to ADC pin

// For Cooling Fan
#define COOLING_FAN_RELAY_PIN 26

// For sound box
#define SOUND_BOX_RELAY_PIN 32

// IR Receiver
#define IR_RECEIVER_PIN 15         // IR receiver data pin


// ---------------- FIREBASE OBJECTS ----------------
FirebaseData fbdo;                   // Firebase data object
FirebaseAuth auth;                   // Authentication object
FirebaseConfig config;               // Configuration object

// ---------------- GLOBAL VARIABLES ----------------
// For Table Lamp
bool is_table_lamp_on = true;

// For Cooling Fan
bool is_cooling_fan_on = true;

// For sound box
bool is_sound_box_on = true;



unsigned long sendDataPrevMillis = 0;  // Timer for sending sensor data
unsigned long readDataPrevMillis = 0;  // Timer for receiving data
unsigned long IRPrevMillis = 0;        // Timer for receiving data

int ldrData = 0;                       // Raw LDR ADC value
float voltage = 0.0;                   // LDR voltage value

bool signupOK = false;                 // Firebase signup status

bool start_the_process = false;
bool fan_on = false;
unsigned long fan_timer = 0;
const unsigned long FAN_INTERVAL = 300000; // 5 minutes


// ---------------- SETUP FUNCTION ----------------
void setup() {

  Serial.begin(115200);               // Start serial monitor

  pinMode(TABLE_LAMP_RELAY_PIN, OUTPUT);          // Digital LED as output
  pinMode(COOLING_FAN_RELAY_PIN, OUTPUT);         // Digital FAN as output
  pinMode(SOUND_BOX_RELAY_PIN, OUTPUT);           // Digital Sound Box as output
  IrReceiver.begin(IR_RECEIVER_PIN, ENABLE_LED_FEEDBACK);


  // ---------------- WIFI CONNECTION ----------------
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to wiFi");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }

  Serial.println();
  Serial.print("Connected With IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // ---------------- FIREBASE SETUP ----------------
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  // Anonymous Firebase sign-up
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Sign-UP OK");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

// ---------------- LOOP FUNCTION ----------------
void loop() {

  // -------- SEND SENSOR DATA EVERY 5 SECONDS --------
  // if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)) {
  //   sendDataPrevMillis = millis();

  //   // Read LDR sensor
  //   ldrData = analogRead(LDR_PIN);
  //   voltage = (float)analogReadMilliVolts(LDR_PIN) / 1000;

  //   // Upload LDR raw data
  //   if (Firebase.RTDB.setInt(&fbdo, "Sensor/LDR_DATA", ldrData)) {
  //     //Serial.printf("\n%d\n", ldrData);
  //   } else {
  //     // Serial.println(fbdo.errorReason());
  //   }

  //   // Upload LDR voltage
  //   if (Firebase.RTDB.setFloat(&fbdo, "Sensor/LDR_Voltage", voltage)) {
  //     //Serial.printf("\n%f\n", voltage);
  //   } else {
  //     // Serial.println(fbdo.errorReason());
  //   }
  // }

  // -------- REMOTE + DIGITAL LED|FAN LOGIC --------
  if (millis() - readDataPrevMillis > 1000 || readDataPrevMillis == 0) {
    readDataPrevMillis = millis();

    if (Firebase.ready() && signupOK) {
      // Read Table Lamp ON/OFF command from Firebase
      if (Firebase.RTDB.getBool(&fbdo, "Table/Table_Lamp")) {
        if (fbdo.dataType() == "boolean") {
          is_table_lamp_on = fbdo.boolData();
          Serial.printf("From Table/Table Lamp%d\n\n", is_table_lamp_on);
        }
      } else {
        Serial.print("Error From Table Lamp ");
        Serial.println(fbdo.errorReason());
      }
      

      // Read Cooling Fan ON/OFF command from Firebase
      if (Firebase.RTDB.getBool(&fbdo, "Table/Cooling_Fan")) {
        if (fbdo.dataType() == "boolean") {
          is_cooling_fan_on = fbdo.boolData();
          Serial.printf("From Table/Table Cooling Fan%d\n\n", is_cooling_fan_on);
        }
      } else {
        Serial.print("Error From Table Cooling fan ");
        Serial.println(fbdo.errorReason());
      }


      // Read Sound Box ON/OFF command from Firebase
      if (Firebase.RTDB.getBool(&fbdo, "Table/Sound_Box")) {
        if (fbdo.dataType() == "boolean") {
          is_sound_box_on = fbdo.boolData();
          Serial.printf("From Table/Sound_Box Fan%d\n\n", is_sound_box_on);
        }
      } else {
        Serial.print("Error From Table Sound Box ");
        Serial.println(fbdo.errorReason());
      }


      // Read Start the hole precess command from Firebase
      if (Firebase.RTDB.getBool(&fbdo, "Power_Mode/Fan_and_Lamp")) {
        if (fbdo.dataType() == "boolean") {
          start_the_process = fbdo.boolData();
          Serial.printf("From Power_Mode/Fan_and_Lamp%d\n\n", start_the_process);
        }
      } else {
        Serial.print("Error From Power_Mode/Fan_and_Lamp ");
        Serial.println(fbdo.errorReason());
      }
    }
    
  }

  if(start_the_process) {
    if (millis() - fan_timer >= FAN_INTERVAL) {
      fan_timer = millis();
      fan_on = !fan_on;   // 🔁 TOGGLE fan state
    }

    if (fan_on) {
      is_cooling_fan_on = true;   // turn fan ON
    } else {
      is_cooling_fan_on = false;  // turn fan OFF
    }

    // take data from LDR and turn on or off the table lamp
    // TODO
  }

  // if (IrReceiver.decode()) {

  //   // Filter repeated signals using millis()
  //   if (millis() - IRPrevMillis > 300) {

  //     // Print raw IR data
  //     Serial.print("IR Code: 0x");
  //     Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);

  //     IRPrevMillis = millis();
  //   }

  //   // Prepare for next IR signal
  //   IrReceiver.resume();
  // }


  
  digitalWrite(TABLE_LAMP_RELAY_PIN, is_table_lamp_on);
  digitalWrite(COOLING_FAN_RELAY_PIN, is_cooling_fan_on);
  digitalWrite(SOUND_BOX_RELAY_PIN, is_sound_box_on);
}
