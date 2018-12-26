#include "3664.h"
#include "wait.h"
#include "libsCore.h"

/*
 * LCD �˥��ޥ�ɤ�ǡ����������Ϥ�����ܥ롼����/�ǡ���
 */

void lcd_init(void);
static char lcd_initialized=FALSE;
static char stat;

static void outLcdC(char x) {

        /*LCD�Υ��˥���饤�������å�*/
        if(lcd_initialized==FALSE){
           lcd_init();
        }

	PCR5 |= 0x0f;				/* ���ϥӥåȻ��� */
	PDR1 &= ~0x07;
	PDR5 = (x & 0x0f) | (PDR5 & ~0x0f);
	PDR1 = 0x04 | (PDR1 & ~0x07);		/* �񤭹��� */
	PDR1 &= ~0x07;
//	PCR5 &= ~0x0f;				/* ���ӥå����Ϥ� */
}

static void outLcdD(char x) {
	PCR5 |= 0x0f;				/* ���ϥӥåȻ��� */
	PDR1 = 0x01 | (PDR1 & ~0x07);
	PDR5 = (x & 0x0f) | (PDR5 & ~0x0f);
	PDR1 = 0x05 | (PDR1 & ~0x07);		/* �񤭹��� */
	PDR1 = 0x01 | (PDR1 & ~0x07);
	PDR1 &= ~0x07;
//	PCR5 &= ~0x0f;				/* ���ӥå����Ϥ� */
}

static unsigned char inLcdC(void) {
	unsigned char x;
	PCR5 &= ~0x0f;				/* ���ӥå����Ϥ� */
	PDR1 = 0x02 | (PDR1 & ~0x07);
	PDR1 = 0x06 | (PDR1 & ~0x07);
	x = PDR5 & 0x0f;
	PDR1 = 0x02 | (PDR1 & ~0x07);		/* ���4�ӥå��ɤ߹��� */
	PDR1 = 0x06 | (PDR1 & ~0x07);
	x = (x << 4) | (PDR5 & 0x0f);
	PDR1 = 0x02 | (PDR1 & ~0x07);		/* ����4�ӥå��ɤ߹��� */
	PDR1 &= ~0x07;
	return x;
}

/* LCD �� BUSY �Ԥ� */
static void waitLcd(void) {
	while (inLcdC()&0x80);	/* Busy �Υ����å� */
}

static void writeLcdC(unsigned char x) {
	waitLcd();
	outLcdC(x>>4);
	outLcdC(x);
}

static void writeLcdD(unsigned char x) {
	waitLcd();
	outLcdD(x>>4);
	outLcdD(x);
}

/* LCD�ν������Ԥ��ޤ���
 *   ������ˡ��LCD�����Ѥ�������ɬ���ƤӽФ��ޤ���
 *   #��������LCD��Ϣ�δؿ���ƤӽФ�����
 *   #outLcdC()�ؿ����ƤӽФ����Τǡ�
 *   #�����ǡ�������Ѥ��ɤ���Ƚ�ꤷ�ơ�̤������ʤ�С�
 *   #���δؿ���ƤӽФ����Ȥ��ǽ��
 */
void lcd_init(void) {
	int i;

	lcd_initialized=TRUE;

	for (i=0; i<100; i++)		/* �ѥ���󤫤� 15ms �ʾ��Ԥ� */
		wait1m();

	outLcdC(0x03);			/* ������� */
	for (i=0; i<5; i++)		/* 4.1ms �ʾ��Ԥ� */
		wait1m();

	outLcdC(0x03);			/* ������� */
	wait1m();			/* 100us �ʾ��Ԥ� */

	outLcdC(0x03);			/* ������� */

	/* �������� Busy �Υ����å����Ǥ��� */
	while ((inLcdC()&0x80)==0x80);
	outLcdC(0x02);			/* �ե��󥯥���󥻥å� */

	/* �������� 4bit ư�� */
	writeLcdC(0x28);		/* ɽ���⡼�� */
	writeLcdC(0x08);		/* ɽ������   */
	writeLcdC(0x01);		/* ɽ�����ꥢ */
	writeLcdC(0x06);		/* ����ȥ꡼�⡼�ɥ��å� */
	writeLcdC(0x80);		/* ���ɥ쥹���å� */

	stat = 0x0f;			/* ɽ��ON,��������ON,�֥��ON */
	writeLcdC(stat);		/* �վ���ɽ���⡼������ */
}

/* LCD��ɽ��ON/OFF���ڤ��ؤ��ޤ���
 *   ������ˡ��LCD����Ѥ���Ȥ��Ȥ��ʤ��Ȥ��ˤ��줾��ƤӽФ��ޤ���
 *   ��ջ��ࡧ���ʥ����Ϥǲ����񹳤��ͤ��ɤ߼��Ȥ���OFF�ˤ��Ƥ���������
 *   ���������������줿ON/OFF����Ѥ��ޤ���
 */
void lcd_enable(int x) {
	stat = (stat & ~0x04) | ((x & 0x01)<<2);
	writeLcdC(stat);
}

/*
 * LCD�Υ�������ON/OFF���ڤ��ؤ��ޤ���
 *   ������ˡ�������������Ѥ���Ȥ��Ȥ��ʤ��Ȥ��ˤ��줾��ƤӽФ��ޤ���
 *   ���������������줿ON/OFF����Ѥ��ޤ���
 */
void lcd_cursor(int x) {
	stat = (stat & ~0x02) | ((x & 0x01)<<1);
	writeLcdC(stat);
}

/*
 * LCD�Υ�������֥�󥯡����ǡ�ON/OFF���ڤ��ؤ��ޤ���
 *   ������ˡ�����Ǥ���Ѥ���Ȥ��Ȥ��ʤ��Ȥ��ˤ��줾��ƤӽФ��ޤ���
 *   ���������������줿ON/OFF����Ѥ��ޤ���
 */
void lcd_blink(int x) {
	stat = (stat & ~0x01) | (x & 0x01);
	writeLcdC(stat);
}

/*
 * LCD�λ��ꤵ�줿��ɸ��ʸ������Ϥ��ޤ���
 *   ������ˡ����ɸ��ʸ�����ʸ�������ɡˤ���Ѥ���LCD��ʸ�������Ϥ��ޤ���
 *   ��ջ��ࡧȾ�ѥ��ʤ�ASCIIʸ���Τ߻��ѤǤ��ޤ���
 *   ������
 *     x: x��ɸ��0��15�ޤǤ�16����饯����
 *     y: y��ɸ��0��1�ޤǤ�2�ԡ�
 *     s: ʸ����

 */
void lcd_putstr(int x, int y, char *s) {
	char c;
	int a = x;			/* A �� DDRAM �Υ��ɥ쥹        */
	if (y!=0) a += 0x40;		/* �����ܤΥ��ɥ쥹�� 0x40 ���� */
	writeLcdC(0x80 | a);		/* ���ɥ쥹�򥻥åȤ���         */
	while ((c=*s++)!=0) {		/* NULLʸ�������ޤǡ�ʸ������� */
		if (c!=0x8e)		/* 0x8e��EUC��Ⱦ�ѥ��ʥ��������� */
		writeLcdD(c);
	}
}

