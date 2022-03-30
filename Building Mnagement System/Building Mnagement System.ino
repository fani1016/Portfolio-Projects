/* 
  • variable declaration  - Done
  • Serial communication  - Done
  • analog / digital I/O  - Done
  • loop                  - Done
  • array                 - Done
  • data structure        - Done
  • user-defined function - Done
  • interrupts            - Done
*/


/*
• 6 door/window alarms
• 1 card-access door locks (doors only open when card swiped – indicate with digital input)               - Done
• users cannot enter the building if they are already in it, or leave if they are outside               - Done
• local alarm system (output to light)                                        - Done
• able to call security (output to another light and serial output)                           - Done
• activated and deactivated using user input code (can be turned on and off using a digital input)        - Done
• emergency button that triggers local and security alarms                              - Done
• temperature and humidity of 2 production rooms are recorded every 15 minutes
• high / low temperature (15°C and 30°C) and humidity (20%RH and 60%RH) alarm (common alarm)
• logging of all sensor and alarm activity, alarm activation / deactivation
• all logged values output serially at end of day

*/


int btn_pin[5] = {2,3,4,5,6};
int led_pin[4] = {10,11,12,13};

int a,b,c,sec,min,hr;

bool alarm_flag = false;
bool temp_alarm = false;
bool sec_flag = false;
bool emergency = false;

char temp_buffer[300];
char security_buffer[300];
char activate_buffer[300];
  
struct room{
  bool IN = false; 
  bool alarm = false;
  
  float temp;
};

void setupTimer1() {
  noInterrupts();
  // Clear registers
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  // 1 Hz (16000000/((15624+1)*1024))
  OCR1A = 15624;
  // CTC
  TCCR1B |= (1 << WGM12);
  // Prescaler 1024
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // Output Compare Match A Interrupt Enable
  TIMSK1 |= (1 << OCIE1A);
  interrupts();
}

void setup()
{
  setupTimer1();
  
  Serial.begin(9600);
  
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  
  for (int i=10; i<14; i++)
  {
    pinMode(led_pin[i], OUTPUT);
    digitalWrite(btn_pin[i], LOW);
  }
  
  for (int j=2; j<7; j++)
  {
    pinMode(btn_pin[j], INPUT);
  }
  
  attachInterrupt(digitalPinToInterrupt(btn_pin[0]), flag, RISING);
}

void loop()
{
  room room1, room2;
  
  if ((digitalRead(btn_pin[3]) == HIGH) && (room1.IN == false))
  {
    delay(100);
    room1.IN = true;
    Serial.println("User 1 Entered.");
    delay(500);   
  }
  
  if ((digitalRead(btn_pin[3]) == HIGH) && (room1.IN == true))
  {
    delay(100);
    room1.IN = false;
    Serial.println("User 1 Left.");
    delay(500);
  }
    
  if ((digitalRead(btn_pin[4]) == HIGH) && (room2.IN == false))
  {
    delay(100);
    room2.IN = true;
    Serial.println("User 2 Entered.");
    delay(500);   
  }
  
  if ((digitalRead(btn_pin[4]) == HIGH) && (room2.IN == true))
  {
    delay(100);
    room2.IN = false;
    Serial.println("User 2 Left.");
    delay(500);
  }
  
  if (digitalRead(btn_pin[1]) == HIGH)
  {
    delay(100);
    alarm_flag = !alarm_flag;
    
    if(alarm_flag == true)
    {
      a++;
      Serial.println("Alarm Set.");
      sprintf(activate_buffer[a], "Alarm Activated: %02d:%02d\r\n", min, hr);
    }
    else
      Serial.println("Alarm Unset.");
  }
  
  if (digitalRead(btn_pin[2]) == HIGH)
  {
    delay(100);
    sec_flag = !sec_flag;
    if (sec_flag == true)
    {
      b++;
      sprintf(security_buffer[b], "Security Alarm: %02d:%02d\r\n", min, hr);
    }
  }  
    
  while ((sec_flag != false) && (alarm_flag == true))
  {
    Serial.println("Security Alarm Enabled.");
    
    digitalWrite(led_pin[1], HIGH);
    delay(500);
    digitalWrite(led_pin[1], LOW);
    delay(500);    

    if (digitalRead(btn_pin[2]) == HIGH || digitalRead(btn_pin[1]) == HIGH)
    {
      delay(100);
      sec_flag = false;
      alarm_flag = false;
      Serial.println("Security Alarm Disabled.");
    }
  }
  
  
  while ((emergency != false) && (alarm_flag == true))
  {
    Serial.println("Emergency Alarm Set!!!");

    digitalWrite(led_pin[0], HIGH);
    digitalWrite(led_pin[1], HIGH);
    delay(500);
    digitalWrite(led_pin[0], LOW);
    digitalWrite(led_pin[1], LOW);
    delay(500);    

    if (digitalRead(btn_pin[1]) == HIGH)
    {
      delay(100);
      alarm_flag = false;
      emergency = false;

      Serial.println("Emergency Alarm Unset.");
    }
  }
  
  room1.temp = temp1();
  room2.temp = temp2();

  if (room1.temp > 30 || room1.temp < 15 || room2.temp > 30 || room2.temp < 15)
  {
    c++;
    temp_alarm = true;
    sprintf(temp_buffer[c], "Temp. Alarm: %02d:%02d\r\n", min, hr);
  } 
  else
    temp_alarm = false;
  
  while ((temp_alarm != false) && (alarm_flag == true))
  {
    digitalWrite(led_pin[0], HIGH);
    delay(250);
    digitalWrite(led_pin[0], LOW);
    delay(250);   
    
    room1.temp = temp1();
    room2.temp = temp2();
  }

//  if ()
}

float temp1()
{
 int sensorValue = analogRead(A0);  // read the value from the sensor 1
 float voltage = (float) sensorValue * 5.0; 
 voltage /= 1024.0; 
 return (voltage - 0.5) * 100; 
}

float temp2()
{
 int sensorValue = analogRead(A1);  // read the value from the sensor 1
 float voltage = (float) sensorValue * 5.0; 
 voltage /= 1024.0; 
 return (voltage - 0.5) * 100; 
}

void flag()
{
  if (alarm_flag == true)
  emergency = !emergency;
  Serial.println(emergency);
}

ISR(TIMER1_COMPA_vect)
{
  sec++;
  
  if (sec > 59)
  {
    min++;
    sec = 0;
  }
  
  if (min > 59)
  {
    hr++;
    min = 0;
  }
  
  if (hr > 23)
  {
    hr = 0;
    min = 0;
    sec = 0;
  }
  
}
