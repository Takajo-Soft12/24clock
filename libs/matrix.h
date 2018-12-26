#ifndef _MATRIX_H
#define _MATRIX_H

#include "libsCore.h"
#include "3664.h"

/* ����������matrix�ط������ */

   /* #define��Ȥ��� (�¤ϡ����ιԤ�PDR8�ϡ������Ǥϡ�ɾ������ʤ�)*/
   /* #include "3664.h"�ϡ����󥯥롼�����פǡ�Ÿ���ξ��ǽ�     */
   //#define  ENABLE_MATRIX_BLANK(a)   BIT_SET(PDR8,0)
   //#define DISABLE_MATRIX_BLANK(a)  BIT_CLR(PDR8,0)

   /* inline�ؿ���Ȥ��� (���Υإå��ե�����򥳥�ѥ��뤹������� */
   /* ɾ������뤿�ᡤ#include "3664.h"�Υ��󥯥롼�ɤ�ɬ�� */
   // extern inline void  ENABLE_MATRIX_BLANK(void)������
   // extern inline void DISABLE_MATRIX_BLANK(void)������

/* util/libsCore.h�� DEFINE_PORT_ACTIVE_HIGH �ޥ����Ȥ������Ԥǽ�      */
/*���åȣ����ϡ��ɤ��ѹ������ä�����,�����ΰ�ս�����񤭴�������ɤ�    */
/*���åȣ��������˵��Ҥ���ľ��ˡ�PDR8�ʤɤ����������ѥ�����˳�ǧ��ǽ */
/*�ǥ��åȣ�: grep��񤱤Ƥ⡤���������꤬ʬ����ʤ������Ф�*/

DEFINE_PORT_ACTIVE_HIGH(MATRIX_BLANK,PDR8,0)

     //ACTIVE_LOW�Ǥ���
     //#define  ENABLE_MATRIX_LATCH(a)   BIT_CLR(PDR5,5)
     //#define DISABLE_MATRIX_LATCH(a)  BIT_SET(PDR5,5)

DEFINE_PORT_ACTIVE_LOW(MATRIX_LATCH,PDR5,5)

DEFINE_PORT_ACTIVE_HIGH(MATRIX_SIN,PDR5,4)

DEFINE_PORT_ACTIVE_HIGH(MATRIX_SCLK,PDR8,1)

extern inline void SELECT_MATRIX_COLUMN(int i){
  PDR7 = (i<<4);  /*�ϡ���Ū�˺�û�����ˡ(P7�ϡ�P76��P74�ޤǤ���̵��)*/
}

extern unsigned const char matrix_fonts[];

extern void matrix_font_debug_out_sample(unsigned int matrix_led_pattern[]);

#endif
