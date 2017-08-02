 #include<reg52.h>
#include<string.h>
#include<STDIO.H>
#include<intrins.h>
#define lcd P0

sbit rs=P1^6; 
sbit rw=P1^7;
sbit en=P1^8;
sbit lcd_bsy = P0^7;

sbit mtr1 = P2^0;
sbit mtr2 = P2^1;
sbit mtr3 = P2^2;
sbit mtr4 = P2^3;

/*****************/
void init_lcd(void);
void cmd_lcd(unsigned char);
void lcd_data(unsigned char);
void display_lcd(unsigned char *);
void LCD_Busy_Chk(void);

void delay_ms(unsigned int);
int i=0;
/******************/

unsigned char buf[5];
bit flag;
bit r_flag = 0;
bit sucess = 0;

char idata buff[10];
char temp,recvbyte;
/************************************/
 void serial_intr(void) interrupt 4
{
if(TI == 1)
{
TI = 0;
flag = 1;
}
else
{
RI = 0;

//if(i < 10)
//buff[i++] = SBUF;
recvbyte=SBUF;
if(recvbyte==0x32)
{	recvbyte=0;
	cmd_lcd(0x01);
	cmd_lcd(0x80);
	display_lcd("FORWARD");
	mtr1=1;mtr2=0;mtr3=1;mtr4=0;
	delay_ms(500);
}
else if(recvbyte==0x38)
{	recvbyte=0;
	cmd_lcd(0x01);
	cmd_lcd(0x80);
	display_lcd("BACKWARD");
	mtr1=0;mtr2=1;mtr3=0;mtr4=1;
	delay_ms(500);
}
else if(recvbyte==0x34)
{	recvbyte=0;
	cmd_lcd(0x01);
	cmd_lcd(0x80);
	display_lcd("  LEFT    ");
	mtr1=1;mtr2=0;mtr3=0;mtr4=0;
	delay_ms(500);
}
else if(recvbyte==0x36)
{	recvbyte=0;
	cmd_lcd(0x01);
	cmd_lcd(0x80);
	display_lcd("   RIGHT    ");
	mtr1=0;mtr2=0;mtr3=1;mtr4=0;
	delay_ms(500);
}
else if(recvbyte==0x35)
{	recvbyte=0;
	cmd_lcd(0x01);
	cmd_lcd(0x80);
	display_lcd("  STOP     ");
	mtr1=0;mtr2=0;mtr3=0;mtr4=0;
	delay_ms(500);
}
}
}
//////////////////////////////////////////////////////////////
void print(char *str)
{
while(*str)
{
	flag = 0;
	SBUF = *str++;
	while(flag == 0);
}
}

/******************/

void main(void)
{
TMOD = 0x20;
SCON = 0x50;
TH1 = 0xFD;
TR1 = 1;
IE = 0x90;

init_lcd();
delay_ms(10);

cmd_lcd(0x01);
cmd_lcd(0x80);
display_lcd("ROBOT AUTOMATION");
cmd_lcd(0xC0);
display_lcd(" USING ANDROID");
delay_ms(500);
cmd_lcd(0x01);
cmd_lcd(0x80);
display_lcd(" APPLICATIONS ");
delay_ms(500);
print("ROBOT AUTOMATION USING ANDROID APPLICATIONS\r\n");
delay_ms(100);
mtr1=0;mtr2=0;mtr3=0;mtr4=0;
while(1);
}


void init_lcd(void)
{
cmd_lcd(0x30);
cmd_lcd(0x30);
cmd_lcd(0x38);
cmd_lcd(0x0c);
cmd_lcd(0x06);
cmd_lcd(0x01);
delay_ms(10);
}
void cmd_lcd(unsigned char c)
{
 LCD_Busy_Chk();

	lcd=c;
	rs=0;
	rw=0;
	en=1;
	_nop_();
	en=0;
}
void lcd_data(unsigned char c)
{
	LCD_Busy_Chk();
	
	lcd=c;
	rs=1;
	rw=0;
	en=1;
	_nop_();
	en=0;	
}
void LCD_Busy_Chk(void)
{
		lcd_bsy=1;
		rs=0;
		rw=1;
	while(lcd_bsy==1)
		{
		en=0;
		_nop_();
	    _nop_();
		_nop_();
	    _nop_();
		en=1;
		}
}
void display_lcd(unsigned char *s)
{
while(*s)
lcd_data(*s++);
}
void delay_ms(unsigned int i)
{
unsigned int j;
while(i-->0)
{
for(j=0;j<500;j++);
}
}


