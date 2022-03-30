/*--------------------------------------------------------*/
/*                                                        */
/*  Author: Farhan Tariq Janjua                           */
/*  Fiverr: https://www.fiverr.com/users/a1_developers/   */
/*                                                        */
/*--------------------------------------------------------*/


#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
SoftwareSerial mySerial(10, 9); // RX, TX

int set = 0;

void setup() 
{
  Serial.begin(9600);
  mySerial.begin(38400);
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Started.");
  mySerial.print("at+init\r\n");
  mySerial.print("at+iac=9e8b33\r\n");
  mySerial.print("at+class=0\r\n ");
  mySerial.print("at+inqm=1,1,48\r\n");
  Serial.println(mySerial.readString());
  delay(500);
}

void loop() 
{
  lcd.clear();
  lcd.print("Scanning...");
  Serial.println("Scanning...");
  if (set == 1)
  {
    lcd.setCursor(0,1);
    lcd.print("DEVICE FOUND.");
  }

  mySerial.print("at+inq\r\n");
  String devices = mySerial.readString();
  Serial.println(devices);

  if(devices.indexOf("C4BF:60:633BD0") > 0) // Enter here the Bluetooth address of your desired device
                                            // (your phone etc.) which would be in this format "C4:AF:90:16:3B:F0"
  {
    Serial.println("DEVICE FOUND!!!!");
    lcd.setCursor(0,1);
    lcd.print("DEVICE FOUND.");
    set = 1;
   }
     
  delay(5000);
}
