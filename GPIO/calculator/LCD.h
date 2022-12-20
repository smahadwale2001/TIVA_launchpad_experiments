/*
 * LCD.h
 *
 *  Created on: 18-Dec-2022
 *      Author: root
 */

#ifndef LCD_H_
#define LCD_H_

#include<stdint.h>
#include<stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "inc/tm4c123gh6pm.h"

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
#define data_t GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
#define data_b GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3

#define SHIFT_LIMIT 40

void LCD_CMD(char);
void LCD_DATA(char);
void LCD_Init();
void LCD_String(char []);
void LCD_Clear();
void LCD_ROW_Shift();
uint8_t LCD_LSL();
uint8_t LCD_RSL();
#endif /* LCD_H_ */
