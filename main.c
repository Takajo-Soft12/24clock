#include "libs.h"       /* ����my3664h�����Ƥϡ�libs/libs.h�����礷�� */

volatile int tma_flag=FALSE;
volatile int sec_flag=FALSE;
volatile int tmv_flag=FALSE;
volatile int sec=0;

volatile int tempo_flag=FALSE;
int tempo_compare=0;




static unsigned int matrix_led_pattern[8]=
  //{0x007e,0x0011,0x0011,0x0011,0x007e,0x7f00,0x4900,0x4900};
{0x7e7e,0x1111,0x1111,0x0011,0x007e,0x7f00,0x4900,0x4900};
/*��0��7�Υǡ���(�ܺ٤ϡ����Υꥹ�Ȥ��ɤ�)*/

#define DEBUG          /* �ǥХå���ϡ�������Ƥ��� */

/* int_timera() �� int_imterv() �γ���롼����(�ܥȥ�ϡ��դν���) */

#pragma interrupt                   /*��������롼����Ǥ��뤳�Ȥλ���*/
void int_timera(void){              
        volatile static int count=0;
	BIT_CLR(IRR1,6);      /* ����Υ�����A�����ߥե饰���ꥢ */
	tma_flag = TRUE;
	//	EI();         /* ɬ�פ˱����� EI()��¹�  */
	/*32��ƤӽФ��줿�顤ifʸ���椬�¹Ԥ���ơ�sec_flag��ͭ���ˤʤ�*/
	if(++count >= 32){
	      count=0;
	      sec_flag = TRUE;
	      /* 1�����䤹 */
	      sec++;
	      
	}

#ifdef DEBUG
       { /*�ܥȥ�ϡ��ս�����;�ϥ����å��ѥ�����*/
	 volatile long int loop;
	 for(loop=0;loop<1;loop++); 
       }

	if((IRR1 & 0x40)!=0){ /* �����ߥե饰�ξ��֥����å� */
	 ENABLE_LED_GREEN();  /* �⤷�����ߥե饰��Ω�äƤ����п�LED���� */
       }
#endif
}

#pragma interrupt
void int_timerv(void){         /* ��1[msec] ���Ȥ˸ƤӽФ����褦�ˤ���*/
       static unsigned int column=0;
       unsigned int i,p;
       /*���ԡ����������*/

       //´�ȸ���Ǻ������Ƥ��줿�����ɤ�����ѡ�
       //tempo���ѹ��Ǥ���褦�˲�¤ by T.nitta
       //������tempo��tempo_compare��const�Ǥʤ����ѿ��ˤ��롣
       //â�������η׻���ܥȥ�ϡ��դ����¹Ԥ����,�������Ť��Τ�
       //�ȥåץϡ��դǷ׻���Ԥ���
       //       const int tempo=120;
       //       const int tempo_compare = 1000/(tempo * 16 / 60);
       //       const int tempo_compare = 3735 / tempo ;
       static int tempo_count=0;

       TCSRV &= ~0xe0;         /* ������V�γ����ߥե饰���ꥢ*/
       tmv_flag=TRUE;
       /*int_timerv�ϡ�����ػߤΤޤ�,����ư��*/

       /* 1ms ���Ȥ˥ϡ��ɥꥢ�륿�����ư�����������(matrix_led����)*/

       column=(column+1) & 0x0007; /* column=(column++)&0x0007�ϡ�NG�Ǥ���  */ 
       ENABLE_MATRIX_LATCH();      /*��å���ͭ���ˤ�������΢��D-FF�򥻥å� */

       /*16bit (1��ʬ) �򥷥ꥢ��ž��*/
       for(p=matrix_led_pattern[column],i=0;i<16;i++,p<<=1){        
	 if((p&0x8000)==0){
	   DISABLE_MATRIX_SIN();
	 }else{
	   ENABLE_MATRIX_SIN();
	 }
	 SET_H_MATRIX_SCLK();SET_L_MATRIX_SCLK();   /* pos���å����*/
       }

       ENABLE_MATRIX_BLANK();   /*�ʲ�����ȴ�������ѹ���ɽ�������ʤ�*/
       DISABLE_MATRIX_LATCH();  ENABLE_MATRIX_LATCH();  /*�����ȴ��*/
       SELECT_MATRIX_COLUMN(column);               /*j���ܤ�����ѹ�*/
       DISABLE_MATRIX_BLANK();                          /*����������*/


       /*���ԡ�����Ϣ����*/
       if(snd_check()==TRUE){
	 if(tempo_compare < ++tempo_count){
	   tempo_count = 0;
	   tempo_flag = TRUE;
	 }
       }


#ifdef DEBUG
       { /*�ܥȥ�ϡ��ս�����;�ϥ����å��ѥ�����*/
	 volatile int loop;
	 for(loop=0;loop<10;loop++);
	 /*�Ȥ����ﲼ�Ǥε�ư(â�������θ�˥������ϤΥ����ɤ��ɲ�)*/
	 /*loop<461 :10ʬ���٤Ǥ�,��þ���ǧ�Ǥ���(â�������ڲ����1�����)*/
       }

       if((TCSRV & 0xe0)!=0){ /* �����ߥե饰�ξ��֥����å� */
	 ENABLE_LED_RED();    /* �⤷�����ߥե饰��Ω�äƤ����ֿ�LED����*/
       }
#endif
}

