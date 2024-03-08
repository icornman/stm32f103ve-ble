#include "stm32f10x.h"
#include "stdint.h"

#define OUTPUT_50MHZ_PP 0x3
#define OUTPUT_50MHZ_OD 0xF
#define OUTPUT_A50MHZ_PP 0xB
#define OUTPUT_PP 0x3
#define INPUT_FLOATING 0x4
#define INPUT_PU_PP 0x8

void GPIO_setup(GPIO_TypeDef *__port, uint8_t __pin, uint8_t __mode);
void Delay_ms(uint32_t ms);
void Delay_us(uint32_t ms);
void Clear_buff(char *buff, uint8_t size);
