#define RTC_Address 0x68

sbit led at ODR12_GPIOD_ODR_bit;
sbit bt_btn at GPIOB_IDR.B0;                                     // MMC module connections
sbit Mmc_Chip_Select at GPIOA_ODR.B4;                     // MMC module connections
Mmc_Set_Interface(_MMC_INTERFACE_SPI);          // Set SPI interface for SD Card

sbit buzzer at GPIOC_ODR.B3;

typedef unsigned short uint8;
typedef signed short int8;

/*-----Variables Declaration START--------------------------------------------------------------------------------------*/
int S_Num = 1;
char set_time[19];
char get_time[19];

char CH4_Send[9] = {0xFF, 0x01, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07};
char CH4_Inq_Mode[9] = {0xFF, 0x01, 0x03, 0x02, 0x00, 0x00, 0x00, 0x00, 0x05};
char CH4_Rep_Mode[9] = {0xFF, 0x01, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x04};
int CH4_Receive[10];

float CH4_res;
int CH4_Gas;

float bpm;
int beats, counter;
char beat_byt[10];

char H2S_Receive[70], CO_Receive[70];
char H2S[8], CO[8];
int H2S_Data = 0;
int CO_Data = 0;

int i, j, k;
int err, set;
char byt[10], byt2[5];

unsigned long fsize;
char ReadBuffer[70];
char receive[15];
short read;
short fhandle;
char store_buff[70];

/*-----Variables Declaration FINISH--------------------------------------------------------------------------------------*/


/*----- Functions Declaration START--------------------------------------------------------------------------------------*/

void Setup();
void Get_CH4();
void Get_H2S();
void Get_CO();
void SD_card();
void read_file();
void write_file();
void create_file();
int check_file();
void initFastSPI();
void initSPI();
void RTC_DS3231();
void RTC_Read();
void RTC_Write(int sec, int min, int hour, int dow, int dom, int month, int year);
int bcdToDec(char val);     // Convert binary coded decimal to normal decimal numbers
char decToBcd(int val);     // Convert normal decimal numbers to binary coded decimal
int Heart_Rate();
void Ext_Int_Setup();
void Timer_Init();


/*----- Functions Declaration FINISH--------------------------------------------------------------------------------------*/


void main()
{
  Setup();
  
  buzzer = 0;
  Delay_ms(10000);

  UART1_Write_Text("\r");
  UART2_Write_Text("\r");
  Delay_ms(100);


//  RTC_Write(0, 47, 17, 6, 1, 1, 22);


  for (i = 0; i < 9; i++)
  {
    UART3_Write(CH4_Rep_Mode[i]);
  }


  while(1)
  {
     if (CH4_Gas >= 25 || H2S_Data >= 8 || CO_Data >= 100) // to generate an alert through a buzzer
     {
      buzzer = 1;
    }
    else
    {
      buzzer = 0;
    }

    if (beats > 29 )                                                 // calculate heart rate when 20 seconds elapsed
    {
      Heart_Rate();
    }
//    UART6_Write_Text("01...\r\n");

    RTC_DS3231();
//   UART6_Write_Text("02...\r\n");

    Get_CH4();
//    UART6_Write_Text("03...\r\n");

    Get_CO();                // '0' for CO Gas, '1' for H2S Gas
//    UART6_Write_Text("04...\r\n");

    Get_H2S();                // '0' for CO Gas, '1' for H2S Gas
//    UART6_Write_Text("05...\r\n");

//    UART6_Write_Text(store_buff);

    SD_Card();
//    UART6_Write_Text("06...\r\n");

  }
}

/*-----HeartRate methods START--------------------------------------------------------------------------------------*/

void Timer_Init()
{
  RCC_APB1ENR.TIM2EN = 1;       // Enable clock gating for timer module 2
  TIM2_CR1.CEN = 0;             // Disable timer
  TIM2_PSC = 2000;              // Set timer prescaler for 96MHz
  TIM2_ARR = 23999;
  NVIC_IntEnable(IVT_INT_TIM2); // Enable timer interrupt
  TIM2_DIER.UIE = 1;            // Update interrupt enable
  TIM2_CR1.CEN = 1;             // Enable timer
}

