#include<stdint.h>
#include<stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "inc/tm4c123gh6pm.h"
#include "LCD.h"

struct equation{
    char str[41];
    uint8_t str_index;
};

struct eqn_stack{
    int num;
    bool eqn;
};

struct equation cal;

int shift_pos = 0;

void keyPadInit();
void keyPad();
int calculate();

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_20 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlDelay(3);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,RS|RW|EN);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlDelay(3);
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE,data_b);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlDelay(3);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE,data_t);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlDelay(3);
    GPIOUnlockPin(GPIO_PORTF_BASE,GPIO_PIN_0);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlDelay(3);
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    cal.str_index =0;

    LCD_Init();
    //LCD_String("HELLO WORLD");
    /*LCD_String("HELLO WORLD This is a 16x2 LCD Display 3");
    int i=0;
    for(i=0;i<30;i++) {
        LCD_CMD(0x18);
    }
    LCD_CMD(0x18); // Left Shift
    LCD_CMD(0x1C); // Right Shift*/
    keyPadInit();
    while(1);
	return 0;
}

void keyPadInit() {
    IntMasterDisable();
    IntDisable(INT_GPIOE);
    GPIOIntTypeSet(GPIO_PORTE_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 ,GPIO_RISING_EDGE);
    GPIOIntClear(GPIO_PORTE_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOIntRegister(GPIO_PORTE_BASE,keyPad);
    GPIOIntEnable(GPIO_PORTE_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    IntMasterEnable();
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,0x0F);
}

void keyPad() {
    uint32_t status = GPIOIntStatus(GPIO_PORTE_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3); //<<4; // ROWS
    uint8_t col_val = 1;
    char data = 'E';
    while(col_val<16) {
        GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,col_val);
        if(GPIOPinRead(GPIO_PORTE_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3)) {
            //status += col_val;
            break;
        }
        col_val=col_val<<1;
    }
    if(status == 0x01) {
        if(col_val == 0x01) {
            data = '1';
        }else if(col_val == 0x02) {
            data = '2';
        }else if(col_val == 0x04) {
            data = '3';
        }else if(col_val == 0x08){
            data = 'A'; //A
        }
    }else if(status == 0x02) {
        if(col_val == 0x01) {
            data = '4';
        }else if(col_val == 0x02) {
            data = '5';
        }else if(col_val == 0x04) {
            data = '6';
        }else if(col_val == 0x08){
            data = '/'; //B
        }
    }else if(status == 0x04) {
        if(col_val == 0x01) {
            data = '7';
        }else if(col_val == 0x02) {
            data = '8';
        }else if(col_val == 0x04) {
            data = '9';
        }else if(col_val == 0x08){
            data = '+'; //C
        }
    }else if(status == 0x08) {
        if(col_val == 0x01) {
            data = 'x';
        }else if(col_val == 0x02) {
            data = '0';
        }else if(col_val == 0x04) {
            data = '=';
        }else if(col_val == 0x08){
            data = '-'; //D
        }
    }
    while(GPIOPinRead(GPIO_PORTE_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3));
    SysCtlDelay(200000);
    if(data == 'E') {
        SysCtlDelay(800000);
    }else if(data == 'A'){
        LCD_Clear();
        cal.str_index=0;
    }else if(data == '='){
        char ans[40];
        int num = calculate();
        LCD_ROW_Shift();
        sprintf(ans,"%d",num);
        LCD_String(ans);
        LCD_ROW_Shift();
    }else{
        int shift;
        if(cal.str_index<40) {
            cal.str[cal.str_index] = data;
            cal.str[cal.str_index+1] = 0;
            LCD_Clear();
            LCD_String(cal.str);
            for(shift=15;shift<cal.str_index;shift++) {
                LCD_LSL();
            }
            cal.str_index+=1;
        }
    }
    GPIOIntClear(GPIO_PORTE_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,0x0F);
}

int calculate() {
    uint8_t i=0;
    int num0,num1,num2;
    num0 = 0;
    num1 = 0;
    num2 = 1;
    char op;
    uint8_t flag=1;
    while(i<cal.str_index) {
        if(cal.str[i] == '-' & flag) {
            num2 = -1;
            i++;
            flag = 0;
            continue;
        }
        if(cal.str[i] > 47 & cal.str[i] < 58) {
            num0 = num0 * 10+(cal.str[i]-48);
            i++;
        }else{
            break;
        }
    }
    num0 = num0*num2;
    op = cal.str[i];
    i++;
    num2 = 1;
    while(i<=cal.str_index) {
            if(cal.str[i] == '-') {
                num2 = -1;
                i++;
                continue;
            }
            if(cal.str[i] > 47 & cal.str[i] < 58) {
                num1 = num1 * 10 + (cal.str[i]-48);
                i++;
            }else{
                break;
            }
    }
    num1 = num1*num2;
    if(op == '+') {
        num2 = num0+num1;
    }else if(op == '-') {
        num2 = num0-num1;
    }else if(op == 'x') {
        num2 = num0*num1;
    }else{
        num2 = num0/num1;
    }
    return num2;
}
