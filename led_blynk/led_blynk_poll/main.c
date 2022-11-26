
#include<stdint.h>
#include<stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_nvic.h"
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"

void SysTickInit(uint32_t);
void tled();


int main(void)
{
    //uint8_t led = 0;
    SysCtlClockSet(SYSCTL_SYSDIV_20 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysTickInit(10000000);
    //SysCtlDelay(3);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    while(1) {
        if(HWREG(NVIC_ST_CTRL) & (1<<16)) {
            tled();

        }
    }

    return 0;
}

void SysTickInit(uint32_t del_val) {
    SysTickDisable();
    SysTickPeriodSet(del_val);
    SysTickEnable();
}

void tled() {
    static uint8_t led=0;
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,led);
    led+=2;
    HWREG(NVIC_ST_CURRENT) = 0;
}