void Timer2_interrupt() iv IVT_INT_TIM2
{
  TIM2_SR.UIF = 0;
  counter++;
}


void Ext_Int_Setup()
{
  SYSCFG_EXTICR2 = 0x00000002;         // Map external interrupt on PC4
  EXTI_RTSR = 0x00000000;              // Set interrupt on Rising edge (none)
  EXTI_FTSR = 0x00000010;              // Set Interrupt on Falling edge (PC4)
  EXTI_IMR |= 0x00000010;              // Set mask
  NVIC_IntEnable(IVT_INT_EXTI4);   // Enable External interrupt
}


void ExtInt() iv IVT_INT_EXTI4 ics ICS_AUTO
{
  EXTI_PR.B4 = 1;                     // clear flag
  beats++;

/*  UART6_Write_Text("Beats: ");
  IntToStr(beats, beat_byt);
  UART6_Write_Text(beat_byt);
  UART6_Write_Text("\t");

  UART6_Write_Text("Time: ");
  IntToStr(counter, beat_byt);
  UART6_Write_Text(beat_byt);
  UART6_Write_Text("\r");
*/
//  Delay_ms(20);
}


int Heart_Rate()
{
   bpm =  ( (float) beats / (float) counter ) * 60;// beats per minute

/*   UART6_Write_Text("Heart Rate: ");
   FloatToStr(bpm, beat_byt);
   UART6_Write_Text(beat_byt);
   UART6_Write_Text(" BPM\r");
*/
   beats = 0;
   counter = 0;

   return bpm;
}

/*-----HeartRate methods FINISH--------------------------------------------------------------------------------------*/



/*-----RTC DS3231 Methods START--------------------------------------------------------------------------------------*/

char decToBcd(int val)     // Convert normal decimal numbers to binary coded decimal
{
  return (char)( (val/10*16) + (val%10) );
}

int bcdToDec(char val)     // Convert binary coded decimal to normal decimal numbers
{
  return (int)( (val/16*10) + (val%16) );
}

void RTC_Write(int sec, int min, int hour, int dow, int dom, int month, int year)
{
    set_time[0] = decToBcd(0);
    set_time[1] = decToBcd(sec);
    set_time[2] = decToBcd(min);
    set_time[3] = decToBcd(hour);
    set_time[4] = decToBcd(dow);
    set_time[5] = decToBcd(dom);
    set_time[6] = decToBcd(month);
    set_time[7] = decToBcd(year);

    I2C1_Start();
    I2C1_Write(RTC_Address, set_time, 19, END_MODE_STOP);
    Delay_ms(10);

//    UART6_Write_Text("Time Is Set.\r\n");
}

void RTC_Read()
{
    I2C1_Start();
    I2C1_Read(RTC_Address, get_time, 19, END_MODE_STOP);
    Delay_ms(10);
}

void RTC_DS3231()
{
  RTC_Read();
}

/*-----RTC DS3231 Methods FINISH--------------------------------------------------------------------------------------*/


/*-----SD Card Methods START--------------------------------------------------------------------------------------*/


//--- initialize SPI3
void initSPI()
{
  SPI1_Init_Advanced(_SPI_FPCLK_DIV64, _SPI_MASTER | _SPI_8_BIT |
                   _SPI_CLK_IDLE_LOW | _SPI_FIRST_CLK_EDGE_TRANSITION |
                   _SPI_MSB_FIRST | _SPI_SS_DISABLE | _SPI_SSM_ENABLE | _SPI_SSI_1,
                   &_GPIO_MODULE_SPI1_PA567);
}

//--- initialize Fast SPI3
void initFastSPI()
{
  SPI1_Init_Advanced(_SPI_FPCLK_DIV2, _SPI_MASTER | _SPI_8_BIT |
                   _SPI_CLK_IDLE_LOW | _SPI_FIRST_CLK_EDGE_TRANSITION |
                   _SPI_MSB_FIRST | _SPI_SS_DISABLE | _SPI_SSM_ENABLE | _SPI_SSI_1,
                   &_GPIO_MODULE_SPI1_PA567);
}

