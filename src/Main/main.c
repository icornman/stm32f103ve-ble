#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"
#include "main.h"
#include "tools.h"
#include "usart.h"
#include "hm19.h"

void USART2_IRQHandler(void) {
	if (USART2->SR & USART_SR_RXNE) {
		USART2_receive();
	}
}

int main (void)
{
	GPIO_init();
	USART_init();
	HM19_init(USART2);
	
	USART_print_string (USART1, "[SYSTEM] DEVICE IS READY FOR WORK!\n");
	
	Delay_ms(5);
	HM19_send_command("AT");
	
	static uint8_t first_on = 1;
	
	while (1)
	{
		USART_RX_timer();
		
		static char data[128];
		int lng = USART2_read(data, 128);
		
		if (lng > 0) { 
			data[lng] = 0;
			USART_printf_string (USART1, "[DEBUG] <- ", data);
			
			if (strcmp("red-on", data) == 0) {
				RED_LED_ON
			} else if (strcmp("red-off", data) == 0) {
				RED_LED_OFF
			} else if (strcmp("green-on", data) == 0) {
				GREEN_LED_ON
			} else if (strcmp("green-off", data) == 0) {
				GREEN_LED_OFF
			} else if (strcmp("blue-on", data) == 0) {
				BLUE_LED_ON
			} else if (strcmp("blue-off", data) == 0) {
				BLUE_LED_OFF
			}
			
			if (first_on) {
				HM19_send_command("AT+NAME?");
				first_on = 0;
			}
			
			USART2_read_completed();
		}
	}
}

void GPIO_init (void)
{
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;
	
	AFIO->MAPR |= AFIO_MAPR_USART2_REMAP;
	
	GPIO_setup(GPIOC, POWER_KEY, OUTPUT_50MHZ_PP);
	GPIO_setup(GPIOE, BT_RESET, OUTPUT_50MHZ_PP);
	GPIO_setup(GPIOB, BT_KEY, OUTPUT_50MHZ_PP);
	
	GPIOC->BSRR = (1 << POWER_KEY);
	GPIOE->BSRR = (1 << BT_RESET);
	GPIOB->BSRR = (1 << BT_KEY);
	
	// LEDS
	GPIO_setup(GPIOB, RED_LED, OUTPUT_50MHZ_PP);
	GPIO_setup(GPIOB, GREEN_LED, OUTPUT_50MHZ_PP);
	GPIO_setup(GPIOB, BLUE_LED, OUTPUT_50MHZ_PP);
	RED_LED_OFF
	GREEN_LED_OFF
	BLUE_LED_OFF
}

void USART_init (void)
{
	USART_setup(USART1, A9_TX_A10_RX, 115200, 2);
	USART_setup(USART2, D5_TX_D6_RX, 9600, 2);
	
	NVIC_EnableIRQ(USART2_IRQn);
	NVIC_SetPriority(USART2_IRQn, 1);
	
	static char ble_usart_rx_buff[128] = {0};
	Set_USART_buff(USART2, ble_usart_rx_buff);
}
