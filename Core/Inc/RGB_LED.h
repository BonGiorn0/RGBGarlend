#ifndef RGB_LED_H
#define RGB_LED_H

#include "stm32f4xx_hal.h"

typedef struct RGB_LED{
	TIM_HandleTypeDef *tim;
	uint32_t red_tim_channel;
	uint32_t green_tim_channel;
	uint32_t blue_tim_channel;
	uint32_t hex_color;
	uint8_t on;
}RGB_LED_t;

void RGB_LED_Init(RGB_LED_t *led, TIM_HandleTypeDef *tim,
		uint32_t r_tim_channel, uint32_t g_tim_channel, uint32_t b_tim_channel);
void RGB_LED_On(RGB_LED_t *led);
void RGB_LED_Off(RGB_LED_t *led);
void RGB_LED_SetColor(RGB_LED_t *led, uint32_t hex_color);
uint32_t interpolateColors(uint32_t color1, uint32_t color2, float ratio);
void RGB_LED_Gradient(RGB_LED_t *led, uint32_t col1, uint32_t col2, float duration, float step);

#endif
