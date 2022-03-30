#include <OLED_I2C.h>
#include <Wire.h>

OLED  myOLED(SDA, SCL); // Remember to add the RESET pin if your display requires it...

extern uint8_t BigNumbers[];

const int flexPin0 = A0;  
const int flexPin1 = A1;  
const int flexPin2 = A2;     
const int button = A3; 

const float VCC = 5;     

const float R_DIV = 47000.0;  // resistor used to create a voltage divider
const float flatResistance = 25000.0; // resistance when flat
const float bendResistance = 100000.0;  // resistance at 90 deg

int prevLoop = 0; 
int loop = 0;
int lastResist = 0;

void setup() 
{
  Serial.begin(9600); 
  myOLED.setFont(BigNumbers);

  pinMode(flexPin0, INPUT);   
  pinMode(flexPin1, INPUT);
  pinMode(flexPin2, INPUT); 
  pinMode(button, INPUT_PULLUP); 
}

void loop() 
{ 
  int btnVal = digitalRead(button); 
  
  if (btnVal == LOW) 
  {    
    Serial.println("Reset Value");
    avgResist = 0;
    lastResist = 0;
    delay(250);
  }
  
  int ADCflex0 = analogRead(flexPin0);
  int ADCflex1 = analogRead(flexPin1);
  int ADCflex2 = analogRead(flexPin2);

  // ---------Resistance of Flex 0---------
  float Vflex0 = ADCflex0 * VCC / 1023.0;
  float Rflex0 = R_DIV * (VCC / Vflex0 - 1.0);
  Serial.println("Resistance of Flex 0: " + String(Rflex0) + " ohms");
  
  // ---------Resistance of Flex 1---------
  float Vflex1 = ADCflex1 * VCC / 1023.0;
  float Rflex1 = R_DIV * (VCC / Vflex1 - 1.0);
  Serial.println("Resistance of Flex 1: " + String(Rflex1) + " ohms");
  
  // ---------Resistance of Flex 2---------
  float Vflex2 = ADCflex2 * VCC / 1023.0;
  float Rflex2 = R_DIV * (VCC / Vflex2 - 1.0);
  Serial.println("Resistance of Flex 2: " + String(Rflex2) + " ohms");

   
  // ---------Average Resistance of all Flex Sensors---------
  avgResist = (Rflex0 + Rflex1 + Rflex2)/3;
 
  loop++;

  if (avgResist > lastResist && loop > prevLoop)
  {
    myOLED.printNumI(avgResist, CENTER, 25);
    lastResist = avgResist;
  }  

  else
    myOLED.printNumI(lastResist, CENTER, 25);

  prevLoop = loop;
  myOLED.update();
  
  delay(500);
}
