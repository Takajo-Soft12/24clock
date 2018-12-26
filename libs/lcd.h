#ifndef _LCD_H
#define _LCD_H
/*
 * �ե�����̾��lcd.c
 * ���ӡ�LCD�������ñ�˹Ԥ���ǽ����¢���롣
 */
extern void lcd_init(void);           /* LCD�ν������Ԥ��ޤ��� */

extern void lcd_enable(int);          /* LCD��ɽ��ON/OFF���ڤ��ؤ��ޤ��� */

extern void lcd_cursor(int);          /* LCD�Υ�������ON/OFF���ڤ��ؤ��ޤ���*/

extern void lcd_blink(int);           /* LCD�Υ�������֥�󥯡����ǡ�ON/OFF*/
                                      /* ���ڤ��ؤ��ޤ��� */

extern void lcd_putstr(int, int, char *);     /* LCD�λ��ꤵ�줿��ɸ��ʸ����*/
					      /*���Ϥ��ޤ��� */


#endif
