/*=======Including Libraries======= */
//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

//Library for Magnetometer
#include <Adafruit_LIS3MDL.h>
#include <Adafruit_Sensor.h>

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/*=======Including Libraries======= */


/*=======Pins Definition======= */
//Voltage Sensor pin
#define voltPin 37

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

//Magnetometer pins
#define LIS3MDL_SCL 22
#define LIS3MDL_SDA 21
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

/*=======Variables Definition======= */


/*=======Functions Definition======= */
void lora_setup();
void lora_write();
void mag_setup();
void mag_read();

/*=======Functions Definition======= */


/*=======Objects Definition======= */
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);
Adafruit_LIS3MDL lis3mdl;
/*=======Objects Definition======= */


void setup() 
{
  Serial.begin(9600);
  lora_setup(); 
  mag_setup();
}


void loop()
{
  analogvalue = analogRead(voltPin);
  Vout = (analogvalue * Vref / 1024);
  Vin = Vout / (R2/(R1+R2));
  
  mag_read();
  lora_write();
  
  delay(6000);
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
  sprintf(buff, "Module: 02, Battery: %03d, X: %02d, Y: %02d, Z: %02d", Vin, magX, magY, magZ);
  LoRa.print(buff);
  LoRa.endPacket();

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("LORA Magnetometer");
  display.setCursor(0, 20);
  display.print("X: ");
  display.setCursor(20, 20);
  display.print(magX);
  display.setCursor(0, 40);
  display.print("Y: ");
  display.setCursor(20, 40);
  display.print(magY);
  display.setCursor(0, 60);
  display.print("X: ");
  display.setCursor(20, 60);
  display.print(magZ);
  display.setCursor(0, 80);
  display.print("Battery: ");
  display.setCursor(70, 20);
  display.print(Vin);
  display.display();
}


void mag_setup()
{
  Serial.println("Adafruit LIS3MDL test!");

  // Try to initialize!
  if (! lis3mdl.begin_I2C(0x1C)) {          // hardware I2C mode, can pass in address & alt Wire
    //if (! lis3mdl.begin_SPI(LIS3MDL_CS)) {  // hardware SPI mode
    //if (! lis3mdl.begin_SPI(LIS3MDL_CS, LIS3MDL_CLK, LIS3MDL_MISO, LIS3MDL_MOSI)) { // soft SPI
    Serial.println("Failed to find LIS3MDL chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("LIS3MDL Found!");

  lis3mdl.setPerformanceMode(LIS3MDL_MEDIUMMODE);
  Serial.print("Performance mode set to: ");
  switch (lis3mdl.getPerformanceMode()) {
    case LIS3MDL_LOWPOWERMODE: Serial.println("Low"); break;
    case LIS3MDL_MEDIUMMODE: Serial.println("Medium"); break;
    case LIS3MDL_HIGHMODE: Serial.println("High"); break;
    case LIS3MDL_ULTRAHIGHMODE: Serial.println("Ultra-High"); break;
  }

  lis3mdl.setOperationMode(LIS3MDL_CONTINUOUSMODE);
  Serial.print("Operation mode set to: ");
  // Single shot mode will complete conversion and go into power down
  switch (lis3mdl.getOperationMode()) {
    case LIS3MDL_CONTINUOUSMODE: Serial.println("Continuous"); break;
    case LIS3MDL_SINGLEMODE: Serial.println("Single mode"); break;
    case LIS3MDL_POWERDOWNMODE: Serial.println("Power-down"); break;
  }

  lis3mdl.setDataRate(LIS3MDL_DATARATE_155_HZ);
  // You can check the datarate by looking at the frequency of the DRDY pin
  Serial.print("Data rate set to: ");
  switch (lis3mdl.getDataRate()) {
    case LIS3MDL_DATARATE_0_625_HZ: Serial.println("0.625 Hz"); break;
    case LIS3MDL_DATARATE_1_25_HZ: Serial.println("1.25 Hz"); break;
    case LIS3MDL_DATARATE_2_5_HZ: Serial.println("2.5 Hz"); break;
    case LIS3MDL_DATARATE_5_HZ: Serial.println("5 Hz"); break;
    case LIS3MDL_DATARATE_10_HZ: Serial.println("10 Hz"); break;
    case LIS3MDL_DATARATE_20_HZ: Serial.println("20 Hz"); break;
    case LIS3MDL_DATARATE_40_HZ: Serial.println("40 Hz"); break;
    case LIS3MDL_DATARATE_80_HZ: Serial.println("80 Hz"); break;
    case LIS3MDL_DATARATE_155_HZ: Serial.println("155 Hz"); break;
    case LIS3MDL_DATARATE_300_HZ: Serial.println("300 Hz"); break;
    case LIS3MDL_DATARATE_560_HZ: Serial.println("560 Hz"); break;
    case LIS3MDL_DATARATE_1000_HZ: Serial.println("1000 Hz"); break;
  }

  lis3mdl.setRange(LIS3MDL_RANGE_4_GAUSS);
  Serial.print("Range set to: ");
  switch (lis3mdl.getRange()) {
    case LIS3MDL_RANGE_4_GAUSS: Serial.println("+-4 gauss"); break;
    case LIS3MDL_RANGE_8_GAUSS: Serial.println("+-8 gauss"); break;
    case LIS3MDL_RANGE_12_GAUSS: Serial.println("+-12 gauss"); break;
    case LIS3MDL_RANGE_16_GAUSS: Serial.println("+-16 gauss"); break;
  }

  lis3mdl.setIntThreshold(500);
  lis3mdl.configInterrupt(false, false, true, // enable z axis
                          true, // polarity
                          false, // don't latch
                          true); // enabled!
}

void mag_read()
{
  lis3mdl.read();      // get X Y and Z data at once
  // Then print out the raw data
  Serial.print("\nX:  "); Serial.print(lis3mdl.x);
  Serial.print("  \tY:  "); Serial.print(lis3mdl.y);
  Serial.print("  \tZ:  "); Serial.println(lis3mdl.z);

  /* Or....get a new sensor event, normalized to uTesla */
  sensors_event_t event;
  lis3mdl.getEvent(&event);
  /* Display the results (magnetic field is measured in uTesla) */
  Serial.print("\tX: "); Serial.print(event.magnetic.x);
  Serial.print(" \tY: "); Serial.print(event.magnetic.y);
  Serial.print(" \tZ: "); Serial.print(event.magnetic.z);
  Serial.println(" uTesla ");

  magX = event.magnetic.x;
  magY = event.magnetic.y;
  magZ = event.magnetic.z;

  delay(100);
  Serial.println();
}
