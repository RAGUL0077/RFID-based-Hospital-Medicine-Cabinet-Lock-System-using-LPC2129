#include <LPC21xx.h>

#define LCD_RS (1 << 24)
#define LCD_EN (1 << 25)

#define LCD_D4 (1 << 16)
#define LCD_D5 (1 << 17)
#define LCD_D6 (1 << 18)
#define LCD_D7 (1 << 19)

#define LCD_DATA (LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7)


#define MOTOR_IN1 (1 << 26)
#define MOTOR_IN2 (1 << 27)

#define LED (1 << 28)
#define BUZZER (1 << 29)

#define SWITCH (1 << 14)

unsigned char AUTH_ID1[] = "0003893303";
unsigned char AUTH_ID2[] = "0987654321";

void timer0_init(void)
{
    T0TCR = 0x02;
    T0PR = 14;
    T0TCR = 0x01;
}

void delay_us(unsigned int us)
{
    T0TCR = 0x02;
    T0TCR = 0x01;

    while(T0TC < us);

    T0TCR = 0x00;
}
   void delay_ms(unsigned int ms)
{
    while(ms--)
    {
        delay_us(1000);
    }
}

void delay_sec(unsigned int sec)
{
    while(sec--)
    {
        delay_ms(1000);
    }
}
void LCD_Enable(void)
{
    IO1SET = LCD_EN;

    delay_us(2);

    IO1CLR = LCD_EN;

    delay_us(100);
}

void LCD_Send4Bit(unsigned char data)
{
    IO1CLR = LCD_DATA;

    if(data & 0x01)
        IO1SET = LCD_D4;

    if(data & 0x02)
        IO1SET = LCD_D5;

    if(data & 0x04)
        IO1SET = LCD_D6;

    if(data & 0x08)
        IO1SET = LCD_D7;

    LCD_Enable();
}

void LCD_Command(unsigned char cmd)
{
    IO1CLR = LCD_RS;

    LCD_Send4Bit(cmd >> 4);
    LCD_Send4Bit(cmd & 0x0F);

    delay_ms(2);
}

void LCD_Data(unsigned char data)
{
    IO1SET = LCD_RS;

    LCD_Send4Bit(data >> 4);
    LCD_Send4Bit(data & 0x0F);

    delay_us(100);
}

void LCD_Init(void)
{
    delay_ms(20);

    IO1CLR = LCD_RS;
    LCD_Send4Bit(0x03);
    delay_ms(5);

    LCD_Send4Bit(0x03);
    delay_us(150);

    LCD_Send4Bit(0x03);

    LCD_Send4Bit(0x02);

    LCD_Command(0x28);

    LCD_Command(0x0C);

    LCD_Command(0x06);

    LCD_Command(0x01);

    delay_ms(2);
}

void LCD_Clear(void)
{
    LCD_Command(0x01);
    delay_ms(2);
}

void LCD_GotoXY(unsigned char row, unsigned char column)
{
    if(row == 0)
        LCD_Command(0x80 + column);
    else
        LCD_Command(0xC0 + column);
}

void LCD_String(char *str)
{
    while(*str)
    {
        LCD_Data(*str);
        str++;
    }
}

void UART0_Init(void)
{

    PINSEL0 &= ~0x0000000F;
    PINSEL0 |= 0x00000005;

    U0LCR = 0x83;

    U0DLL = 97;
    U0DLM = 0;

    U0LCR = 0x03;
}

unsigned char UART0_RxChar(void)
{
    while((U0LSR & 0x01) == 0);

    return U0RBR;
}

void UART0_TxChar(unsigned char data)
{
    while((U0LSR & 0x20) == 0);

    U0THR = data;
}

void RFID_Read(unsigned char *rfid)
{
    unsigned char ch;
    unsigned int i = 0;

    while(1)
    {
        ch = UART0_RxChar();
        if(ch == '\n')
            continue;
        if(ch == '\r')
        {
            break;
        }

        if(i < 10)
        {
            rfid[i] = ch;
            i++;
        }
    }

    rfid[i] = '\0';
}

unsigned char StringCompare(unsigned char *str1,
                            unsigned char *str2)
{
    unsigned int i = 0;

    while(str1[i] != '\0' || str2[i] != '\0')
    {
        if(str1[i] != str2[i])
            return 0;

        i++;
    }

    return 1;
}

