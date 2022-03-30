/*--------------------------------------------------------*/
/*                                                        */
/*  Author: Farhan Tariq Janjua                           */
/*  Fiverr: https://www.fiverr.com/users/a1_developers/   */
/*                                                        */
/*--------------------------------------------------------*/

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "Wire.h"
#include "SPI.h"
#include "SD.h"
#include "RTClib.h"     

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

RTC_DS3231 rtc;

Adafruit_MPU6050 mpu;
const int chipSelect = 15;  // used for ESP32

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
float txValue = 0;
String dataString = "";
String heading = "";

  class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();

        Serial.println();
        Serial.println("*********");
    }
};

void setup(void) 
{
  Serial.begin(9600);
  
    // Create the BLE Device
  BLEDevice::init("ESP32 BLE Test"); // Give it a name

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID_TX,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
                      
  pCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID_RX,
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
  
  while (!Serial)
    delay(10); // will pause esp32 until serial console opens

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  
  rtc.adjust(DateTime(__DATE__, __TIME__));
  DateTime now = rtc.now();
  
  Serial.println("Adafruit MPU6050 test!");
    if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  else  Serial.println("MPU6050 Found!");
  
  if (!SD.begin(chipSelect)) {
    Serial.println("SD Card Initialization failed!");
    while (1);
  }
  
   // If the data.txt file doesn't exist
  // Create a file on the SD card and write the data labels
  File file = SD.open("/data.txt");
  if(!file) {
    Serial.println("File doens't exist");
    Serial.println("Creating file...");
    writeFile(SD, "/data.txt", "Time, Accelerometer, Gyroscope \r\n");
  }
  else {
    Serial.println("File already exists");  
  }
  file.close();

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G); //Accelerometer range setting
  mpu.setGyroRange(MPU6050_RANGE_500_DEG); //Gyroscope range setting
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ); //MPU6050 Filter bandwidth setting
  
  Serial.println("");
  delay(100);
}

void loop() {
  DateTime now = rtc.now();

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values on Serial Monitor for Troubleshooting*/
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.println("");
  delay(500);

  dataString = String(now.unixtime());
  dataString += ",";
  dataString += "X: " + String(a.acceleration.x) + " . " + "Y: " + String(a.acceleration.y) + " . " + "Z: " + String(a.acceleration.z);
  dataString += ",";
  dataString += "X: " + String(g.gyro.x) + " . " + "Y: " + String(g.gyro.y) + " . " + "Z: " + String(g.gyro.z) + "\r\n";
  
  appendFile(SD, "/data.txt", dataString.c_str());

  if (deviceConnected) { 
    // Let's convert the value to a char array:
//    char txString[100]; // make sure this is big enough for all the data
//    dtostrf(dataString, 1, 2, txString); // float_val, min_width, digits_after_decimal, char_buffer
    
    pCharacteristic->setValue(dataString.c_str()); // Sending the data over BLE
    
    pCharacteristic->notify();
    Serial.print("*** Sent Value: ");
    Serial.print(dataString.c_str());
  }
    
  delay(2000);
}

// Write to the SD card (DON'T MODIFY THIS FUNCTION)
void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

// Append data to the SD card (DON'T MODIFY THIS FUNCTION)
void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}
