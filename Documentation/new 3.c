#include"I2C.h"

#define RTC_Address 0x68
#define Start_Reg 0x00

uint8_t temp;
uint8_t *data_1, uint8_t size, uint8_t Address;

int set[7];
int get[7];
int buffer[30];
int sec, min, hr, dow, dom, mon, yr;

typedef struct {
    int seconds;
    int minutes;
    int hour;
    int dayofweek;
    int dayofmonth;
    int month;
    int year;
} TIME;

TIME time;

int decToBcd(int val)     // Convert normal decimal numbers to binary coded decimal
{
  return (int)( (val/10*16) + (val%10) );
}

int bcdToDec(int val)     // Convert binary coded decimal to normal decimal numbers
{
  return (int)( (val/16*10) + (val%16) );
}


/*void rtc_write(int Address, int Reg, int Data)
{
  I2C_Start ();
  I2C_Address (Address);
  I2C_Write (Reg);
    Delay_ms(5);
  I2C_Write (Data);
    Delay_ms(5);
  I2C_Stop ();
}*/

/*void rtc_read(uint8_t Address, uint8_t Reg, uint8_t *buffer, uint8_t size)
{
  I2C_Start ();
  I2C_Address (Address);
  I2C_Write (Reg);
  Delay_ms(5);
  I2C_Start ();  // repeated start
  I2C_Read (Address+0x01, buffer, 7);
  I2C_Stop ();
}*/

void Get_Time (void)
{
//    rtc_read(RTC_Address, Start_Reg, get, 7);
    I2C1_Read(RTC_Address, get, 7, END_MODE_RESTART);
    Delay_ms(5);
    time.seconds = bcdToDec(get[0]);
    time.minutes = bcdToDec(get[1]);
    time.hour = bcdToDec(get[2]);
    time.dayofweek = bcdToDec(get[3]);
    time.dayofmonth = bcdToDec(get[4]);
    time.month = bcdToDec(get[5]);
    time.year = bcdToDec(get[6]);
}

void main()
{
  GPIO_Digital_Output(&GPIOD_BASE, _GPIO_PINMASK_13);
  GPIO_Config(&GPIOB_BASE,_GPIO_CFG_AF4_I2C1, _GPIO_CFG_MODE_ALT_FUNCTION);

//  GPIO_Alternate_Function_Enable(@_GPIO_MODULE_I2C1_PB69)

/*        // If bus is busy wait SDA high before initializing I2C module
        GPIO_Digital_Output(&GPIOB_BASE, _GPIO_PINMASK_7);
        GPIO_Digital_Input(&GPIOB_BASE, _GPIO_PINMASK_6);
        GPIOB_ODR.B7 = 1;
        while (GPIOB_IDR.B6 == 0) {
                GPIOB_ODR.B7 = 0;
                Delay_us(10);
                GPIOB_ODR.B7 = 1;
                Delay_us(10);
        }*/

     I2C1_Init_Advanced( 400000, &_GPIO_MODULE_I2C1_PB67);
//    Delay_ms(200);

    UART2_Init(9600);    //For Debugging       PA2/PA3
    Delay_ms(200);

    while(1)
    {
      GPIOD_ODR.B13 = ~GPIOD_ODR.B13;
      Delay_ms(500);
      GPIOD_ODR.B13 = ~GPIOD_ODR.B13;
      Delay_ms(500);
      GPIOD_ODR.B13 = ~GPIOD_ODR.B13;
      Delay_ms(500);
      GPIOD_ODR.B13 = ~GPIOD_ODR.B13;
      Delay_ms(500);
      GPIOD_ODR.B13 = ~GPIOD_ODR.B13;
      Delay_ms(500);
      GPIOD_ODR.B13 = ~GPIOD_ODR.B13;
      Delay_ms(500);

      Get_Time();

      sprintf (buffer, "%02d:%02d:%02d", time.hour, time.minutes, time.seconds);
      UART2_Write_Text(buffer);
      UART2_Write_Text("\t");

      sprintf (buffer, "%02d-%02d-20%02d", time.dayofmonth, time.month, time.year);
      UART2_Write_Text(buffer);
      UART2_Write_Text("\r");
    }
}


