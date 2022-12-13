

#include<stdint.h>
#include<stdbool.h>
//#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "inc/tm4c123gh6pm.h"

// Define Macro to convert integer address to pointer
#define HWREGB(x,pin) (*((volatile uint32_t *)((x) | pin << 2)))

/*
GPIO Port A (APB) base: 0x4000.4000
GPIO Port A (AHB) base: 0x4005.8000
GPIO Port B (APB) base: 0x4000.5000
GPIO Port B (AHB) base: 0x4005.9000
GPIO Port C (APB) base: 0x4000.6000
GPIO Port C (AHB) base: 0x4005.A000
GPIO Port D (APB) base: 0x4000.7000
GPIO Port D (AHB) base: 0x4005.B000
GPIO Port E (APB) base: 0x4002.4000
GPIO Port E (AHB) base: 0x4005.C000
GPIO Port F (APB) base: 0x4002.5000
GPIO Port F (AHB) base: 0x4005.D000
*/

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

void ext_delay(uint32_t);

int main(void)
{
    SYSCTL_RCC_R = 0x02400540;
    uint8_t led = 0;
    // Set the RCGC register for clock gating of port f peripheral
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;

    // GPIO direction
    //Input -> 0
    //Output -> 1
    GPIO_PORTF_DIR_R |= GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
    // Set the pull up select register GPIOPUR
    GPIO_PORTF_PUR_R |= GPIO_PIN_0 | GPIO_PIN_4;

    // Set the drive current
    GPIO_PORTF_DR8R_R = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;

    // Unlock F0
    GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
    GPIO_PORTF_CR_R = GPIO_PIN_0;

    // GPIO Digital Enable
    GPIO_PORTF_DEN_R = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4;
    ext_delay(3);

    while(1) {
        //HWREGB(GPIO_PORTF_APB_ADDR,(GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3)) = GPIO_PIN_3;
        if(!HWREGB(GPIO_PORTF_APB_ADDR,GPIO_PIN_0)) {
            led+=2;
            HWREGB(GPIO_PORTF_APB_ADDR,(GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3)) = 2;
            ext_delay(6666667);
            //while(!HWREGB(GPIO_PORTF_APB_ADDR,GPIO_PIN_0)); // Wait until switch is closed
        }
        if(!HWREGB(GPIO_PORTF_APB_ADDR,GPIO_PIN_4)) {
            led+=2;
            HWREGB(GPIO_PORTF_APB_ADDR,(GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3)) = 4;
            ext_delay(6666667);
            //while(!HWREGB(GPIO_PORTF_APB_ADDR,GPIO_PIN_4)); // Wait until switch is closed
        }
        HWREGB(GPIO_PORTF_APB_ADDR,(GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3)) = 8;
    }

	return 0;
}

void ext_delay(uint32_t cnt) {
    volatile uint32_t pcnt = cnt;
    while(cnt--);
}
