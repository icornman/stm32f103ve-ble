#include "tools.h"

void GPIO_setup(GPIO_TypeDef *__port, uint8_t __pin, uint8_t __mode)
{
	*(uint64_t *)(__port) &= ~((uint64_t)0x000000000000000F << __pin * 4);
	*(uint64_t *)(__port) |= (uint64_t)__mode << __pin * 4;
}

void Delay_ms(uint32_t ms)
{
	ms *= 1000;
	for (volatile uint32_t i = 0; i < ms; ++i)
	{
		for (volatile uint32_t j = 0; j < 500; ++j)
			;
	}
}

void Delay_us(uint32_t ms)
{
	ms *= 50;
	for (volatile uint32_t i = 0; i < ms; ++i)
	{
		for (volatile uint32_t j = 0; j < 50; ++j)
			;
	}
}

void Clear_buff(char *buff, uint8_t size)
{
	while (size)
	{
		*buff++ = 0;
		size--;
	}
}