int check_file()
{
  err = FAT32_Exists("Smart_Helmet_01.TXT");
/*IntToStr(err,receive);
  UART6_Write_Text("Found: ");
  UART6_Write_Text(receive);
  UART6_Write_Text("\r\n");*/

  return err;
}

void create_file()
{
//  UART6_Write_Text("File not FOUND, Creating the File... \r\n");

  fhandle = FAT32_Open("Smart_Helmet_01.TXT", 0x04);         //.....Creating the file.....//
/*IntToStr(fhandle,receive);
  UART6_Write_Text("File: ");
  UART6_Write_Text(receive);
  UART6_Write_Text("\r\n");*/

//  err = FAT32_Close(fhandle);
}

void write_file()
{
  if (check_file() == 0)
  {
     UART6_Write_Text("File not Present... \r\n");
     Delay_ms(250);
     create_file();
  }

  else if (check_file() == 1)                 //...If file not FOUND...
  {
     create_file();

//     UART6_Write_Text("File Present, Writing into the File... \r\n");
//     Delay_ms(500);

    sprintf (store_buff, "%03d, %02d-%02d-20%02d, %02d:%02d, CO:%03d, H2S:%03d, CH4:%03d, HR:%03d\r\n", S_Num, bcdToDec(get_time[6]),
    bcdToDec(get_time[5]), bcdToDec(get_time[7]), bcdToDec(get_time[3]), bcdToDec(get_time[2]), Co_Data, H2S_Data, CH4_Gas, bpm);

    err = FAT32_Write(fhandle, store_buff, 58); //.....Writing text to the file.....//

//     UART6_Write_Text("File Written... \r\n");

     Delay_ms(10);

     err = FAT32_Close(fhandle);             //.....Closing the file.....//
  }

  else
  {
     UART6_Write_Text("File Error.\r\n");
     Delay_ms(500);
  }
}

void read_file()
{
  if (check_file() == 0)                 //...If file not FOUND...
  {
     UART6_Write_Text("File not Available.\r\n");
     create_file();
  }

  else if (check_file() == 1)                 //...If file FOUND...
  {
//     UART6_Write_Text("File Present, Opening the File.\r\n");
     fhandle = FAT32_Open("Smart_Helmet_01.TXT", 0x01);      //.....Opening the file.....//
     Delay_ms(10);

     err = FAT32_Size("Smart_Helmet_01.TXT", &fsize);              //.....Checking size of the file.....//

//     UART6_Write_Text("Size Obtained...\r\n");

/*LongToStr(fsize,receive);
     UART6_Write_Text("File Size: ");
     UART6_Write_Text(receive);
     UART6_Write_Text("\r\n");
     Delay_ms(250);*/

    while (bt_btn != 0)
    {
//        UART6_Write_Text("Reading the File... \r\n");

        for (i = 0; i <= fsize; i=i+58)
        {
            FAT32_Read(fhandle, ReadBuffer, 58);
            for (j = 0; j < 58; j++)
            {
                if (j <= i)
                {
                  UART6_Write(ReadBuffer[j]);
                }

                UART4_Write_Text(ReadBuffer);
            }

            if(bt_btn == 0)
              break;
         }

    }

    err = FAT32_Close(fhandle);             //.....Closing the file.....//
//    UART6_Write_Text("File Closed... \r\n");

  }

  else
  {
     UART6_Write_Text("File Error.\r\n");
  }

}

void SD_card()
{
    err = FAT32_Dev_Init();
    err = FAT32_Init();

    initFastSPI();

    if (err == 0)
    {
       if (bt_btn == 0)
       {
          write_file();
       }
       else
       {
           read_file();
       }
    }

    else         // ....Initiazation Failed....
    {
       UART6_Write_Text("SD Card Initialization Failed. \r\n");
    }

//    Delay_ms(100);
}

/*-----SD Card Methods FINISH--------------------------------------------------------------------------------------*/


/*-----H2S+CO Sensor Method START--------------------------------------------------------------------------------------*/

