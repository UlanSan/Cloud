#define F_CPU 8000000L
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//--------------------------------------------
void USART_Init( unsigned int speed);
void USART_Transmit( unsigned char data );
//--------------------------------------------

//--------------------------------------------
void timer_ini(void)
{
	TCCR1B |= (1<<WGM12); // устанавливаем режим СТС (сброс по совпадению для таймера 1)
	TCCR2 |= (1<<WGM21); // для таймера 2
	TIMSK |= (1<<OCIE1A) | (1<<OCIE2); //устанавливаем бит разрешения прерывания 1ого счетчика по совпадению с OCR1A (H & L) и 2ого по OCR2
	TCCR1B |= (1<<CS12);//установим делитель, так как таймер 1 на 16 бит, таймер 2 на 8 бит. т.е. делим на 2^8
	OCR1AH = 0b00000000: //записываем в  старший регистр число для сравнения
	//OCR1AL = 0b01111000; // в младший
	//OCR2 = 0b00111100;
	OCR1AL = TIMER1_INTERVAL; // в младший
	OCR2 = TIMER2_INTERVAL;
}
//--------------------------------------------

void USART_Init( unsigned int speed)//Инициализация модуля USART
{
		UBRRH = (unsigned char)(speed>>8);
		UBRRL = (unsigned char)speed;
		
		UCSRB=(1<<RXEN)|( 1<<TXEN); //Включаем прием и передачу по USART
		UCSRB |= (1<<RXCIE); //Разрешаем прерывание при передаче
		UCSRA |= (1<<U2X); // Для 8 мгц
		UCSRC = (1<<URSEL)|(1<<USBS)|(1<<UCSZ1)|(1<<UCSZ0);// Обращаемся именно к регистру UCSRC (URSEL=1),
		//ассинхронный режим (UMSEL=0), без контроля четности (UPM1=0 и UPM0=0),
		//1 стоп-бит (USBS=0), 8-бит посылка (UCSZ1=1 и UCSZ0=1)
		//UCSRC |= (1<<UPM1);//четность
}

void USART_Transmit( unsigned char data ) //Функция отправки данных
{
	while ( !(UCSRA & (1<<UDRE)) ); //Ожидание опустошения буфера приема
	UDR = data; //Начало передачи данных
}

//-----------------------------------------------------------

ISR (TIMER1_COMPA_vect)
{
	USART_Transmit('ping');
	USART_Transmit(0x0d);//переход в начало строки
	USART_Transmit(0x0a);//переход на новую строку
}

ISR (TIMER2_COMP_vect)
{
	USART_Transmit('pong');
	USART_Transmit(0x0d);//переход в начало строки
	USART_Transmit(0x0a);//переход на новую строку
}
//--------------------------------------------
int main(void)
{
	USART_Init (8);//115200
	unsigned char TIMER1_INTERVAL=0b01111000, TIMER2_INTERVAL=0b00111100;
	timer_ini();
	sei();
	while(1)
	{

	}
}