
_main:
	MOV	#2048, W15
	MOV	#6142, W0
	MOV	WREG, 32
	MOV	#1, W0
	MOV	WREG, 52
	MOV	#4, W0
	IOR	68

;POC_Monitor.c,13 :: 		void main()
;POC_Monitor.c,16 :: 		AD1PCFGL        = 0b1110011111111111;        //assign  pin 11 and 12 to analog and all other pins to digital
	PUSH	W10
	PUSH	W11
	MOV	#59391, W0
	MOV	WREG, AD1PCFGL
;POC_Monitor.c,18 :: 		initialize_uart();
	CALL	_initialize_uart
;POC_Monitor.c,19 :: 		initialize_adc();
	CALL	_initialize_adc
;POC_Monitor.c,21 :: 		TRISB12_bit = 1;
	BSET	TRISB12_bit, BitPos(TRISB12_bit+0)
;POC_Monitor.c,22 :: 		TRISB13_bit = 1;
	BSET	TRISB13_bit, BitPos(TRISB13_bit+0)
;POC_Monitor.c,23 :: 		TRISB14_bit=0;
	BCLR	TRISB14_bit, BitPos(TRISB14_bit+0)
;POC_Monitor.c,24 :: 		TRISB15_bit=0;
	BCLR	TRISB15_bit, BitPos(TRISB15_bit+0)
;POC_Monitor.c,26 :: 		while (1)
L_main0:
;POC_Monitor.c,30 :: 		adc1=get_adc(11);
	MOV	#11, W10
	CALL	_get_adc
	MOV	W0, _adc1
;POC_Monitor.c,31 :: 		delay_ms(100);
	MOV	#2, W8
	MOV	#57296, W7
L_main2:
	DEC	W7
	BRA NZ	L_main2
	DEC	W8
	BRA NZ	L_main2
	NOP
	NOP
;POC_Monitor.c,32 :: 		adc2=get_adc(12);
	MOV	#12, W10
	CALL	_get_adc
	MOV	W0, _adc2
;POC_Monitor.c,33 :: 		delay_ms(100);
	MOV	#2, W8
	MOV	#57296, W7
L_main4:
	DEC	W7
	BRA NZ	L_main4
	DEC	W8
	BRA NZ	L_main4
	NOP
	NOP
;POC_Monitor.c,35 :: 		if(adc1>=600)
	MOV	_adc1, W1
	MOV	#600, W0
	CP	W1, W0
	BRA GEU	L__main28
	GOTO	L_main6
L__main28:
;POC_Monitor.c,38 :: 		state1=1;
	MOV	#1, W0
	MOV	W0, _state1
;POC_Monitor.c,39 :: 		}
	GOTO	L_main7
L_main6:
;POC_Monitor.c,42 :: 		state1=0;
	CLR	W0
	MOV	W0, _state1
L_main7:
;POC_Monitor.c,44 :: 		if(adc2>=600)
	MOV	_adc2, W1
	MOV	#600, W0
	CP	W1, W0
	BRA GEU	L__main29
	GOTO	L_main8
L__main29:
;POC_Monitor.c,47 :: 		state2=1;
	MOV	#1, W0
	MOV	W0, _state2
;POC_Monitor.c,48 :: 		}
	GOTO	L_main9
L_main8:
;POC_Monitor.c,51 :: 		state2=0;
	CLR	W0
	MOV	W0, _state2
L_main9:
;POC_Monitor.c,53 :: 		if (flag == 1)
	MOV	_flag, W0
	CP	W0, #1
	BRA Z	L__main30
	GOTO	L_main10
L__main30:
;POC_Monitor.c,55 :: 		if (uart_rd=='a')
	MOV	#lo_addr(_uart_rd), W0
	MOV.B	[W0], W1
	MOV.B	#97, W0
	CP.B	W1, W0
	BRA Z	L__main31
	GOTO	L_main11
L__main31:
;POC_Monitor.c,58 :: 		state1 = 0;
	CLR	W0
	MOV	W0, _state1
;POC_Monitor.c,59 :: 		state2 = 0;
	CLR	W0
	MOV	W0, _state2
