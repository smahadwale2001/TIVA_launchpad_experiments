
#include<stdint.h>
#include<stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"

void ext_delay(uint32_t);

int main(void)
{
    uint8_t led;
    led = 2;
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlDelay(3);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    while(1) {
        GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,led);
        led+=2;
        //ext_delay(33000000); //6.25 s
        //ext_delay(57142857); //10 s
        //ext_delay(28571429);  // 5 sec
        ext_delay(6666667); //1 sec
    }

    return 0;
}

void ext_delay(uint32_t delay_cnt) {
    uint32_t cnt=delay_cnt;
    while(cnt--);
}
