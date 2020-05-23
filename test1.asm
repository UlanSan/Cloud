.include "m168def.inc"
.equ Bitrate = 9600
.equ BAUD = 8000000 / (16 * Bitrate) - 1
.equ TIM_0_DELAY_MILISEC = 10  ; 1-32
.equ TIM_2_DELAY_MILISEC = 10  ; 1-32
.equ TIM_0_DELAY_FIRST = 0x00
.equ TIM_2_DELAY_FIRST = 0x80
.equ TIM_0_DELAY = TIM_0_DELAY_MILISEC * 8 - 1
.equ TIM_2_DELAY = TIM_0_DELAY_MILISEC * 8 - 1
.equ TIM_0_DELAY_REG = 0xff - TIM_0_DELAY
.equ TIM_2_DELAY_REG = 0xff - TIM_2_DELAY
.dseg
.cseg

.org 0x0000
jmp Reset
jmp EXT_INT0
jmp EXT_INT1
jmp PC_INT0
jmp PC_INT1
jmp PC_INT2
jmp WDT
jmp TIM2_COMPA
jmp TIM2_COMPB
jmp TIM2_OVF
jmp TIM1_CAPT
jmp TIM1_COMPA
jmp TIM1_COMPB
jmp TIM1_OVF
jmp TIM0_COMPA
jmp TIM0_COMPB
jmp TIM0_OVF
jmp SPI_STC
jmp USART_RXC
jmp USART_UDRE
jmp USART_TXC
jmp ADC_INT
jmp EE_RDY
jmp ANA_COMP
jmp TWI
jmp SPM_RDY

mes1:
.db 'P','I','N','G','\r','\n'

mes2:
.db 'P','O','N','G','\r','\n'

reset:
	ldi r16, high(ramend)
	out sph, r16
	ldi r16, low(ramend)
	out spl, r16

	ldi r16, high(BAUD)
	sts UBRR0H, r16
	ldi r16, low(BAUD)
	sts UBRR0L, r16

	ldi r16, 0b11011000 
	sts UCSR0B, r16 
	ldi r16, 0b00000110 
	sts UCSR0C, r16


	ldi r16, 0b00000101
	out TCCR0B, r16
	sts TCCR2B, r16

    ldi r16, 0b00000001
	sts TIMSK0, r16
	sts TIFR0, r16
	sts TIMSK2, r16
	sts TIFR2, r16

	ldi r16, TIM_0_DELAY_FIRST
	sts TCNT0, r16

	ldi r16, TIM_2_DELAY_FIRST 
	sts TCNT2, r16
	
	sei

start:
	rjmp start


send:
	lpm
	mov r17, r0
label:
	sts UDR0, r17
	ldi r20, 0
	ret
USART_TXC:
	cli
	push r16
	push r18
	push r17
	lds r18, UCSR0A
    sbrs r18,UDRE0
	rjmp USART_TXC
	cpi r20, 1
	breq VixUT
	ldi r16, 1
	add ZL, r16
	ldi r16, 0
	adc ZH, r16
	lpm                  
	mov r17, r0
	sts UDR0, r17
	cpi r17, '\n'
	brne VixUT
	ldi r20, 1
VixUT:
	pop r16
	pop r17
	pop r18
	sei
	reti

TIM0_OVF:
	ldi ZH,High(mes1*2) ; 1ый
    ldi ZL,Low(mes1*2) ; 2ой
	lpm ; 3ий
	sts UDR0, r0 ; 4ый
	ldi r20, 0
	ldi r16, TIM_0_DELAY_REG
	sts TCNT0, r16
	reti
TIM2_OVF:
	ldi ZH,High(mes2*2) ; 1ый
    ldi ZL,Low(mes2*2) ; 2ой
	lpm ; 3ий
	sts UDR0, r0 ; 4ый
	ldi r20, 0
	ldi r16, TIM_2_DELAY_REG
	sts TCNT2, r16
	reti

EXT_INT0:
EXT_INT1:
PC_INT0:
PC_INT1:
PC_INT2:
WDT:
TIM2_COMPA:
TIM2_COMPB:
TIM2_OVF_2:
TIM1_CAPT:
TIM1_COMPA:
TIM1_COMPB:
TIM1_OVF:
TIM0_COMPA:
TIM0_COMPB:
TIM0_OVF_2:
SPI_STC:
USART_RXC:
USART_UDRE:
ADC_INT:
EE_RDY:
ANA_COMP:
TWI:
SPM_RDY:
      reti