;POC_Monitor.c,60 :: 		}
L_main11:
;POC_Monitor.c,61 :: 		if (uart_rd=='b')
	MOV	#lo_addr(_uart_rd), W0
	MOV.B	[W0], W1
	MOV.B	#98, W0
	CP.B	W1, W0
	BRA Z	L__main32
	GOTO	L_main12
L__main32:
;POC_Monitor.c,64 :: 		state1 = 0;
	CLR	W0
	MOV	W0, _state1
;POC_Monitor.c,65 :: 		state2 = 1;
	MOV	#1, W0
	MOV	W0, _state2
;POC_Monitor.c,66 :: 		}
L_main12:
;POC_Monitor.c,67 :: 		if (uart_rd=='c')
	MOV	#lo_addr(_uart_rd), W0
	MOV.B	[W0], W1
	MOV.B	#99, W0
	CP.B	W1, W0
	BRA Z	L__main33
	GOTO	L_main13
L__main33:
;POC_Monitor.c,70 :: 		state1 = 1;
	MOV	#1, W0
	MOV	W0, _state1
;POC_Monitor.c,71 :: 		state2 = 0;
	CLR	W0
	MOV	W0, _state2
;POC_Monitor.c,72 :: 		}
L_main13:
;POC_Monitor.c,73 :: 		if (uart_rd=='d')
	MOV	#lo_addr(_uart_rd), W0
	MOV.B	[W0], W1
	MOV.B	#100, W0
	CP.B	W1, W0
	BRA Z	L__main34
	GOTO	L_main14
L__main34:
;POC_Monitor.c,76 :: 		state1 = 1;
	MOV	#1, W0
	MOV	W0, _state1
;POC_Monitor.c,77 :: 		state2 = 1;
	MOV	#1, W0
	MOV	W0, _state2
;POC_Monitor.c,78 :: 		}
L_main14:
;POC_Monitor.c,80 :: 		PORTBbits.RB14 = state1;
	MOV	#lo_addr(_state1), W0
	BTSS	[W0], #0
	BCLR	PORTBbits, #14
	BTSC	[W0], #0
	BSET	PORTBbits, #14
;POC_Monitor.c,81 :: 		PORTBbits.RB15 = state2;
	MOV	#lo_addr(_state2), W0
	BTSS	[W0], #0
	BCLR	PORTBbits, #15
	BTSC	[W0], #0
	BSET	PORTBbits, #15
;POC_Monitor.c,84 :: 		flag = 0;
	CLR	W0
	MOV	W0, _flag
;POC_Monitor.c,85 :: 		}
L_main10:
;POC_Monitor.c,87 :: 		IntToStr(state1, text);
	MOV	#lo_addr(_text), W11
	MOV	_state1, W10
	CALL	_IntToStr
;POC_Monitor.c,88 :: 		Trimleft(text);
	MOV	#lo_addr(_text), W10
	CALL	_TrimLeft
;POC_Monitor.c,89 :: 		UART1_Write_Text(text);
	MOV	#lo_addr(_text), W10
	CALL	_UART1_Write_Text
;POC_Monitor.c,90 :: 		IntToStr(state2, text1);
	MOV	#lo_addr(_text1), W11
	MOV	_state2, W10
	CALL	_IntToStr
;POC_Monitor.c,91 :: 		Trimleft(text1);
	MOV	#lo_addr(_text1), W10
	CALL	_TrimLeft
;POC_Monitor.c,92 :: 		UART1_Write_Text(text1);
	MOV	#lo_addr(_text1), W10
	CALL	_UART1_Write_Text
;POC_Monitor.c,93 :: 		}
	GOTO	L_main0
;POC_Monitor.c,94 :: 		}
L_end_main:
	POP	W11
	POP	W10
L__main_end_loop:
	BRA	L__main_end_loop
; end of _main

_UART1_Interrupt:
	PUSH	52
	PUSH	RCOUNT
	PUSH	W0
	MOV	#2, W0
	REPEAT	#12
	PUSH	[W0++]