void Get_CO()             
{
    UART1_Write_Text("\r");
    
    UART1_Read_Text(CO_Receive, "\r", 70);
    UART6_Write_Text(CO_Receive);
    UART6_Write_Text("\r");

    for ( i = 16; i < 22; i++)
    {
      if (CO_Receive[i] == ',')
      {
          strncpy(CO, CO_Receive + 14, i - 14);
          CO_Data = (atoi(CO) / 1000);  // + 0.5;

          if (CO_Data < 0)
             CO_Data = 0;

          for ( i = 0; i < 8; i++)
          {
             CO[i] = 0;
          }
          
          break;
      }
    }

//    sprintf (byt, "CO Gas: %03d ppm\r\n", CO_Data);
//    UART6_Write_Text(byt);
}

void Get_H2S()
{
    UART2_Write_Text("\r");

    UART2_Read_Text(H2S_Receive, "\r", 70);
    UART6_Write_Text(H2S_Receive);
    UART6_Write_Text("\r");

    for ( i = 16; i < 22; i++)
    {
      if (H2S_Receive[i] == ',')
      {
          strncpy(H2S, H2S_Receive + 14, i - 14);
          H2S_Data = (atoi(H2S) / 1000);  // + 0.5;

          if (H2S_Data < 0)
             H2S_Data = 0;

          for ( i = 0; i < 8; i++)
          {
             H2S[i] = 0;
          }
          
          break;
      }
    }

//    sprintf (byt, "H2S Gas: %03d ppm\r\n", H2S_Data);
//    UART6_Write_Text(byt);
}

/*-----H2S+CO Sensor Method FINISH--------------------------------------------------------------------------------------*/


/*-----CH4 Sensor Methods START--------------------------------------------------------------------------------------*/

void Get_CH4()
{
    if(UART3_Read() == 0xFF)
    {
      for (i = 0; i < 9; i++)
      {
         CH4_Receive[i] = UART3_Read();
      }
    }

    if(CH4_Receive[3] == 0x00)
      CH4_res = 1;
    else if(CH4_Receive[3] == 0x01)
      CH4_res = 0.1;
    else if(CH4_Receive[3] == 0x02)
      CH4_res = 0.01;
    else
      CH4_res = 0.001;

    for (i = 0; i < 10; i++)
    {
       IntToStr(CH4_Receive[i], byt);
       UART6_Write_Text(byt);
       UART6_Write_Text(" ");
    }
    UART6_Write_Text("\r");

    CH4_Gas = ((float)((CH4_Receive[4] * 256) + CH4_Receive[5]) * CH4_res);

//    sprintf (byt, "Gas Concentration: %03d */*\r", CH4_Gas);
//    UART6_Write_Text(byt);

}

/*-----CH4 Sensor Methods FINISH--------------------------------------------------------------------------------------*/


/*-----Setup Method START--------------------------------------------------------------------------------------*/

void Setup()
{
  GPIO_Digital_Output(&GPIOA_BASE, _GPIO_PINMASK_4);                //For CS Pin - SD Card
  GPIO_Digital_Input(&GPIOB_BASE, _GPIO_PINMASK_0);                 //For Bluetooth Switch
  GPIO_Digital_Input(&GPIOC_BASE, _GPIO_PINMASK_4);                //For Exteral Interrupt - Heart Rate
  GPIO_Digital_Output(&GPIOC_BASE, _GPIO_PINMASK_3);                //For Buzzer
  GPIO_Digital_Output(&GPIOD_BASE, _GPIO_PINMASK_12);

  UART1_Init(9600); 
  UART2_Init(9600);                //For H2S Sensor                - PA2:Tx and PA3:Rx
  UART3_Init(9600);                //For CH4 Sensor                - PB10:Tx and PB11:Rx
//  UART4_Init(115200);                //For Bluetooth                 - PA0:Tx and PA1:Rx
  UART4_Init_Advanced(115200, _UART_8_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_UART4_PA01);
  UART5_Init(9600);                //For XBee                      - PC12:Tx and PD2:Rx
  UART6_Init(9600);                //For Serial Debug              - PC6:Tx and PC7:Rx
  Delay_ms(100);

  I2C1_Init_Advanced(100000, &_GPIO_MODULE_I2C1_PB67);                //For RTC - PB6:SCL and PB7:SDA
  Delay_ms(100);

  initSPI();                //For SD Card
  Delay_ms(100);

  Ext_Int_Setup();
  Timer_Init();
}

/*-----Setup Method FINISH--------------------------------------------------------------------------------------*/