/*�����޴ط���������*/
void timer_init(void){
	/*������A������(1/32[sec]���ȤΥ����޳����ߤ�����) */
	TMA = 0x9f; /* ������A TCA�ȥץꥹ������W�Υꥻ�å�(2011/12/15�ɲ�) */
	TMA = 0x9b; /* ������A 1/32[sec]��γ��������                      */
	/* 0x9b => 1001 1011 ������ (����ϡ����Υꥹ�Ȥ򸫤�)            */

	IENR1 |= 0x40; 	     /* ������A�γ�����������ͭ���ˤ���*/

	/*������V������(�ޥ˥奢��򸫤�,�ʲ���3�Ԥα��դ�񤭴�����)*/
	TCORA = 124;    /*�����ߴֳ֤���1ms�ˤʤ�褦�ˤ���(125ʬ��)*/
	TCRV0 = 0x4b;  /*����ڥ��ޥå�A�ǳ����ߡ����κݤ˥����󥿥��ꥢ*/
	TCRV1 = 0x01;  /*����å��ϡ������Φ���128ʬ��(16MHz/128)���Ѥ���*/

	/*timerv��ɬ��0���饹�����Ȥ����� */
	TCNTV = 0x00;          /* ������V�����������󥿤�0���鳫��*/
	BIT_CLR(IRR1,6);       /* ������A�γ����ߥե饰���ꥢ   */
	TCSRV &= ~0xe0;        /* ������V�γ����ߥե饰���ꥢ   */

	/* ������W������(���ԡ�����)*/
	TCRW = 0xbc;
}

/* main() �ʤɤγ����ߥ롼����ʳ��ν����ϡ��ȥåץϡ��դν���*/

/* User Interface �Υ��ơ��ȥޥ��� (�����������)*/

#include "modes.h"
extern void do_mode0(UI_DATA *ui_data);
extern void do_mode10(UI_DATA* ui_data);
extern void do_mode20(UI_DATA* ui_data);

UI_DATA* ui(char sw){ /* �ߡ��귿���ࡼ�������ɤä��Ǽ������ɤ��ͤ��� */
  static UI_DATA ui_data={MODE_0,MODE_0,};
  int prev_mode;

  ui_data.sw=(sw & 0x9f); /*ǰ�Τ���ˡ�b6,b5��0�ˤ��Ƥ���*/
  prev_mode=ui_data.mode;

  switch(ui_data.mode){
  case MODE_0:
    do_mode0(&ui_data);
    break;
  case MODE_1:
    do_mode1(&ui_data);
    break;
  case mode10:
    do_mode10(&ui_data);
  case MODE_2:
    do_mode20(&ui_data);
    break;
  default:
    break;
 }

  ui_data.prev_mode=prev_mode;

  return &ui_data;
}

#include "mode0.c"


/*����ɽ���Υ��르�ꥺ��ΰ���*/
void show_sec(void){
  char data[6];
  int h,s;
  int sec_hold=sec; /* �ͤ��������Ƥ������ˡ�sec���Ѥ��ȷ��ʤΤǡ�   */
                    /* �����ǡ�sec���ͤ���ޤ��롣sec���ͤϡ��ܥȥ�ϡ���*/
                    /* ���Ѳ������Ƥ���Τǡ����������Ƚ�������Ѳ����롣*/

  s=sec_hold % 60;
  h=(sec_hold / 60); /* �����ǡ�h���ͤη������ϡ����ڤ��Ƥ��ʤ�����͡�*/
                     /* �ҥ�Ȥϡ���sec�ϡ�int����*/

  data[0]='0'+h/10;
  data[1]='0'+h%10;
  data[2]=':';
  data[3]='0'+ s/10;
  data[4]='0'+ s%10;
  data[5]='\0';

  lcd_putstr(16-5,1,data);
}

#include "mode10.c"


void do_mode20(UI_DATA* ud){

  if(ud->prev_mode!=ud->mode || sec_flag==TRUE){ 
    /* ¾�Υ⡼�����ܤ������˼¹� �⤷���ϡ�1�ä��Ȥ�ɽ��*/
    /*ɬ�פʤ顤���餫�Υ⡼�ɤν��������*/
    lcd_clear();  //0123456789ABCDEF
    lcd_putstr(0,0,"MODE2:sec�� �ˎ�������"); /*�⡼��2�ν��ɽ��*/
    show_sec();
    sec_flag=FALSE;
  }

  /*�⡼��2�ϡ�����ܥ��󤬲����줿�顤MODE0�����*/
  switch(ud->sw){    /*�⡼����ǤΥ������������*/
  case KEY_LONG_C:   /* ���������Ĺ���� */
    ud->mode=MODE_0; /* ���ϡ��⡼��0����� */
    break;
  }
}