void I2C_Config ()
{
  //configure the AF
  RCC_AHB1ENR |= 1<<1;
  RCC_APB1ENR |= 1 <<21;
  GPIOB_MODER |= (2<<12) | (2<<14);  // Bits (17:16)= 1:0 -_ Alternate Function for Pin PB8; Bits (19:18)= 1:0 -_ Alternate Function for Pin PB9
  GPIOB_MODER &= ~(3 << 12);
  GPIOB_MODER &= ~(3 << 14);

//  GPIOD_ODR |= 1<<4;
  GPIOB_AFRL |= (4<< 24);
  GPIOB_AFRH |= (4<< 28);

  GPIOB_OTYPER |= 1<<6;
  GPIOB_OTYPER |= 1<<7;
  GPIOB_PUPDR &= ~(3 << 12);//
  GPIOB_PUPDR &= ~(3 << 14);
  //Configure the I2C
  I2C1_CR2 = 0x0010;
  I2C1_CCR = 0x0050;
  I2C1_TRISE = 0x0011;
  I2C1_CR1 = 0x0001;
}

void I2C_Start (void)
{
  I2C1_CR1 |= (1<<10);  // Enable the ACK
  I2C1_CR1 |= (1<<8);  // Generate START
  while (!(I2C1_SR1 & (1<<0)));  // Wait fror SB bit to set
}


void I2C_Write (uint8_t data_1)
{
  while (!(I2C1_SR1 & (1<<7)));  // wait for TXE bit to set
  I2C1_DR = data_1;
  while (!(I2C1_SR1 & (1<<2)));  // wait for BTF bit to set
}


void I2C_Address (Address)
{
  I2C1_DR = Address;  //  send the address
  while (!(I2C1_SR1 & (1<<1)));  // wait for ADDR bit to set
  temp = I2C1_SR1 | I2C1_SR2;  // read SR1 and SR2 to clear the ADDR bit
}

void I2C_Stop (void)
{
   I2C1_CR1 |= (1<<9);  // Stop I2C


void I2C_WriteMulti (*data_1, size)
{
    while (!(I2C1_SR1 & (1<<7)));  // wait for TXE bit to set
    while (size)
    {
      while (!(I2C1_SR1 & (1<<7)));  // wait for TXE bit to set
      I2C1_DR = (uint32_t )*data_1++;  // send data
      size--;
    }

    while (!(I2C1_SR1 & (1<<2)));  // wait for BTF to set
}

void I2C_Read (uint8_t Address, uint8_t *buffer, uint8_t size)
{
    unsigned int remaining = size;

    if (size == 1)
    {
        /**** STEP 1-a ****/
        I2C1_DR = Address;  //  send the address
        while (!(I2C1_SR1 & (1<<1)));  // wait for ADDR bit to set

        /**** STEP 1-b ****/
        I2C1_CR1 &= ~(1<<10);  // clear the ACK bit
        temp = I2C1_SR1 | I2C1_SR2;  // read SR1 and SR2 to clear the ADDR bit.... EV6 condition
        I2C1_CR1 |= (1<<9);  // Stop I2C

        /**** STEP 1-c ****/
        while (!(I2C1_SR1 & (1<<6)));  // wait for RxNE to set

        /**** STEP 1-d ****/
        buffer[size-remaining] = I2C1_DR;  // Read the data from the DATA REGISTER
    }

    else
    {
        /**** STEP 2-a ****/
        I2C1_DR = Address;  //  send the address
        while (!(I2C1_SR1 & (1<<1)));  // wait for ADDR bit to set

        /**** STEP 2-b ****/
        temp = I2C1_SR1 | I2C1_SR2;  // read SR1 and SR2 to clear the ADDR bit

        while (remaining>2)
        {
                /**** STEP 2-c ****/
                while (!(I2C1_SR1 & (1<<6)));  // wait for RxNE to set

                /**** STEP 2-d ****/
                buffer[size-remaining] = I2C1_DR;  // copy the data into the buffer

                /**** STEP 2-e ****/
                I2C1_CR1 |= 1<<10;  // Set the ACK bit to Acknowledge the data received

                remaining--;
        }

        // Read the SECOND LAST BYTE
        while (!(I2C1_SR1 & (1<<6)));  // wait for RxNE to set
        buffer[size-remaining] = I2C1_DR;

        /**** STEP 2-f ****/
        I2C1_CR1 &= ~(1<<10);  // clear the ACK bit

        /**** STEP 2-g ****/
        I2C1_CR1 |= (1<<9);  // Stop I2C

        remaining--;

        // Read the Last BYTE
        while (!(I2C1_SR1 & (1<<6)));  // wait for RxNE to set
        buffer[size-remaining] = I2C1_DR;  // copy the data into the buffer
    }

}
