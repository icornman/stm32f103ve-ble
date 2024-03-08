#include "stddef.h"
#include "usart.h"
#include "tools.h"

#define USART_COUNT 3

typedef struct{
	char *rx_buff;
	uint16_t rx_lng;
	uint16_t rx_timer;
	uint16_t rx_timeout;
	uint8_t rx_data_en:1;
	uint8_t reserved:7;
	uint8_t temp_byte;
} usart_struct;

static usart_struct com_port[USART_COUNT];

void USART_setup (USART_TypeDef *__usart, enum USART_pins __pins, uint32_t __br, uint8_t __bw) 
{
	switch ((uint32_t)__usart) {
		case (uint32_t)USART1:
				if (__pins == A9_TX_A10_RX) {
					GPIO_setup(GPIOA, 9, OUTPUT_A50MHZ_PP);
					GPIO_setup(GPIOA, 10, INPUT_PU_PP);
					GPIOA->BSRR = (1 << 10);
				} else if (__pins == B6_TX_B7_RX) {
					GPIO_setup(GPIOB, 6, OUTPUT_A50MHZ_PP);
					GPIO_setup(GPIOB, 7, INPUT_PU_PP);
					GPIOB->BSRR = (1 << 7);
				} else {
					return;
				}
				RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
				__usart->BRR = SystemCoreClock / __br;
				com_port[0].rx_timeout = USART_timeout_calculation(__br, __bw);
			break;
		case (uint32_t)USART2:
				if (__pins == A2_TX_A3_RX) {
					GPIO_setup(GPIOA, 2, OUTPUT_A50MHZ_PP);
					GPIO_setup(GPIOA, 3, INPUT_PU_PP);
					GPIOA->BSRR = (1 << 3);
				} else if (__pins == D5_TX_D6_RX) {
					GPIO_setup(GPIOD, 5, OUTPUT_A50MHZ_PP);
					GPIO_setup(GPIOD, 6, INPUT_PU_PP);
					GPIOD->BSRR = (1 << 6);
				} else {
					return;
				}
				RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
				__usart->BRR = SystemCoreClock / __br / 2;
				com_port[1].rx_timeout = USART_timeout_calculation(__br, __bw);
			break;
		case (uint32_t)USART3:
				if (__pins == B10_TX_B11_RX) {
					GPIO_setup(GPIOB, 10, OUTPUT_A50MHZ_PP);
					GPIO_setup(GPIOB, 11, INPUT_PU_PP);
					GPIOB->BSRR = (1 << 11);
				} else if (__pins == C10_TX_C11_RX) {
					GPIO_setup(GPIOC, 10, OUTPUT_A50MHZ_PP);
					GPIO_setup(GPIOC, 11, INPUT_PU_PP);
					GPIOC->BSRR = (1 << 11);
				} else if (__pins == D8_TX_D9_RX) {
					GPIO_setup(GPIOD, 8, OUTPUT_A50MHZ_PP);
					GPIO_setup(GPIOD, 9, INPUT_PU_PP);
					GPIOD->BSRR = (1 << 9);
				} else {
					return;
				}
				RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
				__usart->BRR = SystemCoreClock / __br / 2;
				com_port[2].rx_timeout = USART_timeout_calculation(__br, __bw);
			break;
		default:
			return;
	}
	
	__usart->CR1 |= (USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE);
	__usart->CR1 |= USART_CR1_UE;
}


void Set_USART_buff (USART_TypeDef *__usart, char *__buff)
{
	switch ((uint32_t)__usart) {
		case (uint32_t)USART1:
			com_port[0].rx_buff = __buff;
			break;
		case (uint32_t)USART2:
			com_port[1].rx_buff = __buff;
			break;
		case (uint32_t)USART3:
			com_port[2].rx_buff = __buff;
			break;
		default:
			return;
	}
}

uint32_t USART_timeout_calculation (uint32_t __br, uint8_t __bw)
{
	return (__bw*6850000)/(__br/100);
}

void USART_printf_string (USART_TypeDef *__usart, char *str, char *data)
{
	USART_print_string (__usart, str);
	USART_print_string (__usart, data);
	USART_print_string (__usart, "\n");
}

