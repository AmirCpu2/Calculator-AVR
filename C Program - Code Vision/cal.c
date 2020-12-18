//--------------تعریف کتابخانه های مورد نیاز---------------------
#include <mega32.h> // کتابخانه پردازنده
#include <stdio.h> // کتابخانه عمومی زبان c
#include <alcd.h>// کتابخانه نمایش گر

//-------------تعریف متغییر های مورد نیاز---------------------
int row[4]={0xfe,0xfd,0xfb,0xf7}, // بافر حالت برای چک کردن سطر ها ( در هر حالت یک سطر روشن میشو تا متوجه شویم کدام دکمه زده شده است).
num1=0, // عملوند اول
num2=0; // عملوند دوم
char buffer[16]; // بافر ذخیره سازی حالت نمایشگر
bit flag2=0; // فلگ برای چک کردن عملگر ها
bit flag=0; // فلگ برای چک کردن عملگر ها

//-------------تعریف متد های مورد نیاز---------------------
void setNum(int n);
void sum();
void mul();
void dev();
void sub();

//-----------------متد اجرایی----------------
void main(void)
{
	//----------------تعریف مقدار محلی-------------
	int i=0, //شمارنده
	mode=0;// وضعیت عملگر انتخابی


	//----------------تعیین ورودی خروجی کانال ها و مقدار دهی اولیه-----------------
	// Port A 
	// Bit7=ورودی Bit6=ورودی Bit5=ورودی Bit4=ورودی Bit3=ورودی Bit2=ورودی Bit1=ورودی Bit0=ورودی 
	// State7=یک منطقی State6=یک منطقی State5=یک منطقی State4=یک منطقی State3=یک منطقی State2=یک منطقی State1=یک منطقی State0=یک منطقی 
	PORTA=0x00;
	DDRA=0x00;

	// Port B
	// Bit7=ورودی Bit6=ورودی Bit5=ورودی Bit4=ورودی Bit3=ورودی Bit2=ورودی Bit1=ورودی Bit0=ورودی 
	// State7=یک منطقی State6=یک منطقی State5=یک منطقی State4=یک منطقی State3=یک منطقی State2=یک منطقی State1=یک منطقی State0=یک منطقی 
	PORTB=0x00;
	DDRB=0x00;

	// Port C
	// Bit7=ورودی Bit6=ورودی Bit5=ورودی Bit4=ورودی Bit3=خروجی Bit2=خروجی Bit1=خروجی Bit0=خروجی 
	// State7=یک منطقی State6=یک منطقی State5=یک منطقی State4=یک منطقی State3=0 State2=0 State1=0 State0=0 
	PORTC=0xff;
	DDRC=0x0F;

	// Port D
	// Bit7=ورودی Bit6=ورودی Bit5=ورودی Bit4=ورودی Bit3=ورودی Bit2=ورودی Bit1=ورودی Bit0=ورودی 
	// State7=یک منطقی State6=یک منطقی State5=یک منطقی State4=یک منطقی State3=یک منطقی State2=یک منطقی State1=یک منطقی State0=یک منطقی 
	PORTD=0x00;
	DDRD=0x00;

	//------------غییر فعال سازی ریجسترهای اضافی برای بهبود کارکرد پردازنده-------------
	// غییر فعال کردن تایمر و شمارنده R0
	// صفر کردن فرکانس
	// خاموش کردن خروجی شمارنده
	TCCR0=0x00;
	TCNT0=0x00;
	OCR0=0x00;

	// غییر فعال کردن تایمر و شمارنده R1
	// صفر کردن فرکانس
	// خاموش کردن خروجی شمارنده
	TCCR1A=0x00;
	TCCR1B=0x00;
	TCNT1H=0x00;
	TCNT1L=0x00;
	ICR1H=0x00;
	ICR1L=0x00;
	OCR1AH=0x00;
	OCR1AL=0x00;
	OCR1BH=0x00;
	OCR1BL=0x00;

	// غییر فعال کردن تایمر و شمارنده R2 و ASSR
	// صفر کردن فرکانس
	// خاموش کردن خروجی شمارنده
	ASSR=0x00;
	TCCR2=0x00;
	TCNT2=0x00;
	OCR2=0x00;

	// صفر کردن حافظه سر ریز
	TIMSK=0x00;

	//تنظیم وقفه ساز
	//حساس کردن آن به لبه پایین رونده کلاک
	MCUCR=0x00;
	MCUCSR=0x00;

	//USART غییر فعال کردن
	//B6,B7 برای استفاده از پورتهای
	UCSRB=0x00;

	// غییر فعال کردن پایه های آنالوگ
	ACSR=0x80;
	SFIOR=0x00;
	ADCSRA=0x00;
	SPCR=0x00;
	TWCR=0x00;

	// راه اندازی اولیه مانیتور
	// RS - PORTA Bit 0
	// RD - PORTA Bit 1
	// EN - PORTA Bit 2
	// D4 - PORTA Bit 4
	// D5 - PORTA Bit 5
	// D6 - PORTA Bit 6
	// D7 - PORTA Bit 7
	// Characters/line: 16
	lcd_init(16);
	
	// حلقه زندگی برنامه
	while (1)
	{
		// چک کردن و تعیین وضعیت دکمه انتخابی 
		 for(i=0;i<4;i++)
		{
			// انتخاب یک سطر برای بدست اوردن دکمه انتخاب شده
			PORTC=row[i];
			
			//چک کردن سطر به سطر از 0 تا 3
			if(i==0)
			{
				// چک کردن سطون ها به ترتیب در صورتی که سطر 0 چک می شود
				if(PINC.4==0)
				{
					while(PINC.4==0)
					{}
					lcd_putchar('7');
					setNum(7);
				}
				if(PINC.5==0)
				{
					while(PINC.5==0)
					{}
					lcd_putchar('8');
					setNum(8);
				}
				if(PINC.6==0)
				{
					while(PINC.6==0)
					{}
					lcd_putchar('9');
					setNum(9);
				}
				if(PINC.7==0)
				{
					while(PINC.7==0)
					{}
					if(flag==0 || flag2==1)
					{
						flag=1;
						lcd_putchar('/');
						flag2=0;
						mode=1;
					}
				}     
			}
			else
			{
				if(i==1)
				{
					// چک کردن سطون ها به ترتیب در صورتی که سطر 1 چک می شود
					
					if(PINC.4==0)
					{
						while(PINC.4==0)
						{}
						lcd_putchar('4');
						setNum(4);
					}
					if(PINC.5==0)
					{
						while(PINC.5==0)
						{}
						lcd_putchar('5');
						setNum(5);
					}
					if(PINC.6==0)
					{
						while(PINC.6==0)
						{}
						lcd_putchar('6');
						setNum(6);
					}
					if(PINC.7==0)
					{
						while(PINC.7==0)
						{}
						if(flag==0 || flag2==1)
						{
							flag=1; 
							flag2=0;
							lcd_putchar('*');
							mode=2;
						}
					}     
				}
				else
				{
					// چک کردن سطون ها به ترتیب در صورتی که سطر 2 چک می شود
					if(i==2)
					{
						if(PINC.4==0)
						{
							while(PINC.4==0)
							{}
							lcd_putchar('1');
							setNum(1);
						}
						if(PINC.5==0)
						{
							while(PINC.5==0)
							{}
							lcd_putchar('2');
							setNum(2);
						}
						if(PINC.6==0)
						{
							while(PINC.6==0)
							{}
							lcd_putchar('3');
							setNum(3);
						}
						if(PINC.7==0)
						{
							while(PINC.7==0)
							{}
							if(flag==0 || flag2==1)
							{
								flag=1;  
								flag2=0;
								lcd_putchar('-');
								mode=3;
							}
						}     
					}
					else
					{
						// چک کردن سطون ها به ترتیب در صورتی که سطر 3 چک می شود
						if(i==3)
						{
							if(PINC.4==0)
							{
								while(PINC.4==0)
								{}
								num1=0;
								num2=0;
								flag=0;
								flag2=0;
								lcd_clear();
							}
							if(PINC.5==0)
							{
								while(PINC.5==0)
								{}
								lcd_putchar('0');
								setNum(0);
							}
							if(PINC.6==0)
							{
								while(PINC.6==0)
								{}
								switch(mode)
								{
									case 1:
									{
										dev();
										break;
									}
									case 2:
									{
										mul();
										break;
									}
									case 3:
									{
										sub();
										break;
									}
									case 4:
									{
										sum();
										break;
									}
								}
							}
							if(PINC.7==0)
							{
								while(PINC.7==0)
								{}
								if(flag==0 || flag2==1)
								{
									flag=1;
									flag2=0;
									lcd_putchar('+');
									mode=4;
								}
							}     
						}
					}
				}
			}
			
		}
	}
}


