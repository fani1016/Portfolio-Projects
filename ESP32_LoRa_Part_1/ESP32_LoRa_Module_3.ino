/*=======Including Libraries======= */
//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/*=======Including Libraries======= */


/*=======Pins Definition======= */
//Voltage Sensor pin
#define voltPin 37

//LM393 Voltage Comparator Module pin
#define LM393_An_Pin 36
#define LM393_Dig_Pin 38

//LoRa transceiver module pins
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

//OLED pins
#define OLED_SDA 4
#define OLED_SCL 15
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
/*=======Pins Definition======= */


/*=======Constants Definition======= */
//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 866E6

#define R1 30000
#define R2 7500

#define Vref 3.3

/*=======Constants Definition======= */


/*=======Variables Definition======= */
int counter = 0;  //packet counter
char buff[60];
int magX, magY, magZ;
int analogvalue;
float Vin, Vout;
float displacement;

/*=======Variables Definition======= */


/*=======Functions Definition======= */
void lora_setup();
void lora_write();

/*=======Functions Definition======= */


/*=======Objects Definition======= */
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

/*=======Objects Definition======= */


void setup() 
{
  Serial.begin(9600);
  lora_setup(); 
}


void loop()
{
  Vout = (analogRead(voltPin) * Vref / 1024);
  Vin = Vout / (R2/(R1+R2));
  
  displacement = (analogRead(LM393_An_Pin) * Vref / 1024);
  
  lora_write();
  
  delay(12000);
}


void lora_setup() 
{  
  Serial.println("LORA Initializing!");

  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("LORA SENDER ");
  display.display();

  Serial.println("LoRa Sender Test");

  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initializing OK!");
  display.setCursor(0, 10);
  display.print("LoRa Initializing OK!");
  display.display();
  
//  LoRa.setSyncWord(0xFF);
  delay(2000);
}

void lora_write()
{  
  Serial.print("Sending packet: ");

  //Send LoRa packet to receiver
  LoRa.beginPacket();
  sprintf(buff, "Module: 03, Battery: %03d, Displacement: %04d", Vin, displacement);
  LoRa.print(buff);
  LoRa.endPacket();

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("LORA Position");
  display.setCursor(20, 20);
  display.println("Sensor");
  display.setCursor(0, 40);
  display.print("Displacement: ");
  display.setCursor(80, 40);
  display.print(displacement);
  display.print("Battery: ");
  display.setCursor(70, 60);
  display.print(Vin);
  display.display();
}
