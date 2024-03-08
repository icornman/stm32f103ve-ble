#include "stm32f10x.h"

enum HM19_status {
	HM19_DISCONNECTED = 0,
	HM19_CONNECTED,
};

void HM19_init(USART_TypeDef *__usart);
void HM19_send_command(char *command);
void HM19_send_data(char *command);
void HM19_handler(char *data);
enum HM19_status HM19_get_status(void);