;POC_Monitor.c,96 :: 		void UART1_Interrupt() iv IVT_ADDR_U1RXINTERRUPT
;POC_Monitor.c,98 :: 		IFS0bits.U1RXIF = 0;
	BCLR	IFS0bits, #11
;POC_Monitor.c,100 :: 		if(U1STAbits.URXDA = 1)
	BSET	U1STAbits, #0
	BTSS	U1STAbits, #0
	GOTO	L_UART1_Interrupt15
;POC_Monitor.c,102 :: 		uart_rd = U1RXREG;
	MOV	#lo_addr(_uart_rd), W1
	MOV.B	U1RXREG, WREG
	MOV.B	W0, [W1]
;POC_Monitor.c,103 :: 		flag = 1;
	MOV	#1, W0
	MOV	W0, _flag
;POC_Monitor.c,104 :: 		}
L_UART1_Interrupt15:
;POC_Monitor.c,105 :: 		}
L_end_UART1_Interrupt:
	MOV	#26, W0
	REPEAT	#12
	POP	[W0--]
	POP	W0
	POP	RCOUNT
	POP	52
	RETFIE
; end of _UART1_Interrupt

_TrimLeft:

;POC_Monitor.c,108 :: 		void TrimLeft(char *str) {
;POC_Monitor.c,109 :: 		char count  = 1;
; count start address is: 4 (W2)
	MOV.B	#1, W2
; count end address is: 4 (W2)
;POC_Monitor.c,110 :: 		while (*str++ != 0)
L_TrimLeft16:
; count start address is: 4 (W2)
	MOV	W10, W1
	ADD	W10, #1, W0
	MOV	W0, W10
	MOV.B	[W1], W0
	CP.B	W0, #0
	BRA NZ	L__TrimLeft38
	GOTO	L_TrimLeft17
L__TrimLeft38:
;POC_Monitor.c,112 :: 		if (*str != 32)         // checks if whitespace is available, if available it'll remove it until no whitespace is available.
	MOV.B	[W10], W1
	MOV.B	#32, W0
	CP.B	W1, W0
	BRA NZ	L__TrimLeft39
	GOTO	L__TrimLeft26
L__TrimLeft39:
;POC_Monitor.c,114 :: 		*(str - count--) = *str;
	ZE	W2, W0
	SUB	W10, W0, W0
	MOV.B	[W10], [W0]
; count start address is: 0 (W0)
	SUB.B	W2, #1, W0
; count end address is: 4 (W2)
; count end address is: 0 (W0)
;POC_Monitor.c,115 :: 		}
	GOTO	L_TrimLeft18
L__TrimLeft26:
;POC_Monitor.c,112 :: 		if (*str != 32)         // checks if whitespace is available, if available it'll remove it until no whitespace is available.
	MOV.B	W2, W0
;POC_Monitor.c,115 :: 		}
L_TrimLeft18:
;POC_Monitor.c,116 :: 		count++;
; count start address is: 0 (W0)
; count start address is: 4 (W2)
	ADD.B	W0, #1, W2
; count end address is: 0 (W0)
;POC_Monitor.c,117 :: 		}
; count end address is: 4 (W2)
	GOTO	L_TrimLeft16
L_TrimLeft17:
;POC_Monitor.c,118 :: 		}
L_end_TrimLeft:
	RETURN
; end of _TrimLeft

_initialize_uart:

;POC_Monitor.c,120 :: 		void initialize_uart()
;POC_Monitor.c,122 :: 		Unlock_IOLOCK();
	PUSH	W10
	PUSH	W11
	PUSH	W12
	CALL	_Unlock_IOLOCK
;POC_Monitor.c,123 :: 		PPS_Mapping_NoLock(RB7, _INPUT, _U1RX);   // Sets pin 2 to be Input, and maps as UART RX DATAIN Input to it
	MOV.B	#11, W12
	MOV.B	#1, W11
	MOV.B	#7, W10
	CALL	_PPS_Mapping_NoLock
;POC_Monitor.c,124 :: 		PPS_Mapping_NoLock(RB8, _OUTPUT, _U1TX);   // Sets pin 3 to be Output, and maps as UART TX DATAOUT Output to it
	MOV.B	#3, W12
	CLR	W11
	MOV.B	#8, W10
	CALL	_PPS_Mapping_NoLock
