#include<reg51.h>
#include<intrins.h>

#define lcd_data P2

sfr16 DPTR=0x82;

sbit trig=P3^5;
sbit echo=P3^2;

unsigned int range=0;

sbit rs=P0^0;
sbit rw=P0^1;
sbit en=P0^2;

//sbit On=P0^6;

sbit m1f=P0^3;             // in1 pin of motor1
sbit m1b=P0^4;             // in2 pin of motor1
sbit m2f=P0^5;             // in1 pin of motor2
sbit m2b=P0^6;             // in2 pin of motor2

void delay(unsigned int)  ;    

void lcd_init();
void cmd(unsigned char a);
void dat(unsigned char b);
void show(unsigned char *s);
void lcd_delay();

void lcd_init()
{
  cmd(0x38);
  cmd(0x0e);
  cmd(0x06);
  cmd(0x0c);
  cmd(0x80);
}

void cmd(unsigned char a)
{
  lcd_data=a;
  rs=0;
  rw=0;
  en=1;
  lcd_delay();
  en=0;
}

void dat(unsigned char b)
{
  lcd_data=b;
  rs=1;
  rw=0;
  en=1;
  lcd_delay();
  en=0;
}

void show(unsigned char *s)
{
  while(*s) {
    dat(*s++);
  }
}

void lcd_delay()
{
  unsigned int i;
  for(i=0;i<=1000;i++);
}


void send_pulse(void) 
{
  TH0=0x00;TL0=0x00; 
   trig=1;
   nop();nop();nop();nop();nop();
   nop();nop();nop();nop();nop();
   trig=0;
}

unsigned char ultrasonic()
{
  unsigned char get;
  send_pulse();
  while(!echo);
  while(echo);
  DPH=TH0;
  DPL=TL0;
  TH0=TL0=0xff;
  if(DPTR<38000)
    get=DPTR/59;
  else
    get=0;
  return get;
}


void main()
{
  TMOD=0x09;
  TH0=TL0=0;
  TR0=1;
  lcd_init();
  show("DIS");
  P3|=(1<<2);
  while(1) {
    cmd(0x84);
    range=ultrasonic();
    dat((range/100)+48);
    dat(((range/10)%10)+48);
    dat((range%10)+48);
		//delay(100);
		
		if(range>=20)
		{
		//On = 0;
			m1f=1;
      m1b=0;
      m2f=1;
      m2b=0;
			
		}
		 else 
		 {
		 //On = 1;
			
			m1f=1;
      m1b=0;
      m2f=0;
      m2b=1;
		 }
		 
  }



}