/* Header Files*/

#include "DHT11_lib.c"
#include "Gas_Sensor_lib.c"


/* Global variables declaration and definition*/
char Gas_buffer[80];
char uart_rd;

unsigned int AQI = 0;
unsigned int state1=0;
unsigned int state2=0;
unsigned int flag = 0;
unsigned int adc1,adc2=0;


/* Functions declaration*/
void initialize_uart();
void initialize_adc();
unsigned int get_adc(unsigned int channel);


/* Main Function*/
void main()
{
  initialize_uart();
  initialize_adc();

  delay_ms(15000);

  UART2_Write_Text("Calibrating...\n\r");
  calibration();
  UART2_Write_Text("Calibration is done...\n\r");
  UART2_Write_Text("\n\r");

  while(1)
  {
    Read_DHT11();
    Read_Gas_Sensors();

    adc1=get_adc(11);
    adc2=get_adc(12);

    if(adc1>=600)       state1=1;
    else                state1=0;

    if(adc2>=600)       state2=1;
    else                state2=0;

    if (flag == 1)
    {
       switch (uart_rd)
       {
         case 'a':
         {
//            UART2_Write_Text("Got a\r");          //got=="0000"
            state1 = 0;
            state2 = 0;
            break;
         }

         case 'b':
         {
//            UART2_Write_Text("Got b\r");          //got=="0100"
            state1 = 0;
            state2 = 1;
            break;
         }

         case 'c':
         {
//            UART2_Write_Text("Got c\r");          //got=="1000"
            state1 = 1;
            state2 = 0;
            break;
         }

         case 'd':
         {
//            UART2_Write_Text("Got d\r");          //got=="1100"
            state1 = 1;
            state2 = 1;
            break;
         }

         default:
            break;
       }

       PORTBbits.RB14 = state1;
       PORTBbits.RB15 = state2;

       flag = 0;
    }

    AQI = map(CO_Conc, 0, 999, 0, 9999);

    sprintf (Gas_buffer, "%d%d00%02d%03d%04d%05d%05d%05d%05d%04d", state1, state2, Temp, RH,  (int) CO_Conc, (int) CO2_Conc, (int) CH4_Conc, (int) NH3_Conc, (int) LPG_Conc, AQI);
    UART1_Write_Text(Gas_buffer);
   }
}


/*UART Interrupt Function Definition*/
void UART1_Interrupt() iv IVT_ADDR_U1RXINTERRUPT
{
  IFS0bits.U1RXIF = 0;

  if(U1STAbits.URXDA = 1)
  {
    uart_rd = U1RXREG;
    flag = 1;
  }
}


/*UART Initialization Function Definition*/
void initialize_uart()
{
  Unlock_IOLOCK();
  PPS_Mapping_NoLock(RB7, _INPUT, _U1RX);   // Sets pin 2 to be Input, and maps as UART RX DATAIN Input to it
  PPS_Mapping_NoLock(RB8, _OUTPUT, _U1TX);   // Sets pin 3 to be Output, and maps as UART TX DATAOUT Output to it

  PPS_Mapping_NoLock(RB6, _INPUT, _U2RX);   // Sets pin 2 to be Input, and maps as UART RX DATAIN Input to it
  PPS_Mapping_NoLock(RB5, _OUTPUT, _U2TX);   // Sets pin 3 to be Output, and maps as UART TX DATAOUT Output to it
  Lock_IOLOCK();

  IEC0bits.U1RXIE = 1;    // Enable UART RX Interrupt

  UART1_Init(9600);             // Initialize UART module at 9600 bps
  UART2_Init(9600);               // Initialize UART module at 9600 bps
  Delay_ms(100);                  // Wait for UART module to stabilize

  UART2_Write_Text("Start.\r");
}


/*ADC Initialization Function Definition*/
void initialize_adc()
{
  ADC1_Init_Advanced(_ADC_10bit, _ADC_INTERNAL_REF);

  AD1PCFGL = 0b1110011111101101;        //assign  channel 1,2,9,10,11 and 12 to analog, all other channels to digital

  TRISA0_bit = 1;                //for DHT11 Input
  TRISA1_bit = 1;                //AN1, for MQ2 Input
  TRISB2_bit = 1;                //AN4, for MQ135 Input
  TRISB13_bit = 1;                //AN11, for adc1 Input
  TRISB12_bit = 1;                //AN12, for adc2 Input
  TRISB14_bit = 0;                //Relay2 as Output
  TRISB15_bit = 0;                //Relay1 as Output
}


/*ADC get_samples Function Definition*/
unsigned int get_adc(unsigned int channel)
{
    unsigned int x;
    unsigned int adc=0;

    for(x=0;x<25;x++)
    {
        adc = adc+ ADC1_Get_Sample(channel);
        delay_ms(5);
    }
    adc=adc/25;
    return adc;
}

/*End of Code*/