int main(void){
         UI_DATA* ui_data=NULL;
	unsigned char sw=KEY_NONE;

	//	char test[]="hoge";

	DI();           /* ǰ�Τ����®�䤫��DI() */ 
	io_init();      /* I/O�ݡ��Ȥν�����롼����(matrix,lcd�����ޤ�) */
	lcd_init();     /* LCD�ν�����롼�����¹� */

	/*LCD�ν�����֤ϡ���������OFF��,�֥�󥯤�OFF�Ȳ��ˤ��Ƥ���*/
	lcd_cursor(OFF);
	lcd_blink(OFF); 

	/*���ԡ����ѤΥƥ�ݷ׻�(�Ȥꤢ�����ν���ͤ�����)*/
	{
	  int tempo=120;
	  tempo_compare = 3735 / tempo; /* 1000/(tempo * 16 / 60) ��Ÿ�� */
	}

	timer_init();   /* �����ޤν�������¹�(EI��ľ���˼¹�) */
        EI();	        /* �����ߤ����  */

	//Ⱦ�ѥ������ʰ���:���ϤǤ��ʤ��ʤ顤�������饳�ԥڤ��衣
	//���������� ���������� ���������� �����Î� �ŎƎǎȎ� �ʎˎ͎̎� �ώЎюҎ� �׎؎َڎ� �ԎՎ� �܎��� �ގ� �� ����������
	
	//�ǽ�ϡ��⡼��0����¹Ԥ����ꡣ
	lcd_putstr(0,0,"MODE0->MODE0");

	for(;;){	  /* �Ȥ߹��ߥ����ƥ�ϡ�����Ū�ˤ�̵�¥롼�פǼ¹� */


	  /* �ʲ���ifʸ����ϡ�¿ʬ��1/32�ä��Ȥ˽�����Ԥ�*/
	  if(tma_flag == TRUE){ 
	    sw = sw_mng();       /* �����å������ϥ����å� libs/key.c */
	    ui_data=ui(sw);           /* �桼�����󥿥ե������μ¹� */

	    if(ui_data->mode==MODE_1){
	      /* MODE1���ä��顤�������ϤΥǥХå������ȤΥǥХå����� */
	      key_debug_out_sample(); 
	    }

	    tma_flag=FALSE;
	  }

	  /* �ʲ��ν����ϡ�1ms���Ȥ˹Ԥ� */
	  if(tmv_flag==TRUE){

#if 0
	    /*���ԡ����Υ����쥯�ȥ���ȥ���ϡ�beta2����*/
	    /*�Ǥ��ʤ��褦�ˤʤ�ޤ�����*/
	    /*���ԡ����ѤΥݡ��Ȥϡ�������W�Ȥ��������ޤ���*/
	    /*�����޽��Ϥ�ľ�ܽ��Ϥ��뤳�Ȥˤʤ�ޤ�*/
	    /*���ԡ����Ѥδؿ� snd_mng(),snd_play(),snd_stop()�ʤɤ�*/
	    /*���Ѥ��뤳��*/

	    /*�Ȥꤢ�������⡼��1�λ��ˡ���Υ�����û�������줿�顤   */
	    /*(���Τˤϡ�û������Υ���줿����)��500Hz�β���û���Ф�   */
	    /*�褦�ʥ��ԡ����Υ���ץ륳���ɡ�                        */
	    /*���ޡ��Ȥ˥��ԡ������Ĥ餹�Τ��ä��顤�ɤ����ǡ� �ե饰 */
	    /*��Ω�ä��顤 FLIP_SPEAKER_DIRECT_CONTROL();��¹Ԥ���   */
	    /*�褦�ˤ���Τ����ʤ������äȡ�¿ʬ��                  */
	    if(ui_data!=NULL && ui_data->mode==MODE_1 && sw==KEY_SHORT_U){
		FLIP_SPEAKER_DIRECT_CONTROL();
	    }
#endif
	    tma_flag=FALSE;
	  }

	  if(tempo_flag){
	    snd_mng();
	    tempo_flag = FALSE;
	  }


	  SLEEP(); /*���������ޤǡ����꡼�פ�����(���������㸺!)*/
	}
	return 0;  /*���ιԤϼ¹Ԥ���ʤ������ٹ��Ф��ʤ����ޤ��ʤ�*/
}

/*************************************************************
 (1)  Ʊ�ͤλؼ��Ǿ�ά
 (2)  afp://iemac.ie.tokuyama.ac.jp/i0/nitta/2014exp4/9_2014beta_sample/ ��
     (�ʲ���Ʊ�ͤǾ�ά)
 (3) Ʊ�ͤλؼ��Ǿ�ά
 (4)��(9) ����Ʊ�͡�
 ************************************************************/