//----------------پیاده سازی متدهای تعریف شده-----------
void setNum(int n)// متد مقدار دهی عملگرها
{
	// برای عملوند دوم
    if(flag==1)
    {
		// اضافه کردن رقم جدید انتخاب شده به عنوان یکان جدید
		// مثلا الان مقدار عملگر 1 است و کار بر دکمه 5 را وارد کرده 
		// در این حالت نتیجه میشود 15
        num2=(num2)*10;
        num2+=n;
    }
	// برای عملوند اول
    else if(flag==0)
    {
		// اضافه کردن رقم جدید انتخاب شده به عنوان یکان جدید
		// مثلا الان مقدار عملگر 1 است و کار بر دکمه 5 را وارد کرده 
		// در این حالت نتیجه میشود 15
        num1=(num1)*10;
        num1+=n;
    }          
}

void sum()// متد جمع
{
    int answer=num1+num2;
    num1=answer;
    num2=0;
    lcd_clear();
    sprintf(buffer,"%d",answer);
    lcd_puts(buffer);
    flag2=1;
}

void mul()//متد ضرب
{
    int answer=num1*num2;
    num2=0;
    num1=answer;
    lcd_clear();
    sprintf(buffer,"%d",answer);
    lcd_puts(buffer);
    flag2=1;
}

void dev()//متد تقسیم
{
    int answer=num1/num2;
    num2=0;
    lcd_clear();
    num1=answer;
    sprintf(buffer,"%d",answer);
    lcd_puts(buffer);
    flag2=1;
}

void sub()// متد تفریق
{
    int answer=num1-num2;
    num1=answer;
    num2=0;
    lcd_clear();
    sprintf(buffer,"%d",answer);
    lcd_puts(buffer); 
    flag2=1;
}