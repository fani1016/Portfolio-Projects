/*=======Including Libraries======= */
//Libraries for LoRa
#include <SPI.h>
#include <Wire.h>
#include "Adafruit_FONA.h"

/*=======Including Libraries======= */


/*=======Pins Definition======= */
//Serial pins
#define Tx 1
#define Rx 3

#define LED_BLUE  13
#define RELAY 14

#define SIM800L_RX     27
#define SIM800L_TX     26
#define SIM800L_PWRKEY 4
#define SIM800L_RST    5
#define SIM800L_POWER  23

/*=======Pins Definition======= */


/*=======Constants Definition======= */

/*=======Constants Definition======= */


/*=======Variables Definition======= */
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete


/*=======Variables Definition======= */


/*=======Functions Definition======= */
void lora_setup();
void lora_write();

/*=======Functions Definition======= */


/*=======Objects Definition======= */
HardwareSerial *sim800lSerial = &Serial1;
Adafruit_FONA sim800l = Adafruit_FONA(SIM800L_PWRKEY);

/*=======Objects Definition======= */


void setup() 
{
  Serial.begin(9600);
  inputString.reserve(200);
  
  pinMode(LED_BLUE, OUTPUT);
  pinMode(RELAY, OUTPUT);
  
  sim800l_setup(); 
}


void loop()
{  
  if (stringComplete) 
  {
//    Serial.println(inputString);
/*
 * send data to server
 */
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  
  delay(10000);
}


void sim800l_setup()
{
  pinMode(SIM800L_POWER, OUTPUT);

  digitalWrite(LED_BLUE, HIGH);
  digitalWrite(SIM800L_POWER, HIGH);
  
//  Serial.println(F("ESP32 with GSM SIM800L"));
//  Serial.println(F("Initializing....(May take more than 10 seconds)"));
  
  delay(10000);

  // Make it slow so its easy to read!
  sim800lSerial->begin(4800, SERIAL_8N1, SIM800L_TX, SIM800L_RX);
  if (!sim800l.begin(*sim800lSerial)) {
//    Serial.println(F("Couldn't find GSM SIM800L"));
    while (1);
  }
//  Serial.println(F("GSM SIM800L is OK"));

  // Set up the FONA to send a +CMTI notification
  // when an SMS is received
//  sim800lSerial->print("AT+CNMI=2,1\r\n");

//  Serial.println("GSM SIM800L Ready");
}

void serialEvent() 
{
  while (Serial.available()) 
  {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') 
    {
      stringComplete = true;
    }
  }
}
