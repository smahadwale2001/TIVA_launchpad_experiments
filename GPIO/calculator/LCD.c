/*
 * LCD.c
 *
 *  Created on: 18-Dec-2022
 *      Author: root
 */

#include "LCD.h"

void LCD_CMD(char var) {
    GPIOPinWrite(GPIO_PORTB_BASE,RS,0);
    SysCtlDelay(40);
    GPIOPinWrite(GPIO_PORTD_BASE,data_b,var);
    GPIOPinWrite(GPIO_PORTC_BASE,data_t,var);
    SysCtlDelay(40);
    GPIOPinWrite(GPIO_PORTB_BASE,EN,EN);
    SysCtlDelay(40);
    /*GPIOPinWrite(GPIO_PORTD_BASE,data_b,var&0x0F);
    SysCtlDelay(40);
    GPIOPinWrite(GPIO_PORTC_BASE,data_t,var&0xF0);
    SysCtlDelay(40);*/
    GPIOPinWrite(GPIO_PORTB_BASE,EN,0);
    SysCtlDelay(40);
    SysCtlDelay(4000);
}

void LCD_DATA(char var) {
    GPIOPinWrite(GPIO_PORTB_BASE,RS,RS);
    SysCtlDelay(40);
    GPIOPinWrite(GPIO_PORTD_BASE,data_b,var);
    GPIOPinWrite(GPIO_PORTC_BASE,data_t,var);
    SysCtlDelay(40);
    GPIOPinWrite(GPIO_PORTB_BASE,EN,EN);
    SysCtlDelay(40);
    GPIOPinWrite(GPIO_PORTB_BASE,EN,0);
    SysCtlDelay(4000);
}

void LCD_Init() {
    GPIOPinWrite(GPIO_PORTB_BASE,RW,0);
    SysCtlDelay(40);
    GPIOPinWrite(GPIO_PORTB_BASE,RS,0);
    SysCtlDelay(40);
    GPIOPinWrite(GPIO_PORTB_BASE,EN,0);
    SysCtlDelay(40);
    //LCD_CMD(0x20);
    LCD_CMD(0x38);          // Function Set in Datasheet 1 DL N F x x -> 0x38
    LCD_CMD(0x0C);          // Display Switch 1 D C B -> 0x0c
    LCD_CMD(0x06);          // Input Set 1 I/D S -> 0x06
    LCD_Clear();
    //LCD_CMD(0x0F);
}

void LCD_String(char str[]) {
    int i=0;
    while(str[i] != '\0') {
        LCD_DATA(str[i]);
        i++;
    }
}

void LCD_Clear() {
    LCD_CMD(0x01);
    SysCtlDelay(20000);
}

void LCD_ROW_Shift() {
    static bool row = 0;
    if(row) {
        LCD_CMD(0x80);          // Sets DDRAM Address at location 64
        row = 0;
    }else{
        LCD_CMD(0xC0);          // Sets DDRAM Address at location 0
        row = 1;
    }
}

uint8_t LCD_LSL() {
    LCD_CMD(0x18);
    return 0;
}

uint8_t LCD_RSL() {
    LCD_CMD(0x0C);
    return 0;
}


