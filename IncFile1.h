/*
 * IncFile1.h
 *
 * Created: 2021-10-15 오후 3:01:26
 *  Author: USER
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_

int gtimer1_cnt=0; //타이머 1 인터럽트
int hour_count=0, min_count=0, sec_count=0;
int hour1=0,hour2=0,min1=0,min2=0,sec1=0,sec2=0; //hour1(시의 일의자리), hour2(시의 십의자리), min1(분의 일의 자리), min2(분의 십의 자리), sec1(초의 일의 자리), sec2(초의 십의 자리)
int data=0;
char Time_STOP=0;
int min_delay=0,hour_delay=0,temp_delay=0;
unsigned char flag=0,mode=0;
unsigned int cnt=0;

unsigned char segment_A_P[]={0x77,0x73};
unsigned char USER_Segment_Table[]={0x3f,0x06,0x5B,0x4f,0x66,0x6d,0x7d,0x27,0x7F,0x6f}; //세그먼트 숫자 비트
unsigned char USER_Buffer[8]={0,0,0,0,0,0,0,0}; //스캔 버퍼
unsigned char USER_Output_Buffer[8]={0,0,0,0,0,0,0,0}; //출력 버퍼


unsigned char l_btPINB_0x10 = 0, l_btPINB_0x20=0, l_btPINB_0x40=0; //스위치 누른 값 저장하는 변수
unsigned char l_btPINBTempOld[8]; // 스위치 누른 값이랑 비교하는 배열
unsigned char g_btscankey[8];// 눌렀다고 확신이 드는 스위치 값 저장하는 배열

#define set 1
#define  startkey g_btscankey[0]==0x10? 1:0 
#define  stopkey g_btscankey[1]==0x10? 1:0  
#define  resetkey g_btscankey[2]==0x10? 1:0 

#define  min_up_key g_btscankey[3]==0x10? 1:0 
#define  min_down_key g_btscankey[5]==0x20? 1:0

#define  hour_up_key g_btscankey[4]==0x20? 1:0
#define  hour_down_key g_btscankey[6]==0x40? 1:0



#endif /* INCFILE1_H_ */