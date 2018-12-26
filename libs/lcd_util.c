#include "lcd_util.h"
#include "lcd.h"

/*
 * LCD�λ��ꤵ�줿��ɸ����������Ϥ���ؿ�
 *   ������ˡ����ɸ����������Ѥ���LCD����������Ϥ��ޤ���
 *   ��ջ��ࡧɽ���������ͤϡ�integer�������������ѤǤ��ޤ���
 *   ������
 *     x: x��ɸ��0��15�ޤǤ�16����饯����
 *     y: y��ɸ��0��1�ޤǤ�2�ԡ�
 *     l: ɽ��������
 *     v: ɽ������������
 */
void lcd_putdec(int x, int y, int l, int v) {

  char buf[l+1];
  int  i;

  for (i=l-1; i>=0 && v!=0; i--) {
    buf[i] =(v % 10) + '0';
    v /= 10;
  }

  for (; i>=0; i--) {
    buf[i] = ' ';
  }
  buf[l] = '\0';
  lcd_putstr(x, y, buf);

}

void lcd_putudec(int x, int y, int l, unsigned int v) {

  char buf[l+1];
  int  i;

  for (i=l-1; i>=0 && v!=0; i--) {
    buf[i] =(v % 10) + '0';
    v /= 10;
  }

  for (; i>=0; i--) {
    buf[i] = ' ';
  }
  buf[l] = '\0';
  lcd_putstr(x, y, buf);

}


void lcd_key_debug_out(int x,int y,unsigned char key_data)
{
  char key_state[6];

  /*���ͤȤ��ơ�LCD�˥����ξ��֤�ǥХå�ɽ��*/
  key_state[0]=((key_data & (1<<7) )==0) ? '*':'U' ; /*Up*/
  key_state[1]=((key_data & (1<<6) )==0) ? '*':'D' ; /*Down*/
  key_state[2]=((key_data & (1<<5) )==0) ? '*':'L' ; /*Left*/
  key_state[3]=((key_data & (1<<4) )==0) ? '*':'R' ; /*Right*/
  key_state[4]=((key_data & (1<<3) )==0) ? '*':'C' ; /*Center*/
  key_state[5]='\0';
  lcd_putstr(x,y,key_state);
}


void lcd_clear(void){
                //0123456789ABCDEF
  lcd_putstr(0,0,"                ");
  lcd_putstr(0,1,"                ");
}
