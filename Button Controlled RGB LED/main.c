#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h" //Periperharals Base Addresses

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

int main(void)
{
	SysCtlClockSet(SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ|SYSCTL_SYSDIV_5);  //Gömülü Osilatör
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //F Portu GPIO Pini Aktif Hale Getiriliyor
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3); //PF2 ve PF3 Output olarak ayarlaniyor.
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD); //PF2 ve PF3 çalisma kalibrasyonlari yapiliyor
	
	GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6);  //PF4 ve PF0 Input olarak ayarlaniyor.
	GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD_WPU); //Buton pull-up olarak ayarlaniyor
	
	while(1)
  {
    uint32_t pinVal_1=0;	// variable to hold the pinRead
		uint32_t pinVal_2=0; //
		uint32_t pinVal_3=0;
		
    pinVal_1 = GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_4);	// read F4
		pinVal_2 = GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_5);	// read F0
		pinVal_3 = GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_6);
		
		if((pinVal_1 & GPIO_PIN_4)==0)
		{	// AND to strip out anything but the value read from F4
      GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1); //LED yakma
    }
    
		if((pinVal_2 & GPIO_PIN_5)==0)
		{
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2); //LED yakma
		}
		
		if((pinVal_3 & GPIO_PIN_6)==0)
		{
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
		}

    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, !GPIO_PIN_1 | !GPIO_PIN_2 | !GPIO_PIN_3);
 
	}
	
}