;POC_Monitor.c,128 :: 		Lock_IOLOCK();
	CALL	_Lock_IOLOCK
;POC_Monitor.c,130 :: 		IEC0bits.U1RXIE = 1;    // Enable UART RX Interrupt
	BSET	IEC0bits, #11
;POC_Monitor.c,132 :: 		UART1_Init(9600);               // Initialize UART module at 9600 bps
	MOV	#9600, W10
	MOV	#0, W11
	CALL	_UART1_Init
;POC_Monitor.c,134 :: 		Delay_ms(100);                  // Wait for UART module to stabilize
	MOV	#2, W8
	MOV	#57296, W7
L_initialize_uart19:
	DEC	W7
	BRA NZ	L_initialize_uart19
	DEC	W8
	BRA NZ	L_initialize_uart19
	NOP
	NOP
;POC_Monitor.c,137 :: 		}
L_end_initialize_uart:
	POP	W12
	POP	W11
	POP	W10
	RETURN
; end of _initialize_uart

_initialize_adc:

;POC_Monitor.c,142 :: 		void initialize_adc()
;POC_Monitor.c,144 :: 		ADC1_Init_Advanced(_ADC_10bit, _ADC_INTERNAL_REF);
	PUSH	W10
	PUSH	W11
	CLR	W11
	CLR	W10
	CALL	_ADC1_Init_Advanced
;POC_Monitor.c,145 :: 		PORTA=0X0000;
	CLR	PORTA
;POC_Monitor.c,146 :: 		PORTB=0X0000;
	CLR	PORTB
;POC_Monitor.c,148 :: 		TRISB12_bit = 1;
	BSET	TRISB12_bit, BitPos(TRISB12_bit+0)
;POC_Monitor.c,149 :: 		TRISB13_bit = 1;
	BSET	TRISB13_bit, BitPos(TRISB13_bit+0)
;POC_Monitor.c,150 :: 		TRISB14_bit=0;
	BCLR	TRISB14_bit, BitPos(TRISB14_bit+0)
;POC_Monitor.c,151 :: 		TRISB15_bit=0;
	BCLR	TRISB15_bit, BitPos(TRISB15_bit+0)
;POC_Monitor.c,152 :: 		}
L_end_initialize_adc:
	POP	W11
	POP	W10
	RETURN
; end of _initialize_adc

_get_adc:

;POC_Monitor.c,154 :: 		unsigned int get_adc(unsigned int channel)
;POC_Monitor.c,157 :: 		unsigned int adc=0;
; adc start address is: 6 (W3)
	CLR	W3
;POC_Monitor.c,159 :: 		for(x=0;x<25;x++)
; x start address is: 4 (W2)
	CLR	W2
; adc end address is: 6 (W3)
; x end address is: 4 (W2)
L_get_adc21:
; x start address is: 4 (W2)
; adc start address is: 6 (W3)
	CP	W2, #25
	BRA LTU	L__get_adc43
	GOTO	L_get_adc22
L__get_adc43:
;POC_Monitor.c,161 :: 		adc =adc+ ADC1_Get_Sample(channel);
	CALL	_ADC1_Get_Sample
	ADD	W3, W0, W3
;POC_Monitor.c,162 :: 		delay_ms(10);
	MOV	#12283, W7
L_get_adc24:
	DEC	W7
	BRA NZ	L_get_adc24
	NOP
;POC_Monitor.c,159 :: 		for(x=0;x<25;x++)
	INC	W2
;POC_Monitor.c,163 :: 		}
; x end address is: 4 (W2)
	GOTO	L_get_adc21
L_get_adc22:
;POC_Monitor.c,164 :: 		adc=adc/25;
	MOV	#25, W2
	REPEAT	#17
	DIV.U	W3, W2
; adc end address is: 6 (W3)
;POC_Monitor.c,165 :: 		return adc;
;POC_Monitor.c,166 :: 		}
L_end_get_adc:
	RETURN
; end of _get_adc
