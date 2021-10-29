
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/iom128a.h>
#include "IncFile1.h"

void init_port(){
	
	PORTA = 0x00;
	PORTB = 0x01;
	PORTC = 0x00;
	PORTE = 0x00;
	PORTG = 0x00;
	
	DDRA = 0x00;
	DDRB = 0x01;
	DDRC = 0xff;
	DDRE = 0x00;
	DDRG = 0x00;
}

void time_0(){//5ms
	cli();
	TCCR0=0x0e;
	OCR0=215;
	TIMSK=0x12;
	sei();
}

void time_1(){//1.36ms
	cli();
	TCCR1A=0x00;
	TCCR1B=0x0A;
	OCR1A=1879;
	TIMSK=0x12;
	sei();
}

ISR(TIMER0_COMP_vect)
{
	if (Time_STOP==1)
	{
		static int gtimer0_cnt=0;
		gtimer0_cnt++;
		if (gtimer0_cnt>=200)
		{
			gtimer0_cnt=0;
			sec_count++;
		}
		if (sec_count>=60)
		{
			sec_count=0;
			min_count++;
		}
		if (min_count>=60)
		{
			min_count=0;
			hour_count++;
		}
		if (hour_count>11)
		{
			hour_count=0;
		}
	}
	

}

ISR(TIMER1_COMPA_vect)
{
	PORTA=0x00;

	//스위치
	l_btPINB_0x10 = PINB&0x10; //start/stop/reset/min_up 스위치
	l_btPINB_0x20=PINB&0x20; //hour_up/min_down 스위치
	l_btPINB_0x40=PINB&0x40; //hour_down 스위치
	if(l_btPINBTempOld[gtimer1_cnt] = l_btPINB_0x10) // l_btPINBTempOld값과 l_btPINB_0x10 값이 같은지 다른지 비교
	{
		l_btPINBTempOld[gtimer1_cnt] != l_btPINB_0x10;
	}
	else if (l_btPINBTempOld[gtimer1_cnt] = l_btPINB_0x20)
	{
		l_btPINBTempOld[gtimer1_cnt] != l_btPINB_0x20;
	}
	else if (l_btPINBTempOld[gtimer1_cnt] = l_btPINB_0x40)
	{
		l_btPINBTempOld[gtimer1_cnt] != l_btPINB_0x40;
	}
	else
	{
		
		g_btscankey[0] = l_btPINBTempOld[0]; //start
		g_btscankey[1] = l_btPINBTempOld[1]; //stop
		g_btscankey[2] = l_btPINBTempOld[2]; //reset
		
		g_btscankey[3] = l_btPINBTempOld[7]; //min_up
		g_btscankey[4] = l_btPINBTempOld[6]; //hour_up
		
		g_btscankey[5] = l_btPINBTempOld[7]; //min_down
		g_btscankey[6] = l_btPINBTempOld[6]; //hour_down
	}
	
	
	if(gtimer1_cnt<8){
		gtimer1_cnt++;
	}
	else {
		gtimer1_cnt = 0;
	}
	
	PORTC=~(USER_Output_Buffer[gtimer1_cnt]);
	PORTA=0x01<<gtimer1_cnt;
}

void User_ScanDisplay(void){
	int num=0;
	for (int i; i<8;i++)
	{
		num=0;
		num=(USER_Buffer[7]&(1<<i))?1:0;
		num<<=1;
		num|=(USER_Buffer[6]&(1<<i))?1:0;
		num<<=1;
		num|=(USER_Buffer[5]&(1<<i))?1:0;
		num<<=1;
		num|=(USER_Buffer[4]&(1<<i))?1:0;
		num<<=1;
		num|=(USER_Buffer[3]&(1<<i))?1:0;
		num<<=1;
		num|=(USER_Buffer[2]&(1<<i))?1:0;
		num<<=1;
		num|=(USER_Buffer[1]&(1<<i))?1:0;
		num<<=1;
		num|=(USER_Buffer[0]&(1<<i))?1:0;
		USER_Output_Buffer[i]=num;
	}
}

void User_time_Reset(){ //타이머

	hour_count=0,min_count=0,sec_count=0;
}

