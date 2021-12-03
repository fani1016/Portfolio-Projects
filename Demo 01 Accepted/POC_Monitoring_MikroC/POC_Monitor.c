 char uart_rd;
char text[1],text1[1];
int state1=0;
int state2=0;
int i, flag;
unsigned int adc1,adc2=0;

void initialize_uart();
unsigned int get_adc(unsigned int channel);
void initialize_adc();
void TrimLeft(char *str);

void main()
{
//  AD1PCFGL = 0xffff;                            // all A/D ports digital
  AD1PCFGL        = 0b1110011111111111;        //assign  pin 11 and 12 to analog and all other pins to digital

  initialize_uart();
  initialize_adc();

  TRISB12_bit = 1;		// Pin 12 as Input
  TRISB13_bit = 1;		// Pin 13 as Input
  TRISB14_bit=0;		// Pin 14 as Output
  TRISB15_bit=0;		// Pin 15 as Output

  while (1)
  {
//    delay_ms(1000);
    
    adc1=get_adc(11);
    delay_ms(100);
    adc2=get_adc(12);
    delay_ms(100);

    if(adc1>=600)
    {
//      delay_ms(1000);
      state1=1;
    }
      
    else
      state1=0;

    if(adc2>=600)
    {
//      delay_ms(1000);
      state2=1;
    }

    else
      state2=0;
            
    if (flag == 1)
    {
       if (uart_rd=='a')
       {
//         UART2_Write_Text("Got a\r");          //got=="0000"
         state1 = 0;
         state2 = 0;
       }
       if (uart_rd=='b')
       {
//         UART2_Write_Text("Got b\r");          //got=="0100"
         state1 = 0;
         state2 = 1;
       }
       if (uart_rd=='c')
       {
//         UART2_Write_Text("Got c\r");          //got=="1000"
         state1 = 1;
         state2 = 0;
       }
       if (uart_rd=='d')
       {
//         UART2_Write_Text("Got d\r");          //got=="1100"
         state1 = 1;
         state2 = 1;
       }

       PORTBbits.RB14 = state1;
       PORTBbits.RB15 = state2;

//       delay_ms(500);
      flag = 0;
    }

    IntToStr(state1, text);
    Trimleft(text);
    UART1_Write_Text(text);
    IntToStr(state2, text1);
    Trimleft(text1);
    UART1_Write_Text(text1);
  }
}

void UART1_Interrupt() iv IVT_ADDR_U1RXINTERRUPT
{
  IFS0bits.U1RXIF = 0;

  if(U1STAbits.URXDA = 1)
  {
    uart_rd = U1RXREG;
    flag = 1;
  }
}


void TrimLeft(char *str) {
char count  = 1;
  while (*str++ != 0)
  {
     if (*str != 32)         // checks if whitespace is available, if available it'll remove it until no whitespace is available.
     {
          *(str - count--) = *str;
     }
     count++;
  }
}

void initialize_uart()
{
  Unlock_IOLOCK();
  PPS_Mapping_NoLock(RB7, _INPUT, _U1RX);   // Sets pin 2 to be Input, and maps as UART RX DATAIN Input to it
  PPS_Mapping_NoLock(RB8, _OUTPUT, _U1TX);   // Sets pin 3 to be Output, and maps as UART TX DATAOUT Output to it

//  PPS_Mapping_NoLock(RB14, _INPUT, _U2RX);   // Sets pin 2 to be Input, and maps as UART RX DATAIN Input to it
//  PPS_Mapping_NoLock(RB15, _OUTPUT, _U2TX);   // Sets pin 3 to be Output, and maps as UART TX DATAOUT Output to it
  Lock_IOLOCK();

  IEC0bits.U1RXIE = 1;    // Enable UART RX Interrupt

  UART1_Init(9600);               // Initialize UART module at 9600 bps
//  UART2_Init(9600);               // Initialize UART module at 9600 bps
  Delay_ms(100);                  // Wait for UART module to stabilize

//  UART2_Write_Text("Start.\r");
}




void initialize_adc()
{
  ADC1_Init_Advanced(_ADC_10bit, _ADC_INTERNAL_REF);
  PORTA=0X0000;
  PORTB=0X0000;

  TRISB12_bit = 1;
  TRISB13_bit = 1;
  TRISB14_bit=0;
  TRISB15_bit=0;
}

unsigned int get_adc(unsigned int channel)
{
    unsigned int x;
    unsigned int adc=0;

    for(x=0;x<25;x++)
    {
        adc =adc+ ADC1_Get_Sample(channel);
        delay_ms(10);
    }
    adc=adc/25;
    return adc;
}