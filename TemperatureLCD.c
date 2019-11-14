#include<reg51.h>

sbit rs=P3^2; 
sbit rw=P3^3;
sbit e=P3^4;
sbit ht=P1^4;


void delay(int k)
{
int i,j;
	for(i=1;i<k;i++)
	  for(j=0;j<130;j++);
}

void delay30us() /* Timer0 delay function */
{
	TMOD = 0x01;			/* Timer0 mode1 (16-bit timer mode) */
	TH0 = 0xFF;				/* Load higher 8-bit in TH0 */
	TL0 = 0xF1;				/* Load lower 8-bit in TL0 */
	TR0 = 1;					/* Start timer0 */
	while(TF0 == 0);	/* Wait until timer0 flag set */
	TR0 = 0;					/* Stop timer0 */
	TF0 = 0;					/* Clear timer0 flag */
}

void delay20ms()		/* Timer0 delay function */
{
	TMOD = 0x01;
	TH0 = 0xB8;				/* Load higher 8-bit in TH0 */
	TL0 = 0x0C;				/* Load lower 8-bit in TL0 */
	TR0 = 1;					/* Start timer0 */
	while(TF0 == 0);	/* Wait until timer0 flag set */
	TR0 = 0;					/* Stop timer0 */
	TF0 = 0;					/* Clear timer0 flag */
}


void lcdstart()
{
e = 1;
delay(50);	
}

void lcdcmd(char cmd)
{
P2 = cmd;
rs=0;
rw=0;
e=1;
delay(40);	
e=0;
}

void lcddata(int value)
{
P2 = value;
rs=1;
rw=0;
e=1;
delay(20);	
e=0;	
}

void text(char *text)
{
int i=0;
while(text[i]!='\0')
{
lcddata(text[i]);
i++;	
}	
}

void tempint()
{
ht = 0;
	delay20ms();
	ht = 1;
}

void tempresp()
{

  while(ht == 1);
	while(ht == 0);
  while(ht == 1);
}


int receivedata()
{  
	int i,d=0;
	for(i=0;i<8;i++)
	{
		while(ht == 0);
		delay30us();
		
		if (ht == 1)
		 d = (d<<1)|(0x01);
		else
		 d = (d<<1);
		
	while(ht == 1);	 
	}
	return d;
}


void main()
{
	int humint, humdec, temint, temdec, check, humones, humtens, temones, temtens;
	
	lcdstart();
	lcdcmd(0x38);
	delay(60);
	lcdcmd(0x0C);
	delay(60);
	lcdcmd(0x01);
	delay(60);
	lcdcmd(0x80);
	delay(60);
	
	text("Initializing...");
	delay(50);
	
	lcdcmd(0x01);
	delay(60);
	
while(1)
{	
tempint();
tempresp();

	
  humint=receivedata();
	humdec=receivedata();
	temint=receivedata();
	temdec=receivedata();
	check=receivedata();
	
	if((humint + humdec + temint + temdec) != check)
	{	
	text("Error");
	}
	
	else
	{
		
    humones = (humint%10)+48;
		humtens = (humint /10)+48;
		
		temones = (temint%10)+48;
		temtens = (temint/10)+48;
		
		text("Humidity:    ");
		lcddata(humtens);
    lcddata(humones);
    text("%");

    lcdcmd(0xC0);
    delay(60); 		
		
		text("Temperature: ");
		lcddata(temtens);
    lcddata(temones);
    text("C");
		
		
		delay(1000);
		
	}
	
}
}