void clock_init(){
	for (int i=0; i<7;i++)
	{
		USER_Buffer[i]=USER_Segment_Table[0];
	}
	User_ScanDisplay();
}

void clock__hour_updown(int key_delay){

	if (hour_up_key==set)
	{
		
		USER_delay_ms(key_delay);
		if (hour_count%10<10)
		{
			hour_count++;
			if (hour_count/10==1)//시단위 십의 자리
			{
				if (hour_count%10>1)//날짜 바뀜
				{
					hour_count=0;
					data++;
					if (data>1)
					{
						data=0;
					}
				}
			}
		}
	}
	else if (hour_down_key==set)
	{
		if (hour_count%10<10)
		{
			USER_delay_ms(key_delay);
			hour_count--;
			if (hour_count<0)//시단위 십의 자리
			{
				hour_count=11;
				data--;
				if (data<0)
				{
					data=1;
				}
			}

		}
	}
	

}

void clock_min_updown(int key_delay){
	if (min_up_key==set)
	{
		if (min_count%10<10)
		{
			USER_delay_ms(key_delay);
			min_count++;
			if (min_count/10>5)
			{
				min_count=0;
				hour_count++;
				if (hour_count/10==1)//시단위 십의 자리
				{
					if (hour_count%10>1)//날짜 바뀜
					{
						hour_count=0;
						data++;
						if (data>1)
						{
							data=0;
						}
					}
				}
			}
			
		}
	}
	else if (min_down_key==set)
	{
		if (min_count%10<10)
		{
			USER_delay_ms(key_delay);
			min_count--;
			if (min_count<0)
			{
				min_count=59;
				hour_count--;
				if (hour_count<0)//시단위 십의 자리
				{
					hour_count=11;
					data--;
					if (data<0)
					{
						data=1;
					}
				}
				
			}
			
		}
	}
}

void clock_display(){
	USER_Buffer[6]=segment_A_P[data];
	USER_Buffer[5]=USER_Segment_Table[sec_count%10];
	USER_Buffer[4]=USER_Segment_Table[sec_count/10];
	USER_Buffer[3]=USER_Segment_Table[min_count%10];
	USER_Buffer[2]=USER_Segment_Table[min_count/10];
	USER_Buffer[1]=USER_Segment_Table[hour_count%10];
	USER_Buffer[0]=USER_Segment_Table[hour_count/10];
}

void USER_delay_us(unsigned int time_us)
{
	register unsigned char i;
	
	for(i = 0; i < time_us; i++)   // 4 cycle
	{
		asm volatile("PUSH R0 "); // + 2 cycle
		asm volatile("POP  R0 "); // + 2 cycle; 합계 8 cycles * 125 ns = 1000 ns = 1 us at 8MHz
	}
}
void USER_delay_ms(unsigned int time_ms)
{
	register unsigned int i;

	for(i = 0; i < time_ms; i++)
	{
		USER_delay_us(250);
		USER_delay_us(250);
		USER_delay_us(250);
		USER_delay_us(250);
	}
}

int main(void)
{
	init_port();
	time_0();
	time_1();
	int t=0;
	
	Time_STOP=1;
	clock_init(); //시계
	while (1)
	{
		 
 		if (stopkey==1) //portB의 6번째 입력값을 알고 싶은 경우(key_value1)
 		{
 			Time_STOP=0;
 		}else if (startkey==1)
 		{
 			Time_STOP=1;
		}else if (resetkey==1) //portB의 7번째 입력값을 알고 싶은 경우(key_value1)
 		{
 			User_time_Reset();
 		}
		
		if (min_up_key==set||min_down_key==set)
		{
			temp_delay++;
			if (temp_delay>9){min_delay=50;}
			else{ min_delay=200;}
		}
		else if (hour_up_key||hour_down_key)
		{
			temp_delay++;
			if(temp_delay>10){hour_delay=50;}
			else{hour_delay=200;}
			}else{
			temp_delay=0;
		}
		

		clock__hour_updown(hour_delay);
		clock_min_updown(min_delay);
		clock_display();
		
		User_ScanDisplay();

	}
}

