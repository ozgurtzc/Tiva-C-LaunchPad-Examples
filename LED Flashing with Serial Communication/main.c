#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h" //Her hangi bir Peripheral(Çevre Birimi) kullaniyorsak. 
#include "inc/hw_ints.h"	 //Interreupt kullanacagimiz için

//Kullanacagimiz Peripherallari ekliyoruz.
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h" 
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"

void Int_Uart(void) //Bigisayar tarafindan gönderilen verileri okumak için kullanilacak fonksiyon
{
	char led_atch; // Gelecek veriyi aktarilacak degisken tanimlaniyor
	uint32_t IntStatus; //Interrupt in durumunu okumak icin 32bitlik degisken tanimlaniyor
	
	IntStatus = UARTIntStatus(UART0_BASE, true); //Interrupt degeri ataniyor
	UARTIntClear(UART0_BASE, IntStatus); //Tekrar Interrupta dusebilmesi icin siliyoruz.
	
	led_atch = UARTCharGetNonBlocking(UART0_BASE); //Gelen datayi al ve degiskene ata
	
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4, !GPIO_PIN_0 | !GPIO_PIN_1 | !GPIO_PIN_2 | !GPIO_PIN_3 | !GPIO_PIN_4);
	
	if(led_atch == '1')
	{
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, GPIO_PIN_0);
	}
	
	if(led_atch == '2')
	{
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, GPIO_PIN_1);
	}
	
	if(led_atch == '3')
	{
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, GPIO_PIN_2);
	}
	
	if(led_atch == '4')
	{
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_3);
	}
	
	if(led_atch == '5')
	{
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_PIN_4);
	}
	
	if(led_atch == 'x')
	{
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4, !GPIO_PIN_0 | !GPIO_PIN_1 | !GPIO_PIN_2 | !GPIO_PIN_3 | !GPIO_PIN_4);
	}
	
}


int main(void)
{
	SysCtlClockSet(SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ | SYSCTL_SYSDIV_5);  //Gömülü Osilatör
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0); //UART imizi aktif hale getiriyoruz.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); //GPIOA üzerinde çalistigi için bunuda aktif hale getiriyoruz.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); //LED ler GPIOB üzerinde oldugu icin aktif hale getiriliyor.
	
	//Pin kalibrasyonlari yapiliyor.
	GPIOPinConfigure(GPIO_PA0_U0RX); 
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4);
	GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD);
	
	IntDisable(INT_UART0); //UART0 a ait Interrupt kapatiliyor.
	UARTDisable(UART0_BASE); //UART0 kapatiliyor.
	UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC); //UART için pi osilatörü kullaniliyor
	UARTConfigSetExpClk(UART0_BASE, 16000000, 9600, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE); //UART ayarlamalari yapiliyor.
	UARTFIFODisable(UART0_BASE); //FIFO kapatiliyor
	UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT); //Interruptlar aktiflestiriliyor(Veri geldiginde yada timeout oldugunda)
	UARTIntRegister(UART0_BASE, Int_Uart); //Veri geldiginde hangi fonksiyona dallanacagi söyleniyor.
	UARTEnable(UART0_BASE); //UART aktiflestiriliyor.
	
	IntEnable(INT_UART0); //Interruptlar aktiflestiriliyor
	
	
	while(1)
	{

	}
	
}
