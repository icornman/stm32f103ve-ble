// LEDs
#define RED_LED 15
#define GREEN_LED 14
#define BLUE_LED 13

#define RED_LED_ON GPIOB->BRR = (1 << RED_LED);
#define RED_LED_OFF GPIOB->BSRR = (1 << RED_LED);
#define GREEN_LED_ON GPIOB->BRR = (1 << GREEN_LED);
#define GREEN_LED_OFF GPIOB->BSRR = (1 << GREEN_LED);
#define BLUE_LED_ON GPIOB->BRR = (1 << BLUE_LED);
#define BLUE_LED_OFF GPIOB->BSRR = (1 << BLUE_LED);

// BLE
#define POWER_KEY 0
#define BT_RESET 9
#define BT_KEY 7

void GPIO_init (void);
void USART_init (void);
