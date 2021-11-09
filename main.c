
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/iom128a.h>
#include "IncFile1.h"

unsigned char Segment[10]= {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x27, 0x7f, 0x6f};//숫자 세그먼트 배열
unsigned char am_pm_data[2]={0x77, 0x73};//AM,PM 표시를 위한 배열
unsigned char outputbuffer[8]= {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //portC로 나가는 배열
unsigned char Data_Buffer[8]; // 데이터 임시 저장 배열


unsigned char g_btscankey[8]; //스위치 키 최종 저장

int timer0_count=0; //타이머 0 카운트
int timer1_count=0; //타이머 1 카운트

int hour_count=12, min_count=0, sec_count=0; //시, 분, 초, 변수
unsigned char AM_PM=0; //am,pm 변수

unsigned char Time_Change=0; //시작,멈춤을 위한 변수

int User_delay=0;// 딜레이주기 위한 변수
int system_LED=0;// system LED 변수

unsigned char  minup_Flag=0,mindown_Flag=0,hourup_Flag=0,hourdown_Flag=0;
//ON_Flag=0; // 상태변화를 위한 변수
unsigned char Buffer[8]={0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}; //비트쉬프트를 위한 배열

unsigned char T_SCAN1[8],T_SCAN2[8],T_SCAN3[8];
unsigned char g_btScanKeyValue1[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
unsigned char g_btScanKeyValue2[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
unsigned char g_btScanKeyValue3[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

void Init_Port(){   //port/register 초기화
   PORTA = 0x00;
   PORTB = 0x81;
   PORTC = 0x00;
   PORTE = 0x00;
   PORTG = 0xff;

   DDRA = 0x00;
   DDRB = 0x81;
   DDRC = 0xff;
   DDRE = 0x02;
   DDRG = 0x1f;
}

ISR(TIMER0_COMP_vect)//5ms
{
   timer0_count++;
   
   system_LED++;
   
   User_delay++;
   
}

ISR(TIMER1_COMPA_vect)//1.36ms
{

   //    unsigned char l_btScanTmp_0x10,l_btScanTmp_0x20,l_btScanTmp_0x40;
   //    l_btScanTmp_0x10   = (PINB & 0x10);
   //    l_btScanTmp_0x20   = (PINB & 0x20);
   //    l_btScanTmp_0x40   = (PINB & 0x40);
   //    if( l_btScanTmp_0x10 != g_btScanKeyValue[ timer1_count ] )
   //    {
   //       g_btScanKeyValue[ timer1_count ] = l_btScanTmp_0x10;
   //    }else if ( l_btScanTmp_0x20 != g_btScanKeyValue[ timer1_count ] )
   //    {
   //       g_btScanKeyValue[ timer1_count ] = l_btScanTmp_0x20;
   //    }else if ( l_btScanTmp_0x40 != g_btScanKeyValue[ timer1_count ] )
   //    {
   //       g_btScanKeyValue[ timer1_count ] = l_btScanTmp_0x40;
   //    }
   //    else
   //    {
   //       T_SCAN[ timer1_count ]= g_btScanKeyValue[ timer1_count ];
   //    }
   
   unsigned char l_btScanTmp0x10,l_btScanTmp0x20,l_btScanTmp0x40;
   l_btScanTmp0x10   = ((PINB&0x10));
   l_btScanTmp0x20   = ((PINB&0x20));
   l_btScanTmp0x40   = ((PINB&0x40));

   if( l_btScanTmp0x10 != g_btScanKeyValue1[ timer1_count ] )
   {
      g_btScanKeyValue1[ timer1_count ] = l_btScanTmp0x10;
   }else if (l_btScanTmp0x20 != g_btScanKeyValue2[ timer1_count ])
   {
      g_btScanKeyValue2[ timer1_count ] = l_btScanTmp0x20;
   }else if (l_btScanTmp0x40 != g_btScanKeyValue3[ timer1_count ])
   {
      g_btScanKeyValue3[ timer1_count ] = l_btScanTmp0x40;
   }
   else
   {
      T_SCAN1[ timer1_count ]= g_btScanKeyValue1[ timer1_count ];
      T_SCAN2[ timer1_count ]= g_btScanKeyValue2[ timer1_count ];
      T_SCAN3[ timer1_count ]= g_btScanKeyValue3[ timer1_count ];
      
   }
   
   timer1_count++;
   if(timer1_count >= 8) timer1_count = 0;
   
   PORTC=outputbuffer[timer1_count];
   PORTA=Buffer[timer1_count];
   
   
}

void Switch_ScanKey()   //switch key scan 함수
{
   unsigned char PINB_0x10, PINB_0x20, PINB_0x40;
   static unsigned char cnt=0;             //이전 값과 현재 누른 값을 체크하기 위한 카운트
   static unsigned char PINB_Buffer[8];  //스위치 값 임시 저장
   PINB_0x10=((PINB&0x10)); //start/stop/reset/min_up 스위치
   PINB_0x20=((PINB&0x20)); //hour_up/min_down 스위치
   PINB_0x40=((PINB&0x40)); //hour_down 스위치
   
   if(PINB_0x10 != PINB_Buffer[timer1_count])   // 이전값과 현재 값 비교
   {
      PINB_Buffer[timer1_count] = PINB_0x10;
      //cnt=4;
   }
   else if (PINB_0x20 != PINB_Buffer[timer1_count])
   {
      PINB_Buffer[timer1_count] = PINB_0x20;
      //cnt=4;
   }
   else if (PINB_0x40 != PINB_Buffer[timer1_count])
   {
      PINB_Buffer[timer1_count] = PINB_0x40;
      //cnt=4;
   }
   else
   {
      //cnt--;
      //if (cnt<=0)                      //n번 맞으면
      //{
      g_btscankey[0] = PINB_Buffer[0]; //start
      g_btscankey[1] = PINB_Buffer[1]; //stop
      g_btscankey[2] = PINB_Buffer[2]; //reset
      
      g_btscankey[3] = PINB_Buffer[7]; //min_up
      g_btscankey[4] = PINB_Buffer[6]; //hour_up
      
      g_btscankey[5] = PINB_Buffer[7]; //min_down
      g_btscankey[6] = PINB_Buffer[6]; //hour_down
      //}
   }
   
   //       unsigned char g_btscankey1;
   //       unsigned char bttemp;
   //       static unsigned char cnt=0;      //이전 값과 현재 누른 값을 체크하기 위한 카운트
   //       static unsigned char btbuffer=0;
   //
   //       bttemp=((PINB&0x0C)^0x0c); //start/stop/reset/min_up 스위치
   //       if(bttemp != btbuffer)   // 이전값과 현재 값 비교
   //       {
   //          btbuffer = bttemp;
   //          cnt=3;
   //       }
   //       else
   //       {
   //          cnt--;
   //          if (cnt<=0)                      //n번 맞으면
   //          {
   //             PortB.byt = bttemp; //start
   //
   //          }
   //       }
}

void User_LED(){   //system LED 1초마다 토글
   if (system_LED>=200)
   {
      PORTB^=0x01;
      system_LED=0;
   }
}

void time_count(){      //시계 초, 분, 시 카운트 증가 함수
   if (Time_Change==1)
   {
      if (timer0_count>=200)
      {
         timer0_count=0;
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
      if (hour_count>12)
      {
         hour_count=1;
      }
   }
}

void time_0(){   //타이머0 분주비 설정
   cli();
   TCCR0=0x0e;
   OCR0=215;
   TIMSK=0x12;
   sei();
}

void time_1(){   //타이머1 분주비 설정
   cli();
   TCCR1A=0x00;
   TCCR1B=0x0A;
   OCR1A=1879;
   TIMSK=0x12;
   sei();
}

void USER_PORTC_SCAN(){
   
   unsigned char bitloop;
   unsigned char bittemp;
   
   for(bitloop=0; bitloop<8; bitloop++){
      
      bittemp=0;
      bittemp|=((Data_Buffer[7]&1<<bitloop))?0:1;
      bittemp<<=1;
      bittemp|=((Data_Buffer[6]&1<<bitloop))?0:1;
      bittemp<<=1;
      bittemp|=((Data_Buffer[5]&1<<bitloop))?0:1;
      bittemp<<=1;
      bittemp|=((Data_Buffer[4]&1<<bitloop))?0:1;
      bittemp<<=1;
      bittemp|=((Data_Buffer[3]&1<<bitloop))?0:1;
      bittemp<<=1;
      bittemp|=((Data_Buffer[2]&1<<bitloop))?0:1;
      bittemp<<=1;
      bittemp|=((Data_Buffer[1]&1<<bitloop))?0:1;
      bittemp<<=1;
      bittemp|=((Data_Buffer[0]&1<<bitloop))?0:1;
      outputbuffer[bitloop]=(bittemp);
   }
}

void Timer_init(){   //시계 쓰레기 값 제거
   for (int i=0; i<7; i++)
   {
      Data_Buffer[i]=Segment[0];
   }
   USER_PORTC_SCAN();
}

void Hour_up()
{
   if (hour_up_key)
   {
      if (hourup_Flag==0)
      {
         hourup_Flag=1;
         if (hour_count>=12)//시단위 십의 자리
         {
            hour_count=1;
            if (AM_PM==1)
            {
               AM_PM=0;
            }
            else
            {
               AM_PM=1;
            }
         }
         else{
            hour_count++;
         }
      }
   }
   else
   {
      hourup_Flag=0;
   }
}

void Hour_down(){
   if (hour_down_key)
   {
      if (hourdown_Flag==0)
      {
         hourdown_Flag=1;
         if (hour_count<=1)//시단위 십의 자리
         {
            hour_count=12;
            if (AM_PM==1)
            {
               AM_PM=0;
            }
            else
            {
               AM_PM=1;
            }
         }
         else{
            hour_count--;
         }
      }
   }
   else
   {
      hourdown_Flag=0;
   }
}

void Min_up()
{
   if (min_up_key)
   {
      if (minup_Flag==0)
      {
         minup_Flag=1;
         if (min_count>59)
         {
            min_count=0;
            hour_count++;
            if (hour_count>=12)//시단위 십의 자리
            {
               if (AM_PM==1)
               {
                  AM_PM=0;
               }
               else
               {
                  AM_PM=1;
               }
            }
            if (hour_count>12)
            {
               hour_count=1;
            }
         }
         else
         {
            min_count++;
         }
      }
   }
   else
   {
      minup_Flag=0;
   }
}


void Min_Down()
{
   if (min_down_key)
   {
      if (mindown_Flag==0)
      {
         mindown_Flag=1;
         if (min_count<=0)
         {
            min_count=59;
            hour_count--;
            if (hour_count<=0)//시단위 십의 자리
            {
               if (AM_PM==1)
               {
                  AM_PM=0;
               }
               else
               {
                  AM_PM=1;
               }
            }
            if (hour_count<=0)
            {
               hour_count=12;
            }
         }
         else
         {
            min_count--;
         }
      }
   }
   else
   {
      mindown_Flag=0;
   }
}

void User_Time_Reset(){   //시계 reset
   hour_count=0, min_count=0, sec_count=0;
}

void Time_Buffer_Display(){   //값을 임시저장버퍼에 저장
   Data_Buffer[6]=am_pm_data[AM_PM];
   Data_Buffer[5]=Segment[sec_count%10];
   Data_Buffer[4]=Segment[sec_count/10];
   Data_Buffer[3]=Segment[min_count%10];
   Data_Buffer[2]=Segment[min_count/10];
   Data_Buffer[1]=Segment[hour_count%10];
   Data_Buffer[0]=Segment[hour_count/10];
}



void Key_Switch()
{
   if (startkey==set) //portB의 6번째 입력값을 알고 싶은 경우(key_value1)
   {
      Time_Change=1;
   }else if (stopkey==set)
   {
      Time_Change=0;
      
   }else if (resetkey==set) //portB의 7번째 입력값을 알고 싶은 경우(key_value1)
   {
      User_Time_Reset();
   }
}



int main(void)
{
   Init_Port();         //포트 초기화
   time_0();            //timer0 설정
   time_1();            //timer1 설정
   Timer_init();         //time 초기화
   
   Time_Change=1;
   while (1)
   {
      User_LED();
      time_count();
      
      Switch_ScanKey();
      Key_Switch();
      
      Hour_up();
      Hour_down();
      
      Min_up();
      Min_Down();
      
      Time_Buffer_Display();
      USER_PORTC_SCAN();
   }
}