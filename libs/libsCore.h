#ifndef _LIBSCORE_H
#define _LIBSCORE_H

#include "3664.h"

/*�����������������*/
#define FALSE 0
#define TRUE 1
#define OFF 0
#define ON  1

#define NULL ((void*)0)

/*�����쥳���ɴ�Ϣ*/
#define EI()    asm("andc.b  #0x7f,ccr")     /*�����ߵ���*/
#define DI()    asm("orc.b #0xc0,ccr")       /*�����߶ػ�*/
#define SLEEP() asm("sleep");   /*���������ޤǡ����꡼��(���������㸺!)*/

/* ����������bit������� */
#define  BIT_SET(a,b)   ((a) |=  (1<<(b)))
#define  BIT_CLR(a,b)   ((a) &= ~(1<<(b)))
#define  BIT_FLP(a,b)   ((a) ^=  (1<<(b)))

/* �������ȥ�å�����#define�λȤ������㡣define�� inline�ؿ�����Ѥ��� */
/* ����Τϡ�#define�����,����#define�ϤǤ��ʤ�����Ǥ���                */
/* ##�ϡ�define�ΰ������礹�뤿����Ѥ��Ƥ��롣                         */
/* ����##�ϡ����������äơ������Ȥ��ȡ������ɤ��ѹ���ȼ���ݼ������ɤ��� */
/* ��Τ������ɤ��˽񤫤�Ƥ���Τ�ʬ����ʤ��Ȥ����ݼ������������������ */
/* �����롣*/

#define DEFINE_PORTS(NAME,b,c) \
extern inline void    FLIP_##NAME(void){BIT_FLP( b , c);}  \
extern inline void   SET_H_##NAME(void){BIT_SET( b , c);}  \
extern inline void   SET_L_##NAME(void){BIT_CLR( b , c);}  

#define DEFINE_PORT_ACTIVE_HIGH(NAME,b,c) \
extern inline void  ENABLE_##NAME(void){BIT_SET( b , c);}  \
extern inline void DISABLE_##NAME(void){BIT_CLR( b , c);}  \
DEFINE_PORTS(NAME,b,c)

#define DEFINE_PORT_ACTIVE_LOW(NAME,b,c) \
extern inline void  ENABLE_##NAME(void){BIT_CLR( b , c);}  \
extern inline void DISABLE_##NAME(void){BIT_SET( b , c);}  \
DEFINE_PORTS(NAME,b,c)

/* �ݼ������ɤ��ʤ�Τϡ���®���Ѥ������ǽ��,ENABLE��DISABLE������ä�*/
/* ���ʤ��ä������դȻפ��դ��ơ�FLIP���ɲä�����������Ǥϡ��ѹ�̵����*/
/* ������2�Ԥ��ɲä�������ǡ�BIT_FLIP��ǽ����Ѥ��뤳�Ȥ��Ǥ���*/

/* �ǡ������ɲä���������ǽ(ENABLE��DISABLE)�Ǥʤ��ơ��Ṳ̋ǽ񤭤����� */
/* (���å���ʬ��ɽ���Ȥ���)�⤢��Τǡ�SET_L��SET_H �ε�ǽ���դ��ä��� */

/* �ǡ������ɲä�������ACTIVE_HIGH��ACTIVE_LOW�ȤΤɤ���Ȥ�Ʊ�����Ȥ� */
/* �¸�������ʬ�ϡ�DEFINE_PORTS�ǡ���ޤȤ�ˤ���(�¤ϡ����������Ȥ� */
/* �Ԥä����Ȥˤ�äơ�����ѥ����¹����˥Х������դ��ä�������Ū�ˤ�*/
/* ACTIVE_LOW��SET_H��SET_L����FLIPư��ε��ҤˤʤäƤ���)             */

/* �ְ㤤��FIX����ǽ�ɲäϡ����ϡ�1�ս�ǺѤ�褦�˥����ɤ�񤯤��ɤ�*/

/*�嵭�Υޥ���(define)�ϡ��ʲ��Τ褦�˻��Ѥ���*/
//DEFINE_PORT_ACTIVE_HIGH(MATRIX_BLANK,PDR8,0)

/*LED�γ�����ƥӥå�(�Τ�����ϡ�DEFINE_PORT_...�κ����ˤ��ΤƤ��� */
/*�����ιͤ����ϡ�led.h��led.c�ذܹԤ�����*/
//#define LED_R   (1<<7)
//#define LED_G   (1<<6)
//#define LED_B   (1<<5)


#endif

