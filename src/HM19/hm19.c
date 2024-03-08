#include "string.h"
#include "tools.h"
#include "usart.h"
#include "hm19.h"

static USART_TypeDef *usart;
static enum HM19_status status = HM19_DISCONNECTED;

void HM19_init(USART_TypeDef *__usart) {
	usart = __usart;
}

void HM19_send_command(char *command) {
	USART_print_string (usart, command);
}

void HM19_send_data(char *bytes) {
	USART_send_byte(usart, (unsigned char *)bytes, sizeof(bytes));
}

enum HM19_status HM19_get_status (void) {
	return status;
}

void HM19_handler(char *data) {
	if (strcmp("OK+CONN\r\n", data) == 0) {
		status = HM19_CONNECTED;
	} else if (strcmp("OK+LOST\r\n", data) == 0) {
		status = HM19_DISCONNECTED;
	}
}
