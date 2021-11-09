/*
 * IncFile1.h
 *
 * Created: 2021-10-15 오후 3:01:26
 *  Author: USER
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_
#define set 1
#define reset 0



#define ON_minup   (PINB&0x10)&&(PORTA==0x80)    //스위치 누름

#define  startkey g_btscankey[0]==0x10? set:reset 
#define  stopkey g_btscankey[1]==0x10? set:reset   
#define  resetkey g_btscankey[2]==0x10? set:reset  

#define  min_up_key  T_SCAN1[7]
#define  min_down_key T_SCAN2[7]

#define  hour_up_key T_SCAN2[6]
#define  hour_down_key T_SCAN3[6]
  
///////////////////////////////////////////////////////////////


typedef union _Port
{
   unsigned char byt;
   
   struct 
   {
      unsigned char bit0:1;
      unsigned char bit1:1;
      unsigned char bit2:1;
      unsigned char bit3:1;
      unsigned char bit4:1;
      unsigned char bit5:1;
      unsigned char bit6:1;
      unsigned char bit7:1;
   }bit;
}PORT_SFR;



extern PORT_SFR PortB;
//extern PORT_SFR T_SCAN[8];
PORT_SFR PortB;
//PORT_SFR T_SCAN[ 8 ];

//#define MAS_DN_SW         T_SCAN[7].bit.bit4==1? set:reset
// typedef unsigned char byte;
// 
// typedef union _Port
// {
//    unsigned char byt;
//    struct
//    {
//       unsigned char bit0:1;
//       unsigned char bit1:1;
//       unsigned char bit2:1;
//       unsigned char bit3:1;
//       unsigned char bit4:1;
//       unsigned char bit5:1;
//       unsigned char bit6:1;
//       unsigned char bit7:1;
//    }bit;
// }PORT_SFR;
// 
// //extern PORT_SFR   PortB;
// 
// //byte g_btScanKeyValue[ 8 ] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//    
// //extern byte g_btScanKeyValue[8];         //Scan
// 
// //extern PORT_SFR T_SCAN[8];//= { 0, 0, 0, 0, 0, 0, 0, 0 };
//    
// // KEY_VALUE 0
// #define HEAD_SW            T_SCAN[0].bit.bit4
// #define CHEST_SW         T_SCAN[1].bit.bit4
// #define ABDOM_SW         T_SCAN[2].bit.bit4
// #define PELVI_SW         T_SCAN[3].bit.bit4
// #define STORE_SW         T_SCAN[4].bit.bit4
// #define APR_UP_SW         T_SCAN[5].bit.bit4
// #define READY_SW         T_SCAN[6].bit.bit4
// #define MAS_DN_SW         T_SCAN[7].bit.bit4
// 
// // KEY_VALUE 1
// #define LEGS_SW            T_SCAN[0].bit.bit5
// #define FOOT_SW            T_SCAN[1].bit.bit5
// #define ARM_SW            T_SCAN[2].bit.bit5
// #define HAND_SW            T_SCAN[3].bit.bit5
// #define APR_DN_SW         T_SCAN[4].bit.bit5
// #define LAMP_SW            T_SCAN[5].bit.bit5
// #define KV_DN_SW         T_SCAN[6].bit.bit5
// #define MAS_UP_SW         T_SCAN[7].bit.bit5
// 
// // KEY_VALUE 2
// #define BABY_SW            T_SCAN[0].bit.bit6
// #define SMALL_SW         T_SCAN[1].bit.bit6
// #define MEDIUM_SW         T_SCAN[2].bit.bit6
// #define LARGE_SW         T_SCAN[3].bit.bit6
// #define APR_SW            T_SCAN[4].bit.bit6
// #define XRAY_SW            T_SCAN[5].bit.bit6
// #define KV_UP_SW         T_SCAN[6].bit.bit6
// #define TABLE_SW         T_SCAN[7].bit.bit6




#endif /* INCFILE1_H_ */