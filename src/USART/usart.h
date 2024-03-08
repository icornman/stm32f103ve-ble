#include "stm32f10x.h"

enum USART_pins {
	A9_TX_A10_RX = 0,
	B6_TX_B7_RX,
	A2_TX_A3_RX,
	D5_TX_D6_RX,
	B10_TX_B11_RX,
	C10_TX_C11_RX,
	D8_TX_D9_RX,
};

void USART_setup (USART_TypeDef *__usart, enum USART_pins __pins, uint32_t __br, uint8_t __bw);
void Set_USART_buff (USART_TypeDef *__usart, char *buff);

void USART_print_string (USART_TypeDef *__usart, char *str);
void USART_printf_string (USART_TypeDef *__usart, char *str, char *data);
void USART_send_byte (USART_TypeDef *__usart, unsigned char *byte, int size);

void USART1_receive (void);
void USART2_receive (void);
void USART3_receive (void);
int USART1_read (char *__data, int __size);
int USART2_read (char *__data, int __size);
int USART3_read (char *__data, int __size);
void USART1_read_completed(void);
void USART2_read_completed(void);
void USART3_read_completed(void);

void USART_RX_timer (void);
uint32_t USART_timeout_calculation (uint32_t __br, uint8_t __bw);
