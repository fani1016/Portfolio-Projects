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
//LoRa transceiver module pins
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

//Serial pins
#define Tx 1
#define Rx 3

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

/*=======Constants Definition======= */


/*=======Variables Definition======= */
int counter = 0;  //packet counter
char buff[60];
String received_data, mod2_data, mod3_data;

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
  lora_read();
  Serial.println("Module 03 Data Received: " + mod3_data);
  Serial.println("Module 02 Data Received: " + mod2_data);
  
//  delay(10000);
}


void lora_setup() 
{  
//  Serial.println("LORA Initializing!");

  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
//    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("LORA Receiver");
  display.display();

  Serial.println("LoRa Receiver Test");

  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
//  Serial.println("LoRa Initializing OK!");
  display.setCursor(0, 10);
  display.print("LoRa Initializing OK!");
  display.display();

//  LoRa.setSyncWord(0xFF);
  delay(2000);
}

void lora_read() 
{
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) 
  {
    // received a packet
//    Serial.print("Received packet '");

    // read packet
	{
      received_data = LoRa.readString();
      if(received_data.substring(0,9) == "Module 02")
		mod2_data = received_data;
      
      if(received_data.substring(0,9) == "Module 03")
		mod3_data = received_data;
    }

    // print RSSI of packet
//    Serial.print("' with RSSI ");
//    Serial.println(LoRa.packetRssi());
  }
}
