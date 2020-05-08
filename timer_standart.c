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
	TCCR1B |= (1<<WGM12); // ������������� ����� ��� (����� �� ���������� ��� ������� 1)
	TCCR2 |= (1<<WGM21); // ��� ������� 2
	TIMSK |= (1<<OCIE1A) | (1<<OCIE2); //������������� ��� ���������� ���������� 1��� �������� �� ���������� � OCR1A (H & L) � 2��� �� OCR2
	TCCR1B |= (1<<CS12);//��������� ��������, ��� ��� ������ 1 �� 16 ���, ������ 2 �� 8 ���. �.�. ����� �� 2^8
	OCR1AH = 0b00000000: //���������� �  ������� ������� ����� ��� ���������
	//OCR1AL = 0b01111000; // � �������
	//OCR2 = 0b00111100;
	OCR1AL = TIMER1_INTERVAL; // � �������
	OCR2 = TIMER2_INTERVAL;
}
//--------------------------------------------

void USART_Init( unsigned int speed)//������������� ������ USART
{
		UBRRH = (unsigned char)(speed>>8);
		UBRRL = (unsigned char)speed;
		
		UCSRB=(1<<RXEN)|( 1<<TXEN); //�������� ����� � �������� �� USART
		UCSRB |= (1<<RXCIE); //��������� ���������� ��� ��������
		UCSRA |= (1<<U2X); // ��� 8 ���
		UCSRC = (1<<URSEL)|(1<<USBS)|(1<<UCSZ1)|(1<<UCSZ0);// ���������� ������ � �������� UCSRC (URSEL=1),
		//������������ ����� (UMSEL=0), ��� �������� �������� (UPM1=0 � UPM0=0),
		//1 ����-��� (USBS=0), 8-��� ������� (UCSZ1=1 � UCSZ0=1)
		//UCSRC |= (1<<UPM1);//��������
}

void USART_Transmit( unsigned char data ) //������� �������� ������
{
	while ( !(UCSRA & (1<<UDRE)) ); //�������� ����������� ������ ������
	UDR = data; //������ �������� ������
}

//-----------------------------------------------------------

ISR (TIMER1_COMPA_vect)
{
	USART_Transmit('ping');
	USART_Transmit(0x0d);//������� � ������ ������
	USART_Transmit(0x0a);//������� �� ����� ������
}

ISR (TIMER2_COMP_vect)
{
	USART_Transmit('pong');
	USART_Transmit(0x0d);//������� � ������ ������
	USART_Transmit(0x0a);//������� �� ����� ������
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