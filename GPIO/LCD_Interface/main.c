#include<stdint.h>
#include<stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include<string.h>


#define HWREGB(x,pin) (*((volatile uint32_t *)((x) | pin << 2)))

#define GPIO_PORTA_APB_ADDR 0x40004000
#define GPIO_PORTB_APB_ADDR 0x40005000
#define GPIO_PORTC_APB_ADDR 0x40006000
#define GPIO_PORTD_APB_ADDR 0x40007000
#define GPIO_PORTE_APB_ADDR 0x40024000
#define GPIO_PORTF_APB_ADDR 0x40025000


#define GPIO_PIN_0 0x00000001
#define GPIO_PIN_1 0x00000002
#define GPIO_PIN_2 0x00000004
#define GPIO_PIN_3 0x00000008
#define GPIO_PIN_4 0x00000010
#define GPIO_PIN_5 0x00000020
#define GPIO_PIN_6 0x00000040
#define GPIO_PIN_7 0x00000080

/*
 * RS -> PB5
 * RW -> PB0
 * EN -> PB1
 * DB0 -> PD0
 * DB1 -> PD1
 * DB2 -> PD2
 * DB3 -> PD3
 * DB4 -> PC4
 * DB5 -> PC5
 * DB6 -> PC6
 * DB7 -> PC7
 */

#define RS GPIO_PIN_5
#define RW GPIO_PIN_0
#define EN GPIO_PIN_1

#define LowerNibble GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3
#define UpperNibble GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7


void ext_delay(uint32_t);
void delay_us(uint32_t);
void LCD_CMD(uint32_t);


int main(void)
{
    // Set the Clock
    SYSCTL_RCC_R = 0x02400540;

    // Enable Peripheral Clock Gating
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1 | SYSCTL_RCGCGPIO_R2 | SYSCTL_RCGCGPIO_R3;

    // GPIODIR Register Set
    GPIO_PORTB_DIR_R = RS|RW|EN;
    GPIO_PORTD_DIR_R = LowerNibble;
    GPIO_PORTC_DIR_R = UpperNibble;

    GPIO_PORTB_DEN_R = RS|RW|EN;
    ext_delay(3);

    GPIO_PORTD_DEN_R = LowerNibble;
    ext_delay(3);

    GPIO_PORTC_DEN_R = UpperNibble;
    ext_delay(3);

    LCD_CMD(0x38);
    LCD_CMD(0x01);
    delay_us(2000);
    while(1);

	return 0;
}

void ext_delay(uint32_t cnt) {
    volatile uint32_t pcnt = cnt;
    while(cnt--);                               //6 clock per loop
}

void delay_us(uint32_t us) {
    volatile uint32_t clks = 40*us;
    ext_delay(us);
}

void LCD_CMD(uint32_t cmd) {
    HWREGB(GPIO_PORTB_APB_ADDR,RS) = 0x00;
    delay_us(50);
    HWREGB(GPIO_PORTD_APB_ADDR,LowerNibble) = cmd;
    HWREGB(GPIO_PORTC_APB_ADDR,UpperNibble) = cmd;
    delay_us(50);
    HWREGB(GPIO_PORTB_APB_ADDR,EN) = EN;
    delay_us(10);
    HWREGB(GPIO_PORTB_APB_ADDR,EN) = 0;
    delay_us(50);
}