void USART_print_string (USART_TypeDef *__usart, char *str)
{
	int to = 10000000;
	while (*str)
	{
		__usart->DR = (uint8_t) *str++;
		to = 10000000;
		while (!(__usart->SR & USART_SR_TXE) && to)
		{
			to--;
		}
		if (!to)
		{
			return;
		}
	}
}

void USART_send_byte (USART_TypeDef *__usart, unsigned char *byte, int size)
{
	int to = 10000000;
	while (size)
	{
		__usart->DR = (uint8_t) *byte++;
		to = 10000000;
		while (!(__usart->SR & USART_SR_TXE) && to)
		{
			to--;
		}
		if (!to)
		{
			return;
		}
		size--;
	}
}

void USART1_receive (void)
{
	usart_struct * cp_ptr = &com_port[0];
	cp_ptr->temp_byte = (uint8_t)USART1->DR;
	if (cp_ptr->rx_buff == NULL) { return; }
	cp_ptr->rx_buff[cp_ptr->rx_lng] = cp_ptr->temp_byte;
	cp_ptr->rx_timer = cp_ptr->rx_timeout;
	cp_ptr->rx_lng++;
	cp_ptr->rx_buff[cp_ptr->rx_lng] = 0;
}

void USART2_receive (void)
{
	usart_struct * cp_ptr = &com_port[1];
	cp_ptr->temp_byte = (uint8_t)USART2->DR;
	if (cp_ptr->rx_buff == NULL) { return; }
	cp_ptr->rx_buff[cp_ptr->rx_lng] = cp_ptr->temp_byte;
	cp_ptr->rx_timer = cp_ptr->rx_timeout;
	cp_ptr->rx_lng++;
	cp_ptr->rx_buff[cp_ptr->rx_lng] = 0;
}

void USART3_receive (void)
{
	usart_struct * cp_ptr = &com_port[2];
	cp_ptr->temp_byte = (uint8_t)USART3->DR;
	if (cp_ptr->rx_buff == NULL) { return; }
	cp_ptr->rx_buff[cp_ptr->rx_lng] = cp_ptr->temp_byte;
	cp_ptr->rx_timer = cp_ptr->rx_timeout;
	cp_ptr->rx_lng++;
	cp_ptr->rx_buff[cp_ptr->rx_lng] = 0;
}

int USART1_read (char *__data, int __size)
{
	if (!com_port[0].rx_data_en) { return 0; }
	for (int i = 0; i < __size && i < com_port[0].rx_lng; i++) {
		__data[i] = com_port[0].rx_buff[i];
	}
	return com_port[0].rx_lng;
}

int USART2_read (char *__data, int __size)
{
	if (!com_port[1].rx_data_en) { return 0; }
	for (int i = 0; i < __size && i < com_port[1].rx_lng; i++) {
		__data[i] = com_port[1].rx_buff[i];
	}
	return com_port[1].rx_lng;
}

int USART3_read (char *__data, int __size)
{
	if (!com_port[2].rx_data_en) { return 0; }
	for (int i = 0; i < __size && i < com_port[2].rx_lng; i++) {
		__data[i] = com_port[2].rx_buff[i];
	}
	return com_port[2].rx_lng;
}

void USART1_read_completed(void)
{
	usart_struct * cp_ptr = &com_port[0];
	cp_ptr->rx_data_en = 0;
	cp_ptr->rx_buff[0] = 0;
	cp_ptr->rx_lng = 0;
}

void USART2_read_completed(void)
{
	usart_struct * cp_ptr = &com_port[1];
	cp_ptr->rx_data_en = 0;
	cp_ptr->rx_buff[0] = 0;
	cp_ptr->rx_lng = 0;
}

void USART3_read_completed(void)
{
	usart_struct * cp_ptr = &com_port[2];
	cp_ptr->rx_data_en = 0;
	cp_ptr->rx_buff[0] = 0;
	cp_ptr->rx_lng = 0;
}

void USART_RX_timer (void)
{
	usart_struct * cp_ptr = com_port;
	for ( int i=0; i < USART_COUNT; i++)
	{
		if (cp_ptr->rx_timer) {
			cp_ptr->rx_timer--;
			if (!cp_ptr->rx_timer) {
				cp_ptr->rx_data_en=1;
			}
		}
		cp_ptr++;
	}
}
