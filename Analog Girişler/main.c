#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h" //Her hangi bir Peripheral(Çevre Birimi) kullaniyorsak. 
#include "inc/hw_ints.h"	 //Interreupt kullanacagimiz için

//Kullanacagimiz Peripherallari ekliyoruz.
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h" 
#include "driverlib/uart.h"
#include "driverlib/adc.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"

bool isAdcReadOk = false;
char str[100];

uint32_t measuredValue = 0;

void Int_Adc(void)
{
	uint32_t AdcValues[1];
	
	ADCIntClear(ADC0_BASE, 1);
	ADCSequenceDataGet(ADC0_BASE, 1, AdcValues);
	
	measuredValue = AdcValues[0];
	
	isAdcReadOk = true;
	
}

void SendUartString(uint32_t nBase, const char *nData)
{
	while(*nData != '\0')
	{
		if(UARTSpaceAvail(nBase))
		{
			while(!UARTCharPutNonBlocking(nBase, *nData));
			
			nData++;
		}
		
	}
	
}


int main(void)
{
		SysCtlClockSet(SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ | SYSCTL_SYSDIV_5);
		
		SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
		
		SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
		GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3 );
		ADCSequenceDisable(ADC0_BASE, 1);
		ADCHardwareOversampleConfigure(ADC0_BASE, 64);
		ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
		ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH0 |ADC_CTL_IE | ADC_CTL_END);
		ADCSequenceEnable(ADC0_BASE, 1);
		ADCIntClear(ADC0_BASE, 1);
		ADCIntEnable(ADC0_BASE, 1);
		ADCIntRegister(ADC0_BASE, 1, Int_Adc);
		
		GPIOPinConfigure(GPIO_PA0_U0RX);
		GPIOPinConfigure(GPIO_PA1_U0TX);
		GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
		
		IntDisable(INT_UART0);
		UARTDisable(UART0_BASE);
		UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
		UARTConfigSetExpClk(UART0_BASE, 16000000, 115200, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
		UARTFIFODisable(UART0_BASE);
		UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
		//UARTIntRegister(UART0_BASE, Int_Uart); 
		UARTEnable(UART0_BASE);
		
		IntEnable(INT_UART0);
		IntEnable(INT_ADC0SS3);
	
	
	while(1)
	{
				ADCProcessorTrigger(ADC0_BASE, 1); // 0 BASE çalistiriliyor.
				
				sprintf( str, "%d\r\n", measuredValue); // uint32_t char veritipine dönüstüme islemi( Bu islem hata veriyor. )
				
				isAdcReadOk = false;
				
				SendUartString(UART0_BASE, str); // Serial yazdirma (Bu kisimda da hata aliyorum !)
				SysCtlDelay((SysCtlClockGet() / 3000) * 500); //1 ms

	}
	
}
