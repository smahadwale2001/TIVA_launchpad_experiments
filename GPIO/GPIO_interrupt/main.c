#include<stdint.h>
#include<stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "inc/tm4c123gh6pm.h"

void SWInitInterrupt();
void switchLed();

uint8_t led;

int main(void)
{

    //int test=0;
    led = 2;
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlDelay(3);
    GPIOUnlockPin(GPIO_PORTF_BASE,GPIO_PIN_0);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_4,GPIO_STRENGTH_8MA,GPIO_PIN_TYPE_STD_WPU);
    SWInitInterrupt();
    while(1) {
        GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,led);
        led+=2;
        SysCtlDelay(20000000);
    }
	return 0;
}

void SWInitInterrupt() {
    IntMasterDisable();
    IntDisable(INT_GPIOF);
    GPIOIntTypeSet(GPIO_PORTF_BASE,GPIO_PIN_0 | GPIO_PIN_4,GPIO_FALLING_EDGE);
    GPIOIntClear(GPIO_PORTF_BASE,GPIO_PIN_0 | GPIO_PIN_4);
    GPIOIntRegister(GPIO_PORTF_BASE,switchLed);
    GPIOIntEnable(GPIO_PORTF_BASE,GPIO_PIN_0 | GPIO_PIN_4);
    IntMasterEnable();
}

void switchLed() {
    uint32_t status = GPIOIntStatus(GPIO_PORTF_BASE,GPIO_PIN_0 | GPIO_PIN_4);
    if(status & GPIO_PIN_0) {
        led+=2;
    }else{
        led-=2;
    }
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,led);
    GPIOIntClear(GPIO_PORTF_BASE,GPIO_PIN_0 | GPIO_PIN_4);
}