void Motor_Stop(void)
{
    IO1CLR = MOTOR_IN1 | MOTOR_IN2;
}

void Motor_Clockwise(void)
{
    IO1SET = MOTOR_IN1;
    IO1CLR = MOTOR_IN2;
}

void Motor_Anticlockwise(void)
{
    IO1CLR = MOTOR_IN1;
    IO1SET = MOTOR_IN2;
}

void LED_ON(void)
{
    IO1SET = LED;
}

void LED_OFF(void)
{
    IO1CLR = LED;
}


void Buzzer_ON(void)
{
    IO1SET = BUZZER;
}

void Buzzer_OFF(void)
{
    IO1CLR = BUZZER;
}

void GPIO_Init(void)
{
    IO1DIR |= LCD_DATA;
    IO1DIR |= LCD_RS | LCD_EN;

    IO1DIR |= MOTOR_IN1 | MOTOR_IN2;

    IO1DIR |= LED;
    IO1DIR |= BUZZER;

    Motor_Stop();

    LED_OFF();

    Buzzer_OFF();

    IO0DIR &= ~SWITCH;
}

unsigned char Switch_Pressed(void)
{
    if((IO0PIN & SWITCH) == 0)
        return 1;

    return 0;
}

void Authorized_Access(void)
{
    LCD_Clear();

    LCD_GotoXY(0,0);
    LCD_String("Authorized ID");

    LED_ON();

    delay_ms(1000);

    LCD_Clear();

    LCD_GotoXY(0,0);
    LCD_String("LED ON");

    LCD_GotoXY(1,0);
    LCD_String("Motor ON-Open");

    Motor_Clockwise();

    delay_sec(5);

    Motor_Stop();

    LED_OFF();

    LCD_Clear();

    LCD_GotoXY(0,0);
    LCD_String("Cabinet Open");

    LCD_GotoXY(1,0);
    LCD_String("Press Switch");

    while(!Switch_Pressed());

    delay_ms(100);

    if(Switch_Pressed())
    {
        LCD_Clear();

        LCD_GotoXY(0,0);
        LCD_String("Motor ON");

        LCD_GotoXY(1,0);
        LCD_String("Closing");

        Motor_Anticlockwise();

        delay_sec(5);

        Motor_Stop();

        LCD_Clear();

        LCD_GotoXY(0,0);
        LCD_String("Cabinet Closed");

        delay_sec(2);
    }
}

void Unauthorized_Access(void)
{
    Motor_Stop();
    LED_OFF();

    LCD_Clear();

    LCD_GotoXY(0,0);
    LCD_String("Unauthorized ID");

    Buzzer_ON();

    LCD_GotoXY(1,0);
    LCD_String("Buzzer ON");
    delay_sec(2);

    Buzzer_OFF();

    LCD_Clear();

    LCD_GotoXY(0,0);
    LCD_String("Scan RFID");
}

 void UART_Test(void)
{
    unsigned char ch;

    LCD_Clear();
    LCD_GotoXY(0, 0);
    LCD_String("Waiting RFID");

    ch = UART0_RxChar();

    LCD_Clear();
    LCD_GotoXY(0, 0);
    LCD_String("Data Received");

    LCD_GotoXY(1, 0);
    LCD_Data(ch);

    delay_ms(2000);
}

int main(void)
{
    unsigned char rfid[20];
    GPIO_Init();
    timer0_init();

    LCD_Init();
    UART0_Init();
    LCD_Clear();

    LCD_GotoXY(0,0);
    LCD_String("Medicine");

    LCD_GotoXY(1,0);
    LCD_String("Cabinet");

    delay_sec(5);

    LCD_Clear();

    LCD_GotoXY(0,0);
    LCD_String("Scan RFID");

    while(1)
    {

        RFID_Read(rfid);

        if(StringCompare(rfid, AUTH_ID1))
        {
            Authorized_Access();
        }


        else if(StringCompare(rfid, AUTH_ID2))
        {
            Authorized_Access();
        }


        else
        {
            Unauthorized_Access();
        }
        LCD_Clear();

        LCD_GotoXY(0,0);
        LCD_String("Scan RFID");
    }

while(1)
{
    UART_Test();
}
}
	
	